#pragma once

#include "Arduino.h"
#include "defines.h"


/*--------------------------------------------------------------------------------------+
|   Arquivo responsável pela comunicação com cartão SD - Marco Aurélio (05/10/2024)     |
|                                                                                       |
|   Arquivo com todas as funções relacionadas a ler e escrever dados do cartão SD       |
|   Usado principalmente com o sistema de mapeamento, mas pode ser usado também com     |
|   Outros sistemas que necessitem de armazenamento de dados a longo prazo              |
+--------------------------------------------------------------------------------------*/

// Interface básica de leitura e escrita de cartão SD
// Criação de arquivos
// Leitura de chunks