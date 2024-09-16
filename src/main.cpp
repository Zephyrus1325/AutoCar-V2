#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <webPage.h>
#include <requests.h>
AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "MarcoFilho";
const char* password = "MarcoFilho12";

int mode = 0;
int throttleLeft = 0;
int throttleRight = 0;
float speedLeft = 0;
float speedRight = 0;
float ultrassound[8] = {0,0,0,0,0,0,0,0};
float motor[4] = {0,0,0,0};

void notFound(AsyncWebServerRequest *request) {
request->send(404, "text/plain", "Not found");
}

void setup() {
Serial.begin(115200);
WiFi.mode(WIFI_AP);
WiFi.begin(ssid, password);
if (WiFi.waitForConnectResult() != WL_CONNECTED) {
Serial.println("WiFi Failed!");
return;
}
Serial.println();
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", index_html);
});

// Enviar dados de Status do Motor para Cliente
server.on("/getMotorMode", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "application/json", HTMLMotorMode(mode));
});

// Enviar dados de Potencia do Motor para Cliente
server.on("/getMotorConfigs", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "application/json", HTMLMotorReadings(motor));
});

// Enviar leitura dos ultrassons para Cliente
server.on("/getUltrassoundReadings", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "application/json", HTMLUltrassoundReadings(ultrassound));
});



// Receber comando de mudança de Modo
server.on("/setMotorMode", HTTP_GET, [](AsyncWebServerRequest *request){
  // Checar se há parametro correto
  if (request->hasParam("mode"))
  {
    // Caso positivo, alterar o modo para o parametro recebido
    mode =  String(request->getParam("mode")->value()).toInt();
  }
  // Caso contrário, faça nada
});

// Receber comando de mudança de Potencia de Motor
server.on("/setMotorThrottle", HTTP_GET, [](AsyncWebServerRequest *request){
  // Checar se há todos parametros corretos
  if (request->hasParam("left") && request->hasParam("right") )
  {
    // Caso positivo, alterar as potencias para os parametros recebidos
     motor[2] =  String(request->getParam("left")->value()).toInt();
     motor[3] =  String(request->getParam("right")->value()).toInt();
  }
  // Caso contrário, faça nada
});

// Receber comando de mudança de Potencia de Motor
server.on("/setMotorSpeed", HTTP_GET, [](AsyncWebServerRequest *request){
  // Checar se há todos parametros corretos
  if (request->hasParam("left") && request->hasParam("right") )
  {
    // Caso positivo, alterar as potencias para os parametros recebidos
    motor[0] =  String(request->getParam("left")->value()).toFloat();
    motor[1] =  String(request->getParam("right")->value()).toFloat();
  }
  // Caso contrário, faça nada
});

server.onNotFound(notFound);
server.begin();
}

void loop() {
  delay(250);
  for(int i = 0; i < 8; i++){
    ultrassound[i] = (float) (random(1500) / 100);
  }
  Serial.println("mode: " + String(mode) + " LS: " + String(motor[0]) + " RS: " + String(motor[1]) + " LT: " + String(motor[2]) + " RT: " +String(motor[3]) );
}