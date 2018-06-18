//
//  main.c
//  aeds2-tp2
//
//  Created by  Bruno Marra (3029), Gustavo Viegas (3026) e Heitor Passeado (3055) on 10/04/18.
//  Copyright © 2018 UFV Florestal. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "file.h"

#define FILE_NAME 20

int main()
{
    char file[FILE_NAME];
    printf("Digite o nome do arquivo: ");
    scanf("%s", file);

    TipoPagina *Arvore;
    Inicializa(&Arvore);

    CarregaDados(file, Arvore);
    return 0;
}