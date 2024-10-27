#pragma once

#include "Arduino.h"
#include "defines.h"

/*---------------------------------------------------------------------------------------+
|    Chunk Handler - Marco Aurélio (22/10/2024)                                          |
|                                                                                        |
|    Arquivo responsável por tomar conta do cache e manipulação de arquivos de chunk     |
|    Desde Leitura e Escrita até a criação de novos agrupamentos de posições             |
|    E transformação de vetor global para vetor de chunk                                 |
+---------------------------------------------------------------------------------------*/

// Criação de chunk
// Leitura de chunk
// Escrita de chunk
// Transformação de Coordenadas

// Variavel com os dados de um chunk


// Transforma um par de coordenadas (x,y),em cm, em uma posição dentro de todo o array de coordenadas possíveis,
// Levando em consideração o tamanho de uma unidade de medida
uint32_t coordinateToIndex(int16_t x, int16_t y){
    
    // Transforma o x e y em seus gemeos posicionais, divivindo pela precisão
    // Ex: Precisao = 10 Coordenadas (10,-21) => (1, -2);
    int16_t x_p = x / NAVIGATION_UNIT_PRECISION;
    int16_t y_p = y / NAVIGATION_UNIT_PRECISION;

    // Calcula quantas unidades de navegação há em 1 linha do espaço navegável
    uint32_t side = NAVIGATION_CHUNK_LENGTH;
    
    // Calcula o endereço do centro, que é referencia em ambos os espaços
    uint32_t center_index = side*side/2;

    // Mover pra esquerda (-X) = pos - 1
    // Mover para direita (+X) = pos + 1
    // Mover para frente  (+Y) = pos - 1 * (largura_total)
    // Mover para tras    (-Y) = pos + 1 * (largura_total)

    // Calcula endereço dentro das coordenadas de navegação
    uint32_t index = center_index + x_p - y_p * side;
    return index;
}

// Transforma uma coordenada (x, y) em um endereço de um chunk
uint32_t coordinateToChunkIndex(int16_t x, int16_t y){
    
    // Transforma o x e y em seus gemeos posicionais, divivindo pela precisão
    // Ex: Precisao = 10 Coordenadas (10,-21) => (1, -2);
    int16_t x_p = x / NAVIGATION_UNIT_PRECISION;
    int16_t y_p = y / NAVIGATION_UNIT_PRECISION;
    
    // Calcula o endereço do centro, que é referencia em ambos os espaços
    uint32_t center_index = NAVIGATION_CHUNK_LENGTH*NAVIGATION_CHUNK_LENGTH/2;

    // Mover pra esquerda (-X) = pos - 1
    // Mover para direita (+X) = pos + 1
    // Mover para frente  (+Y) = pos - 1 * (largura_total)
    // Mover para tras    (-Y) = pos + 1 * (largura_total)

    // Calcula endereço dentro das coordenadas de navegação
    uint32_t index = center_index + x_p - y_p * NAVIGATION_CHUNK_LENGTH;
    return index;
}

// Recebe um endereço de posição e calcula a qual chunk este endereço pertence
uint16_t indexToChunk(uint32_t index){
    uint16_t x_pos = (index % (NAVIGATION_CHUNK_LENGTH * NAVIGATION_SIDE_CHUNKS))/ NAVIGATION_CHUNK_LENGTH;
    uint16_t y_pos = index / (NAVIGATION_CHUNK_LENGTH * NAVIGATION_SIDE_CHUNKS);
    uint16_t chunk_index = x_pos + y_pos * NAVIGATION_SIDE_CHUNKS;
    return chunk_index;
}

// Recebe um endereço de posição, e returna o endereço local do chunk que este endereço pertence
uint32_t indexToChunkIndex(uint32_t index){
    uint16_t x_pos = index % NAVIGATION_CHUNK_LENGTH;
    uint16_t y_pos = (index / NAVIGATION_CHUNK_LENGTH) % NAVIGATION_CHUNK_LENGTH;
    // TODO: TERMINAR ESSA FUNÇÃO
    return index;
}