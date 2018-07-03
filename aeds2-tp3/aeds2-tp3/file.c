//
//  file.c
//  aeds2-tp2
//
//  Created by  Bruno Marra (3029), Gustavo Viegas (3026) e Heitor Passeado (3055) on 10/04/18.
//  Copyright © 2018 UFV Florestal. All rights reserved.
//

#include "file.h"

/** 
 * @note   Carrega os dados a partir de um arquivo
 * @param  *fileName: nome do arquivo lido
 * @param  *arvore: nó raiz da árvore
 * @retval 1
 */
int CarregaDados(char *fileName, TipoApontador *arvore, char* msg) {
    int numero = 0;
    int comparisons = 0;
    FILE *loadedFile = NULL; // Arquivo TXT lido
    loadedFile = fopen(fileName, "r");
    TipoRegistro registro;

    if (loadedFile == NULL) {
        sprintf(msg, "O arquivo %s nao existe ou nao pode ser lido corretamente.", fileName);
        return 0;
    }

    while (1) {
        fscanf(loadedFile, "%d", &numero);
        if (fgetc(loadedFile) == EOF)
            break;
        
        registro.Chave = numero;
        if (!Insere(registro, arvore, &comparisons, msg)) {
            sprintf(msg, "Erro ao inserir o registro %d", numero);
            return 0;
        }
    }

    fclose(loadedFile);
    sprintf(msg, "Arquivo carregado com sucesso!");
    return 1;
}
