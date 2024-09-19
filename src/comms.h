#ifndef COMMS_H
#define COMMS_H
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
    uint8_t battery_percentage;
    int32_t ultrassound_reading_front;          // Float
    int32_t ultrassound_reading_front_left;     // Float
    int32_t ultrassound_reading_front_right;    // Float    
    int32_t ultrassound_reading_left;           // Float
    int32_t ultrassound_reading_right;          // Float    
    int32_t ultrassound_reading_back;           // Float
    int32_t ultrassound_reading_back_left;      // Float    
    int32_t ultrassound_reading_back_right;     // Float    
    uint8_t motor_mode;
    int32_t motor_left_setpoint;  // Float
    int32_t motor_left_speed;     // Float
    uint8_t motor_left_throttle;
    int32_t motor_left_kp;          // Float
    int32_t motor_left_ki;          // Float
    int32_t motor_left_kd;          // Float
    int32_t motor_right_setpoint;   // Float        
    int32_t motor_right_speed;      // Float    
    uint8_t motor_right_throttle;
    int32_t motor_right_kp;  // Float
    int32_t motor_right_ki;  // Float
    int32_t motor_right_kd;  // Float
    int16_t gyro_raw_x;
    int16_t gyro_raw_y;
    int16_t gyro_raw_z;
    int16_t gyro_treated_x;
    int16_t gyro_treated_y;
    int16_t gyro_treated_z;
    int16_t acc_raw_x;
    int16_t acc_raw_y;
    int16_t acc_raw_z;
    int16_t acc_treated_x;
    int16_t acc_treated_y;
    int16_t acc_treated_z;
    int16_t mag_raw_x;
    int16_t mag_raw_y;
    int16_t mag_raw_z;
    int16_t mag_treated_x;
    int16_t mag_treated_y;
    int16_t mag_treated_z;
    uint8_t navigation_mode;
    int16_t navigation_position_x;
    int16_t navigation_position_y;
    int16_t navigation_position_z;
    int32_t navigation_position_pitch;   // Float
    int32_t navigation_position_roll;    // Float
    int32_t navigation_position_heading; // Float
    int16_t navigation_destination_x;
    int16_t navigation_destination_y;
    int16_t navigation_destination_z;
    int16_t navigation_home_x;
    int16_t navigation_home_y;
    int16_t navigation_home_z;
};

// Função de receber dados do Serial
// Incluir um pointer para a variavel que vai ser alterada com a leitura nova
void receiveData(CarData* data){
    // Ler a variavel caso tenha sido enviada algum dado no serial
    // E escrever esses dados no Struct CarData
    byte packetStatus = StreamSend::receiveObject(Serial2, data, sizeof(*data));
}

#endif