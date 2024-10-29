#pragma once

//#define DEBUG

#include "Arduino.h"
#include "timer.h"
#include "StreamSend.h"

/*---------------------------------------------------------------------------------------+
|    Arquivo de Comunicações - Marco Aurélio (19/09/2024)                                |
|                                                                                        |
|    Arquivo responsável por fazer a recepção dos dados seriais vindos do Arduino Mega   |
|    e transformar eles de volta em um Struct legível para o ESP                         |
|                                                                                        |
+---------------------------------------------------------------------------------------*/

// Struct com todos os dados do Carro (160 bytes total)
struct CarData{
    int32_t battery_voltage;                    // Float
    int32_t battery_percentage;
    int32_t ultrassound_reading_front;          // Float
    int32_t ultrassound_reading_front_left;     // Float
    int32_t ultrassound_reading_front_right;    // Float    
    int32_t ultrassound_reading_left;           // Float
    int32_t ultrassound_reading_right;          // Float    
    int32_t ultrassound_reading_back;           // Float
    int32_t ultrassound_reading_back_left;      // Float    
    int32_t ultrassound_reading_back_right;     // Float    
    int32_t motor_left_mode;
    int32_t motor_left_setpoint;  // Float
    int32_t motor_left_speed;     // Float
    int32_t motor_left_throttle;
    int32_t motor_left_kp;          // Float
    int32_t motor_left_ki;          // Float
    int32_t motor_left_kd;          // Float
    int32_t motor_right_mode;
    int32_t motor_right_setpoint;   // Float        
    int32_t motor_right_speed;      // Float    
    int32_t motor_right_throttle;
    int32_t motor_right_kp;  // Float
    int32_t motor_right_ki;  // Float
    int32_t motor_right_kd;  // Float
    int32_t gyro_raw_x;
    int32_t gyro_raw_y;
    int32_t gyro_raw_z;
    int32_t gyro_treated_x; // Float
    int32_t gyro_treated_y; // Float
    int32_t gyro_treated_z; // Float
    int32_t acc_raw_x;
    int32_t acc_raw_y;
    int32_t acc_raw_z;
    int32_t acc_treated_x;  // Float
    int32_t acc_treated_y;  // Float
    int32_t acc_treated_z;  // Float
    int32_t mag_raw_x;
    int32_t mag_raw_y;
    int32_t mag_raw_z;
    int32_t mag_treated_x;  // Float
    int32_t mag_treated_y;  // Float
    int32_t mag_treated_z;  // Float
    int32_t raw_temperature;
    int32_t temperature; // Float
    int32_t raw_pressure;
    int32_t pressure;   // Float
    int32_t navigation_mode;
    int32_t navigation_position_x;
    int32_t navigation_position_y;
    int32_t navigation_position_z;
    int32_t navigation_position_pitch;   // Float
    int32_t navigation_position_roll;    // Float
    int32_t navigation_position_heading; // Float
    int32_t navigation_destination_x;
    int32_t navigation_destination_y;
    int32_t navigation_destination_z;
    int32_t navigation_home_x;
    int32_t navigation_home_y;
    int32_t navigation_home_z;
};

struct Command {
    int32_t index;
    int32_t value;
};

unsigned long packetResetTimer = 0;   // Timer para correção de problemas com desincronização de Seriais
unsigned long packetResetTime = 1000; // Tempo até a correção ser ativada

// Envia os dados a cada tempo, definido pelo sendTimer
void sendCommand(int32_t command, int32_t value){
    Command data{command, value};
    StreamSend::sendObject(Serial2, &data, sizeof(data));
    
     //  Debug Info
    #ifdef DEBUG
    Serial.print("Sent Packet! Size: ");
    Serial.println((int)sizeof(data));
    #endif
}

// Função de receber dados do Serial
// Incluir um pointer para a variavel que vai ser alterada com a leitura nova
void receiveData(CarData* data){
    // Ler a variavel caso tenha sido enviada algum dado no serial
    // E escrever esses dados no Struct CarData
    byte packetStatus = StreamSend::receiveObject(Serial2, data, sizeof(*data));
    if(packetStatus == GOOD_PACKET){
        packetResetTimer = millis();
    } else if(packetStatus == PACKET_NOT_FOUND){
        if(millis() - packetResetTimer > packetResetTime){
            // Clear Receive Buffer
            while(Serial2.available() > 0) {
              char t = Serial2.read();
            }
            #ifdef DEBUG
            Serial.println("Serial2 Buffer Cleaned");
            #endif
            packetResetTimer = millis();
        }
    }
    // Debugging info
    #ifdef DEBUG
    if(packetStatus == GOOD_PACKET){
        Serial.print("Received Healty Packet: ");
        Serial.println(sizeof(*data));
        
    } else if(packetStatus == BAD_PACKET){
        Serial.print("Bad Packet: ");
        Serial.print(sizeof(*data));
        Serial.print(" Struct Size: ");
        Serial.println(sizeof(CarData));
    } 
    #endif
}

#ifdef DEBUG
#undef DEBUG
#endif