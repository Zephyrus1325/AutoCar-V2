#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <webPage.h>
#include <ArduinoJson.h>
#include "esp_wpa2.h"
#include "comms.h"

AsyncWebServer server(80); // Declara um servidor e um websocket
AsyncWebSocket ws("/ws");

// Timers
timer webSocketTimer{0, 100, true, true, true};

// Credenciais de Rede 
const char* ssid = "MarcoFilho";
const char* password = "MarcoFilho12";

// Variáveis do carrinho, estão de exemplo aqui
CarData car;
int mode = 0;
int throttleLeft = 0;
int throttleRight = 0;
float speedLeft = 0;
float speedRight = 0;
float ultrassound[8] = {0,0,0,0,0,0,0,0};
float motor[4] = {0,0,0,0};


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

        // Muda as variaveis de acordo com o que precisa ser alterado
        // TODO: Adicionar comunicação Serial com o Arduino ao inves de variaveis globais do ESP
        if (strcmp(toChange, "motorMode") == 0){
            mode = (value == "manual" ? 0 : 1);
            Serial.println("motorMode:" + value);
        } else if (strcmp(toChange, "leftSetpoint") == 0){
            speedLeft = value.toFloat();
            Serial.println("leftSetpoint" + value);
        } else if (strcmp(toChange, "leftThrottle") == 0){
            throttleLeft = value.toFloat();
            Serial.println("leftThrottle" + value);
        } else if (strcmp(toChange, "leftKp") == 0){

        } else if (strcmp(toChange, "leftKi") == 0){

        } else if (strcmp(toChange, "leftKd") == 0){

        } else if (strcmp(toChange, "rightSetpoint") == 0){
            speedRight = value.toFloat();
            Serial.println("rightSetpoint" + value);
        } else if (strcmp(toChange, "rightThrottle") == 0){
            throttleRight = value.toFloat();
            Serial.println("rightThrottle" + value);
        } else if (strcmp(toChange, "rightKp") == 0){

        } else if (strcmp(toChange, "rightKi") == 0){

        } else if (strcmp(toChange, "rightKd") == 0){

        } else if (strcmp(toChange, "navigationMode") == 0){

        } else if (strcmp(toChange, "destionation") == 0){

        } else if (strcmp(toChange, "home") == 0){

        } else if (strcmp(toChange, "updateFrequency") == 0) {

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
    data["battery"]["voltage"] = 11.1f;
    data["battery"]["percentage"] = 100;
    data["ultrassound"]["front"] = (float)random(1000)/100.0f;
    data["ultrassound"]["front_left"] = (float)random(1000)/100.0f;
    data["ultrassound"]["front_right"] = (float)random(1000)/100.0f;
    data["ultrassound"]["left"] = (float)random(1000)/100.0f;
    data["ultrassound"]["right"] = (float)random(1000)/100.0f;
    data["ultrassound"]["back"] = (float)random(1000)/100.0f;
    data["ultrassound"]["back_left"] = (float)random(1000)/100.0f;
    data["ultrassound"]["back_right"] = (float)random(1000)/100.0f;
    data["motor"]["mode"] = mode == 0 ? "auto" : "manual";
    data["motor"]["left"]["setpoint"] = 16.0f;
    data["motor"]["left"]["speed"] = speedLeft;
    data["motor"]["left"]["throttle"] = throttleLeft;
    data["motor"]["left"]["kp"] = 1.6f;
    data["motor"]["left"]["ki"] = 1.7f;
    data["motor"]["left"]["kd"] = 1.8f;
    data["motor"]["right"]["setpoint"] = 16.0f;
    data["motor"]["right"]["speed"] = speedRight;
    data["motor"]["right"]["throttle"] = throttleRight;
    data["motor"]["right"]["kp"] = 1.6f;
    data["motor"]["right"]["ki"] = 1.7f;
    data["motor"]["right"]["kd"] = 1.8f;
    data["IMU"]["gyro"]["raw"]["x"] = 10;
    data["IMU"]["gyro"]["raw"]["y"] = 10;
    data["IMU"]["gyro"]["raw"]["z"] = 10;
    data["IMU"]["gyro"]["treated"]["x"] = 10;
    data["IMU"]["gyro"]["treated"]["y"] = 10;
    data["IMU"]["gyro"]["treated"]["z"] = 10;
    data["IMU"]["acc"]["raw"]["x"] = 10;
    data["IMU"]["acc"]["raw"]["y"] = 10;
    data["IMU"]["acc"]["raw"]["z"] = 10;
    data["IMU"]["acc"]["treated"]["x"] = 10;
    data["IMU"]["acc"]["treated"]["y"] = 10;
    data["IMU"]["acc"]["treated"]["z"] = 10;
    data["IMU"]["mag"]["raw"]["x"] = 10;
    data["IMU"]["mag"]["raw"]["y"] = 10;
    data["IMU"]["mag"]["raw"]["z"] = 10;
    data["IMU"]["mag"]["treated"]["x"] = 10;
    data["IMU"]["mag"]["treated"]["y"] = 10;
    data["IMU"]["mag"]["treated"]["z"] = 10;
    data["navigation"]["mode"] = "manual";
    data["navigation"]["position"]["x"] = 0;
    data["navigation"]["position"]["y"] = 0;
    data["navigation"]["position"]["z"] = 0;
    data["navigation"]["position"]["pitch"] = 0;
    data["navigation"]["position"]["roll"] = 0;
    data["navigation"]["position"]["heading"] = 0;
    data["navigation"]["destination"]["x"] = 0;
    data["navigation"]["destination"]["y"] = 0;
    data["navigation"]["destination"]["z"] = 0;
    data["navigation"]["home"]["x"] = 0;
    data["navigation"]["home"]["y"] = 0;
    data["navigation"]["home"]["z"] = 0;
    return data;
}

void setup() {
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    WiFi.mode(WIFI_STA);    // Inicia o ESP como um STATION (cliente de uma rede)
    WiFi.begin(ssid, password); // Inicia o WIFI com as credenciais de rede
    // Se a conexão falhar, desista!
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed!");
        return;
    }
    WiFi.setSleep(false); // Desativar modo de economia de energia com o Wifi (adeus latencia!)
    Serial.println();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); // Printa o IP no serial para saber onde caralhos estou

    ws.onEvent(onEvent);    // Define qual função deve rodar em um evento do WS
    server.addHandler(&ws); // Define qual é o handler de WS do servidor

    // Inicia o servidor em si
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    server.onNotFound(notFound);
    server.begin();
}
CarData t;
void loop() {
    // Checar se há mensagem de update nova
    receiveData(&t);
    // A cada x milisegundos, definido pelo timer, envie uma mensagem para os clientes
    if(webSocketTimer.CheckTime()){
        char data[1400]; // Cria um buffer de caracteres
        size_t len = serializeJson(carData(), data); // Usa o buffer para escrever o JSON
        ws.textAll(data, len);  // Envia esse buffer no WS para todos os clientes
    }
    
}