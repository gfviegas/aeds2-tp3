//
//  arvoreb.h
//  aeds2-tp2
//
//  Created by  Bruno Marra (3029), Gustavo Viegas (3026) e Heitor Passeado (3055) on 10/04/18.
//  Copyright © 2018 UFV Florestal. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

#define M 512
#define MM (M * 2)

#ifndef FALSE
    #define FALSE 0
#endif
#ifndef TRUE
    #define TRUE 1
#endif

typedef long TipoChave;
typedef struct TipoRegistro
{
    TipoChave Chave;
    /*outros componentes*/
} TipoRegistro;

typedef struct TipoPagina *TipoApontador;
typedef struct TipoPagina
{
    short n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

void Inicializa(TipoApontador *Dicionario);
int Pesquisa(TipoRegistro *x, TipoApontador Ap, int *qntdComparacoes, char *msg);
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, int *qntdComparacoes);
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, int *qntdComparacoes);
int Insere(TipoRegistro Reg, TipoApontador *Ap, int *qntdComparacoes, char *msg);
void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, int PosPai, short *Diminuiu, int *qntdComparacoes);
void Antecessor(TipoApontador Ap, int Ind, TipoApontador ApPai, short *Diminuiu, int *qntdComparacoes);
void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu, int *qntdComparacoes);
int Retira(TipoChave Ch, TipoApontador *Ap, int *qntdComparacoes, char *msg);
void ImprimeI(TipoApontador p, int nivel);
void Imprime(TipoApontador p);
void TestaI(TipoApontador p, int pai, short direita);
void Testa(TipoApontador p);
void AlturaI(TipoApontador Ap, int *height);
void Altura(TipoApontador *Ap, int *qntdComparacoes, char *msg);
