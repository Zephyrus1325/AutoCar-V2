#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <webPage.h>
#include <ArduinoJson.h>
#include "esp_wpa2.h"
#include "comms.h"
#include "defines.h"

//#include "sdcard.h"
#include "navigation.h"

// Descomente essa linha para habilitar debug de tudo
#define DEBUG

AsyncWebServer server(80); // Declara um servidor e um websocket
AsyncWebSocket ws("/ws");

// Timers
timer webSocketTimer{0, 100, true, true, true};
timer mapDataTimer{10, 100, true, true, true};

// Credenciais de Rede Locais
#ifndef PROGRAMMING
    const char* ssid = "AutoCar_V2";
    const char* password = "vroomvroom";
#else
// Credenciais de Rede do LabMaker
//const char* ssid = "LabMaker_Teste";
//const char* password = "LabMaker";
    const char* ssid = "MarcoFilho";
    const char* password = "MarcoFilho12";
#endif

// Variáveis do carrinho, estão de exemplo aqui
CarData car;
Navigation nav;
int32_t navigationMode = 0;
int8_t chunkCounter = 0;
bool batteryAlert = false;

TaskHandle_t InterfaceTaskHandler;

void TaskSendData(void * pvParameters);

// O que retornar em caso de não encontrar o servidor
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


// Handler de Mensagens recebidas do websocket
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    
    //Checa se recebeu um websocket valido, eu acho que é isso que esse if faz
    if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT){
        // Transforma os dados recebidos (text) em um Json (Json)
        JsonDocument json; 
        DeserializationError err = deserializeJson(json, data);
        // Em caso de erro de serialização
        if(err){
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(err.c_str());
            return;
        }
        
        // Recebe o que precisar ser mudado e para qual valor vai ser alterado
        const char *toChange = json["toChange"];
        const char *changeTo = json["changeTo"];
        String value = changeTo;
        
        #ifdef DEBUG
            Serial.print("Command Received: ");
            Serial.print(toChange);
            Serial.print(" Value: ");
            Serial.println(value);
        #endif

        // Muda as variaveis de acordo com o que precisa ser alterado
        if (strcmp(toChange, "leftMode") == 0){
            sendCommand(COMMAND_MOTOR_LEFT_SETMODE, value.toInt());
        } else if (strcmp(toChange, "leftSetpoint") == 0){
            sendCommand(COMMAND_MOTOR_LEFT_SETSPEED, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "leftThrottle") == 0){
            sendCommand(COMMAND_MOTOR_LEFT_SETTHROTTLE, value.toInt());
        } else if (strcmp(toChange, "leftKp") == 0){
            sendCommand(COMMAND_MOTOR_LEFT_SETKP, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "leftKi") == 0){
            sendCommand(COMMAND_MOTOR_LEFT_SETKI, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "leftKd") == 0){
            sendCommand(COMMAND_MOTOR_LEFT_SETKD, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "rightMode") == 0){
            sendCommand(COMMAND_MOTOR_RIGHT_SETMODE, value.toInt());
        }else if (strcmp(toChange, "rightSetpoint") == 0){
            sendCommand(COMMAND_MOTOR_RIGHT_SETSPEED, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "rightThrottle") == 0){
            sendCommand(COMMAND_MOTOR_RIGHT_SETTHROTTLE, value.toInt());
        } else if (strcmp(toChange, "rightKp") == 0){
            sendCommand(COMMAND_MOTOR_RIGHT_SETKP, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "rightKi") == 0){
            sendCommand(COMMAND_MOTOR_RIGHT_SETKI, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "rightKd") == 0){
            sendCommand(COMMAND_MOTOR_RIGHT_SETKD, (int)(value.toFloat() * FLOAT_MULTIPLIER));
        } else if (strcmp(toChange, "navigationMode") == 0){
            navigationMode = value.toInt();
            sendCommand(COMMAND_NAVIGATION_SETMODE, navigationMode);
        } else if (strcmp(toChange, "destination") == 0){

        } else if (strcmp(toChange, "home") == 0){

        } else if (strcmp(toChange, "updateFrequency") == 0) {

        } else if(strcmp(toChange, "walk") == 0){
            sendCommand(COMMAND_TEMP_3_METERS, 0);
            nav.clearChunkData();
        }
    }
}

// Checa o que deve ser feito ao receber um evento do websocket
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

// Cria um JSON com todos os dados do carro
JsonDocument carData(){
    JsonDocument data;
    data["type"] = "carInfo";
    data["battery"]["voltage"] = (float) car.battery_voltage / FLOAT_MULTIPLIER;
    data["battery"]["percentage"] = car.battery_percentage;
    data["ultrassound"]["front"] = car.ultrassound_reading_front;
    data["ultrassound"]["front_left"] = car.ultrassound_reading_front_left;
    data["ultrassound"]["front_right"] = car.ultrassound_reading_front_right;
    data["ultrassound"]["left"] = car.ultrassound_reading_left;
    data["ultrassound"]["right"] = car.ultrassound_reading_right;
    data["ultrassound"]["back"] = car.ultrassound_reading_back;
    data["ultrassound"]["back_left"] = car.ultrassound_reading_back_left;
    data["ultrassound"]["back_right"] = car.ultrassound_reading_back_right;
    data["motor"]["left"]["mode"] = car.motor_left_mode;
    data["motor"]["left"]["setpoint"] = (float) car.motor_left_setpoint / FLOAT_MULTIPLIER;
    data["motor"]["left"]["speed"] = (float) car.motor_left_speed / FLOAT_MULTIPLIER;
    data["motor"]["left"]["throttle"] = car.motor_left_throttle;
    data["motor"]["left"]["kp"] = (float) car.motor_left_kp / FLOAT_MULTIPLIER;
    data["motor"]["left"]["ki"] = (float) car.motor_left_ki / FLOAT_MULTIPLIER;
    data["motor"]["left"]["kd"] = (float) car.motor_left_kd / FLOAT_MULTIPLIER;
    data["motor"]["right"]["mode"] = car.motor_right_mode;
    data["motor"]["right"]["setpoint"] = (float) car.motor_right_setpoint / FLOAT_MULTIPLIER;
    data["motor"]["right"]["speed"] = (float) car.motor_right_speed / FLOAT_MULTIPLIER;
    data["motor"]["right"]["throttle"] = car.motor_right_throttle;
    data["motor"]["right"]["kp"] = (float) car.motor_right_kp / FLOAT_MULTIPLIER;
    data["motor"]["right"]["ki"] = (float) car.motor_right_ki / FLOAT_MULTIPLIER;
    data["motor"]["right"]["kd"] = (float) car.motor_right_kd / FLOAT_MULTIPLIER;
    data["imu"]["gyro"]["raw"]["x"] = car.gyro_raw_x;
    data["imu"]["gyro"]["raw"]["y"] = car.gyro_raw_y;
    data["imu"]["gyro"]["raw"]["z"] = car.gyro_raw_z;
    data["imu"]["gyro"]["treated"]["x"] = (float) car.gyro_treated_x / FLOAT_MULTIPLIER;
    data["imu"]["gyro"]["treated"]["y"] = (float) car.gyro_treated_y / FLOAT_MULTIPLIER;
    data["imu"]["gyro"]["treated"]["z"] = (float) car.gyro_treated_z / FLOAT_MULTIPLIER;
    data["imu"]["acc"]["raw"]["x"] = car.acc_raw_x;
    data["imu"]["acc"]["raw"]["y"] = car.acc_raw_y;
    data["imu"]["acc"]["raw"]["z"] = car.acc_raw_z;
    data["imu"]["acc"]["treated"]["x"] = (float) car.acc_treated_x / FLOAT_MULTIPLIER;
    data["imu"]["acc"]["treated"]["y"] = (float) car.acc_treated_y / FLOAT_MULTIPLIER;
    data["imu"]["acc"]["treated"]["z"] = (float) car.acc_treated_z / FLOAT_MULTIPLIER;
    data["imu"]["mag"]["raw"]["x"] = car.mag_raw_x;
    data["imu"]["mag"]["raw"]["y"] = car.mag_raw_y;
    data["imu"]["mag"]["raw"]["z"] = car.mag_raw_z;
    data["imu"]["mag"]["treated"]["x"] = (float) car.mag_treated_x / FLOAT_MULTIPLIER;
    data["imu"]["mag"]["treated"]["y"] = (float) car.mag_treated_y / FLOAT_MULTIPLIER;
    data["imu"]["mag"]["treated"]["z"] = (float) car.mag_treated_z / FLOAT_MULTIPLIER;
    data["imu"]["baro"]["raw"]["pressure"] = car.raw_pressure;
    data["imu"]["baro"]["raw"]["temperature"] = car.raw_temperature;
    data["imu"]["baro"]["treated"]["pressure"] = (float) car.pressure / FLOAT_MULTIPLIER;
    data["imu"]["baro"]["treated"]["temperature"] = (float) car.temperature / FLOAT_MULTIPLIER;
    data["navigation"]["mode"] = navigationMode;
    data["navigation"]["position"]["x"] = car.navigation_position_x;
    data["navigation"]["position"]["y"] = car.navigation_position_y;
    data["navigation"]["position"]["z"] = car.navigation_position_z;
    data["navigation"]["position"]["pitch"] =  (float) car.navigation_position_pitch / FLOAT_MULTIPLIER;
    data["navigation"]["position"]["roll"] =  (float) car.navigation_position_roll / FLOAT_MULTIPLIER;
    data["navigation"]["position"]["heading"] =  (float) car.navigation_position_heading / FLOAT_MULTIPLIER;
    data["navigation"]["destination"]["x"] = car.navigation_destination_x;
    data["navigation"]["destination"]["y"] = car.navigation_destination_y;
    data["navigation"]["destination"]["z"] = car.navigation_destination_z;
    data["navigation"]["home"]["x"] = car.navigation_home_x;
    data["navigation"]["home"]["y"] = car.navigation_home_y;
    data["navigation"]["home"]["z"] = car.navigation_home_z;
    return data;
}



void setup() {
    Serial.begin(115200);
    Serial.println(
    "|             AUTOCAR V2            |\n"
    "|        BY: MARCO AURELIO          |\n"
    "|                2024               |\n"
    "|                                   |");
    //SD_begin();
    Serial2.begin(115200, SERIAL_8N1, 16, 17);

    WiFi.setSleep(false); // Desativar modo de economia de energia com o Wifi (adeus latencia!)
    WiFi.setTxPower(WIFI_POWER_19_5dBm);

    #ifndef PROGRAMMING
        #ifdef EXTRA_ROUTER
            WiFi.mode(WIFI_STA);    // Inicia o ESP como um STATION (cliente de uma rede)
            uint8_t waitCounter = 0;
            while (waitCounter < 2) {
                WiFi.begin(ssid, password); // Inicia o WIFI com as credenciais de rede
                // Checa primeiro se o roteador já está ligado
                if(WiFi.waitForConnectResult() == WL_CONNECTED){
                    break;
                }
                Serial.println("Waiting for Router Boot...");
                delay(25000);           // Aguarda até o roteador Ligar totalmente ~25 Segundos
            }
            if(waitCounter >= 2){
                Serial.println("Router connection Failed!");
                return; // Se a conexão com o roteador falhar, desista!
            }
            Serial.println("Connected!");
            // Printa o IP no serial para saber onde caralhos estou
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP()); 
        #else
            WiFi.mode(WIFI_AP);       // Inicia o ESP como um ACCESS POINT (ponto de acesso)
            WiFi.softAP(ssid, password); // Inicia o WIFI com as credenciais de rede
            Serial.print("IP Address: ");
            Serial.println(WiFi.softAPIP());
        #endif 
        
    #else
        WiFi.mode(WIFI_STA);    // Inicia o ESP como um STATION (cliente de uma rede)
        WiFi.begin(ssid, password); // Inicia o WIFI com as credenciais de rede
        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.println("WiFi Failed!");
        return; // Se a conexão com o roteador falhar, desista!
        }
        // Printa o IP no serial para saber onde caralhos estou
        Serial.println();
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP()); 
    #endif  
    #ifdef EXTRA_ROUTER

    #endif 

    ws.onEvent(onEvent);    // Define qual função deve rodar em um evento do WS
    server.addHandler(&ws); // Define qual é o handler de WS do servidor

    // Inicia o servidor em si
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    server.onNotFound(notFound);
    server.begin();

    xTaskCreatePinnedToCore(
            TaskSendData,  /* Task function. */
             "DataTask",    /* name of task. */
             10000,      /* Stack size of task */
             NULL,       /* parameter of the task */
             1,          /* priority of the task */
             &InterfaceTaskHandler,     /* Task handle to keep track of created task */
             0);         /* pin task to core 0 */

    pinMode(BATTERY_PIN, INPUT);
}

void loop() {
    
    // Checar se há mensagem de update nova
    receiveData(&car);
    nav.update(car);

    //A cada x milisegundos, definido pelo timer, envie uma mensagem para os clientes
    if(webSocketTimer.CheckTime()){
        char data[1400]; // Cria um buffer de caracteres
        car.battery_voltage = (analogRead(BATTERY_PIN)/73.5f) * FLOAT_MULTIPLIER; // Atualiza o valor da bateria
        size_t len = serializeJson(carData(), data); // Usa o buffer para escrever o JSON
        ws.textAll(data, len);  // Envia esse buffer no WS para todos os clientes
        Serial.print("F: ");
        Serial.print(car.ultrassound_reading_front);
        Serial.print(" FL: ");
        Serial.print(car.ultrassound_reading_front_left);
        Serial.print(" FR: ");
        Serial.print(car.ultrassound_reading_front_right);
        Serial.print(" L: ");
        Serial.print(car.ultrassound_reading_left);
        Serial.print(" R: ");
        Serial.print(car.ultrassound_reading_right);
        Serial.print(" BL: ");
        Serial.print(car.ultrassound_reading_back_left);
        Serial.print(" BR: ");
        Serial.print(car.ultrassound_reading_back_right);
        Serial.print(" B: ");
        Serial.println(car.ultrassound_reading_back);
    }
    if(car.battery_voltage < BATTERY_PROGRAMMING_VOLTAGE){
        batteryAlert = false;
    }else if(car.battery_voltage < BATTERY_ALERT_VOLTAGE  && !batteryAlert){
        sendCommand(COMMAND_BUZZER_SETSTATE, BUZZER_BATTERY_ALERT);
        batteryAlert = true;
    }
    
    if(car.battery_voltage > BATTERY_ALERT_VOLTAGE && batteryAlert){
        sendCommand(COMMAND_BUZZER_SETSTATE, BUZZER_NONE);
        batteryAlert = false;
    }

}

void TaskSendData(void * pvParameters){
    for(;;){
        if(mapDataTimer.CheckTime()){
            char data[4000]; // Cria um buffer de caracteres
            size_t len = serializeJson(nav.getChunkData(chunkCounter), data); // Escreve o JSON
            ws.textAll(data, len);  // Manda os dados de mapa      
            chunkCounter = ++chunkCounter % CHUNK_PARTS;    // Soma 1 pro chunkCounter, e reseta se passar de chunk_parts
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
