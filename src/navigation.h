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
    uint8_t chunkData[NAVIGATION_CHUNK_LENGTH * NAVIGATION_CHUNK_LENGTH];
    uint8_t chunkWeights[NAVIGATION_CHUNK_LENGTH * NAVIGATION_CHUNK_LENGTH];

    public:
    Navigation(){}

    void update(CarData data){
        for(int i = 0; i < 8; i++){
            int16_t x;
            int16_t y;
            getCoordinatesFromUltrassound(i, data, &x, &y);
            chunkData[coordinateToIndex(x,y)] = 1;
        }
    }
    
    void getCoordinatesFromUltrassound(uint8_t ultrassound_index, CarData data, int16_t* x, int16_t* y){
        float distance;
        switch(ultrassound_index){
            case 0:
                distance = data.ultrassound_reading_front;
                break;
            case 1:
                distance = data.ultrassound_reading_front_left;
                break;
            case 2:
                distance = data.ultrassound_reading_front_right;
                break;
            case 3:
                distance = data.ultrassound_reading_left;
                break;
            case 4:
                distance = data.ultrassound_reading_right;
                break;
            case 5:
                distance = data.ultrassound_reading_back;
                break;
            case 6:
                distance = data.ultrassound_reading_back_left;
                break;
            case 7:
                distance = data.ultrassound_reading_back_right;
                break;
            default:
                distance = data.ultrassound_reading_front;
                break;
        };   
        *x = data.navigation_position_x + sin(data.navigation_position_heading) * distance;
        *y = data.navigation_position_x + cos(data.navigation_position_heading) * distance;
    }

    JsonDocument getChunkData(uint8_t part){
        JsonDocument output;
        output["type"] = "chunkData";
        output["size"] = CHUNK_PARTS;
        output["index"] = part;
        uint8_t data[NAVIGATION_CHUNK_LENGTH * NAVIGATION_CHUNK_LENGTH / CHUNK_PARTS];
        for(int i = 0; i < NAVIGATION_CHUNK_LENGTH * NAVIGATION_CHUNK_LENGTH / CHUNK_PARTS; i++){
            output["chunkData"][i] = chunkData[i + NAVIGATION_CHUNK_LENGTH * NAVIGATION_CHUNK_LENGTH * part/ CHUNK_PARTS];
        }
        return output;
    }
    


};

