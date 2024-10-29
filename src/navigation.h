#pragma once

#include "Arduino.h"
#include "comms.h"
#include "chunkHandler.h"
#include <ArduinoJson.h>
/*--------------------------------------------------------------------------------------+
|   Classe Responsável pela navegação - Marco Aurélio (05/10/2024)                      |
|                                                                                       |
|   Classe responsável por receber dados de sensores e a posição atual do veiculo e     |
|   alterar os valores dos chunks, assim gerando o mapa, além de calcular o conjunto    |
|   de pontos a serem seguidos para alcançar o destino, comandando o Arduino para tal   |
+--------------------------------------------------------------------------------------*/

// Criar classe de navegação aqui

// Usar A*?
// Transformada de distancia Euclidiana (Euclidean distance Transform)
class Navigation {
    private:

    uint16_t chunkSize = NAVIGATION_CHUNK_LENGTH; //
    uint8_t chunkData[NAVIGATION_CHUNK_LENGTH][NAVIGATION_CHUNK_LENGTH];
    uint8_t chunkWeights[NAVIGATION_CHUNK_LENGTH * NAVIGATION_CHUNK_LENGTH];

    public:
    Navigation(){}

    void update(CarData data){
        for(int i = 0; i < 8; i++){
            uint16_t distance = getUltrassoundDistance(i, data) / NAVIGATION_UNIT_PRECISION;
            float heading = data.navigation_position_heading / FLOAT_MULTIPLIER;
            if(distance > 0){
                int16_t x = (data.navigation_position_x / NAVIGATION_UNIT_PRECISION + sin(heading + (i * PI / 4.f)) * distance);
                int16_t y = (data.navigation_position_y / NAVIGATION_UNIT_PRECISION + cos(heading + (i * PI / 4.f)) * distance);
                for(int j = 0; j < distance; j++){
                    int16_t x_del = (data.navigation_position_x / NAVIGATION_UNIT_PRECISION + sin(heading + (i * PI / 4.f)) * j);
                    int16_t y_del = (data.navigation_position_y / NAVIGATION_UNIT_PRECISION + cos(heading + (i * PI / 4.f)) * j);
                    chunkData[x_del + NAVIGATION_CHUNK_LENGTH/2][y_del + 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + NAVIGATION_CHUNK_LENGTH/2][y_del + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + NAVIGATION_CHUNK_LENGTH/2][y_del - 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + 1 + NAVIGATION_CHUNK_LENGTH/2][y_del - 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del - 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del - 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del - 1 + NAVIGATION_CHUNK_LENGTH/2][y_del - 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    
                }
                chunkData[x + NAVIGATION_CHUNK_LENGTH/2][y + NAVIGATION_CHUNK_LENGTH/2] = 1;
                //Serial.print("(");
                //Serial.print(x);
                //Serial.print(",");
                //Serial.print(y);
                //Serial.print(")|");

            } else {
                for(int j = 0; j < 100 / NAVIGATION_UNIT_PRECISION; j++){
                    int16_t x_del = (data.navigation_position_x / NAVIGATION_UNIT_PRECISION + sin(heading + (i * PI / 4.f)) * j);
                    int16_t y_del = (data.navigation_position_y / NAVIGATION_UNIT_PRECISION + cos(heading + (i * PI / 4.f)) * j);
                    chunkData[x_del + NAVIGATION_CHUNK_LENGTH/2][y_del + 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + NAVIGATION_CHUNK_LENGTH/2][y_del + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + NAVIGATION_CHUNK_LENGTH/2][y_del - 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del + 1 + NAVIGATION_CHUNK_LENGTH/2][y_del - 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del - 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del - 1 + NAVIGATION_CHUNK_LENGTH/2][y_del + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    //chunkData[x_del - 1 + NAVIGATION_CHUNK_LENGTH/2][y_del - 1 + NAVIGATION_CHUNK_LENGTH/2] = 0;
                    
                }
            }
        }
        //Serial.println("");
    }
    
    uint16_t getUltrassoundDistance(uint8_t ultrassound_index, CarData data){
        uint16_t distance;
        switch(ultrassound_index){
            case 0:
                distance = data.ultrassound_reading_front;
                break;
            case 1:
                distance = data.ultrassound_reading_front_right;
                break;
            case 2:
                distance = data.ultrassound_reading_right;
                break;
            case 3:
                distance = data.ultrassound_reading_back_right;
                break;
            case 4:
                distance = data.ultrassound_reading_back;
                break;
            case 5:
                distance = data.ultrassound_reading_back_left;
                break;
            case 6:
                distance = data.ultrassound_reading_left;
                break;
            case 7:
                distance = data.ultrassound_reading_front_right;
                break;
            default:
                distance = data.ultrassound_reading_front;
                break;
        };  
        return distance;
        
        
    }

    JsonDocument getChunkData(uint8_t part){
        JsonDocument output;
        output["type"] = "chunkData";
        output["size"] = NAVIGATION_CHUNK_LENGTH;
        output["total_parts"] = CHUNK_PARTS;
        output["part"] = part;
        for(int i = 0; i < NAVIGATION_CHUNK_LENGTH * NAVIGATION_CHUNK_LENGTH / CHUNK_PARTS; i++){
            output["chunkData"][i] = chunkData[(part * NAVIGATION_CHUNK_LENGTH / CHUNK_PARTS) + (i / NAVIGATION_CHUNK_LENGTH)][i % NAVIGATION_CHUNK_LENGTH];
            //if(chunkData[(part * NAVIGATION_CHUNK_LENGTH / CHUNK_PARTS) + (i / NAVIGATION_CHUNK_LENGTH)][i % NAVIGATION_CHUNK_LENGTH]){
            //    Serial.print("(");
            //    Serial.print((part * NAVIGATION_CHUNK_LENGTH * CHUNK_PARTS) + (i / NAVIGATION_CHUNK_LENGTH));
            //    Serial.print(",");
            //    Serial.print(i % NAVIGATION_CHUNK_LENGTH);
            //    Serial.print(")");
            //}
        }
        //Serial.println("");
        return output;
    }

    void clearChunkData(){
        for(int i = 0; i < NAVIGATION_CHUNK_LENGTH; i++){
            for(int j = 0; j < NAVIGATION_CHUNK_LENGTH; j++){
                chunkData[i][j] = 0;
            }
        }
    }

};

