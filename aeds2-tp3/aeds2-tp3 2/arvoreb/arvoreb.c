//
//  arvoreb.c
//  aeds2-tp2
//
//  Created by  Bruno Marra (3029), Gustavo Viegas (3026) e Heitor Passeado (3055) on 10/04/18.
//  Copyright © 2018 UFV Florestal. All rights reserved.
//

#include "arvoreb.h"
#include <stddef.h>

/**
 * @note   Inicializa uma nova árvore
 * @param  *Dicionario: nó inicial da árvore
 * @retval None
 */
void Inicializa(TipoApontador *Dicionario) {
    *Dicionario = NULL;
}

/**
 * @note Pesquisa um dado registro na árvore
 * @param  *x: registro buscado
 * @param  Ap: apontador do nó
 * @retval 1 se a chave foi encontrada e 0 se não foi
 */
int Pesquisa(TipoRegistro *x, TipoApontador Ap, int *qntdComparacoes, char *msg) {
    int cond;
    long i = 1;
    
    (*qntdComparacoes)++;
    if (Ap == NULL) {
        sprintf(msg, "Registro %ld nao encontrado! Arvore nula! %d comparacoes foram feitas", x->Chave, *qntdComparacoes);
        return 0;
    }

    while (i < Ap->n && x->Chave > Ap->r[i - 1].Chave){
        i++;
        (*qntdComparacoes)++;
    }
    
    (*qntdComparacoes)++;
    if (x->Chave == Ap->r[i - 1].Chave) {
        *x = Ap->r[i - 1];
        sprintf(msg, "Registro %ld encontrado! %d comparacoes foram feitas", x->Chave, *qntdComparacoes);
        return 1;
    }
    
    (*qntdComparacoes)++;
    if (x->Chave < Ap->r[i - 1].Chave)
        cond = Pesquisa(x, Ap->p[i - 1],qntdComparacoes, msg);
    else
        cond = Pesquisa(x, Ap->p[i],qntdComparacoes, msg);
    
    (*qntdComparacoes)++;
    if (cond) {
        sprintf(msg, "Registro %ld encontrado! %d comparacoes foram feitas", x->Chave, *qntdComparacoes);
        return 1;
    }
    
    sprintf(msg, "Registro %ld nao encontrado! %d comparacoes foram feitas", x->Chave, *qntdComparacoes);
    return 0;
}

/**
 * @note Insere um novo registro na página
 * @param  Ap: apontador do nó
 * @param  Reg: registro a ser inserido
 * @param  ApDir: nó da direita
 * @retval None
 */
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, int *qntdComparacoes) {
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    
    (*qntdComparacoes)++;
    while (NaoAchouPosicao) {
        (*qntdComparacoes) += 2;
        if (Reg.Chave >= Ap->r[k - 1].Chave) {
            NaoAchouPosicao = FALSE;
            break;
        }
        
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
        
        (*qntdComparacoes)++;
        if (k < 1)
            NaoAchouPosicao = FALSE;
    }
    
    Ap->r[k] = Reg;
    Ap->p[k + 1] = ApDir;
    Ap->n++;
}

/**
 * @note Função auxiliar para inserir um registro
 * @param  Reg: registro a ser inserido
 * @param  Ap: apontador do nó
 * @param  *Cresceu: booleano que define se árvore cresceu
 * @param  *RegRetorno: registro retornado
 * @param  *ApRetorno: nó a ser retornado
 * @retval None
 */
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, int *qntdComparacoes) {
    long i = 1;
    long j;
    TipoApontador ApTemp;
    
    (*qntdComparacoes)++;
    if (Ap == NULL) {
        *Cresceu = TRUE;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
         return;
    }
    
    (*qntdComparacoes)++;
    while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave){
        i++;
        (*qntdComparacoes)++;
    }
    
    
    if (Reg.Chave == Ap->r[i - 1].Chave) {
        printf(" Erro: Registro ja esta presente\n");
        *Cresceu = FALSE;
        return;
    }
    
    (*qntdComparacoes)++;
    if (Reg.Chave < Ap->r[i - 1].Chave)
        i--;
    
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, qntdComparacoes);
    
    (*qntdComparacoes)++;
    if (!*Cresceu) return;
    
    (*qntdComparacoes)++;
    if (Ap->n < MM) { // Pagina tem espaco
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno,qntdComparacoes);
        *Cresceu = FALSE;
        return;
    }
    
    // Overflow: Pagina tem que ser dividida
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    
    (*qntdComparacoes)++;
    if (i < M + 1) {
        InsereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM], qntdComparacoes);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, qntdComparacoes);
    } else {
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno, qntdComparacoes);
    }
    
    (*qntdComparacoes)++;
    for (j = M + 2; j <= MM; j++){
        (*qntdComparacoes)++;
        InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j], qntdComparacoes);
    }
    
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M + 1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
}

/**
 * @note Insere um dado registro na árvore
 * @param  Reg: registro a ser inserido
 * @param  *Ap: nó pai da árvore
 * @retval 1 se inseriu e 0 se já havia o registro na árvore
 */
int Insere(TipoRegistro Reg, TipoApontador *Ap, int *qntdComparacoes, char *msg){
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    (*qntdComparacoes)++;
    if (Pesquisa(&Reg, *Ap, qntdComparacoes, msg)) {
        sprintf(msg, "Registro %ld ja esta na arvore! %d comparacoes foram feitas", Reg.Chave, *qntdComparacoes);
        return 0;// Registro já está na arvore
    } else {
        Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno, qntdComparacoes);
        
        (*qntdComparacoes)++;
        if (Cresceu) { // Arvore cresce na altura pela raiz
            ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
            ApTemp->n = 1;
            ApTemp->r[0] = RegRetorno;
            ApTemp->p[1] = ApRetorno;
            ApTemp->p[0] = *Ap;
            *Ap = ApTemp;
        }
        sprintf(msg, "Registro %ld inserido com sucesso! %d comparacoes foram feitas", Reg.Chave, *qntdComparacoes);
        return 1;
        }
}

/**
 * @note
 * @param  ApPag: apontador da página a ser reconstituída
 * @param  ApPai: apontador pai da página a ser reconstituída
 * @param  PosPai: a posição do pai na página pai
 * @param  *Diminuiu: booleano que define se página diminuiu ou não
 * @retval None
 */
void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, int PosPai, short *Diminuiu, int *qntdComparacoes) {
    TipoPagina *Aux;
    long DispAux, j;
    
    (*qntdComparacoes)++;
    if (PosPai < ApPai->n) { // Aux = TipoPagina a direita de ApPag
        Aux = ApPai->p[PosPai + 1];
        DispAux = (Aux->n - M + 1) / 2;
        ApPag->r[ApPag->n] = ApPai->r[PosPai];
        ApPag->p[ApPag->n + 1] = Aux->p[0];
        ApPag->n++;
        
        (*qntdComparacoes)++;
        if (DispAux > 0) { // Existe folga: transfere de Aux para ApPag
            (*qntdComparacoes)++;
            for (j = 1; j < DispAux; j++){
                InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j], qntdComparacoes);
                (*qntdComparacoes)++;
            }
            
            ApPai->r[PosPai] = Aux->r[DispAux - 1];
            Aux->n -= DispAux;

            for (j = 0; j < Aux->n; j++){
                (*qntdComparacoes)++;
                Aux->r[j] = Aux->r[j + DispAux];
            }
            
            for (j = 0; j <= Aux->n; j++){
                (*qntdComparacoes)++;
                Aux->p[j] = Aux->p[j + DispAux];
            }

            *Diminuiu = FALSE;
        } else {
            // Fusao: intercala Aux em ApPag e libera Aux */
            for (j = 1; j <= M; j++){
                (*qntdComparacoes)++;
                InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j], qntdComparacoes);
            }
            free(Aux);
            for (j = PosPai + 1; j < ApPai->n; j++) {
                ApPai->r[j - 1] = ApPai->r[j];
                ApPai->p[j] = ApPai->p[j + 1];
                (*qntdComparacoes)++;
            }
            ApPai->n--;
            (*qntdComparacoes)++;
            if (ApPai->n >= M)
                *Diminuiu = FALSE;
        }
    } else {
        // Aux = TipoPagina a esquerda de ApPag
        Aux = ApPai->p[PosPai - 1];
        DispAux = (Aux->n - M + 1) / 2;
        for (j = ApPag->n; j >= 1; j--){
            ApPag->r[j] = ApPag->r[j - 1];
            (*qntdComparacoes)++;
        }
        ApPag->r[0] = ApPai->r[PosPai - 1];
        for (j = ApPag->n; j >= 0; j--){
            ApPag->p[j + 1] = ApPag->p[j];
            (*qntdComparacoes)++;
        }
        ApPag->n++;
        (*qntdComparacoes)++;
        if (DispAux > 0) { // Existe folga: transf. de Aux para ApPag */
            for (j = 1; j < DispAux; j++){
                InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1], qntdComparacoes);
                (*qntdComparacoes)++;
            }
            ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
            ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
            Aux->n -= DispAux;
            *Diminuiu = FALSE;
        } else {
            // Fusao: intercala ApPag em Aux e libera ApPag */
            for (j = 1; j <= M; j++){
                InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j],qntdComparacoes);
                (*qntdComparacoes)++;
            }

            free(ApPag);
            ApPai->n--;

            (*qntdComparacoes)++;
            if (ApPai->n >= M)
                *Diminuiu = FALSE;
        }
    }
}

/**
 * @note   Busca qual é o registro antecessor para reconstituição
 * @param  Ap: apontador do nó
 * @param  Ind: indice do nó
 * @param  ApPai: apontador pai do nó
 * @param  *Diminuiu: booleano que define se página diminuiu ou não
 * @retval None
 */
void Antecessor(TipoApontador Ap, int Ind, TipoApontador ApPai, short *Diminuiu, int *qntdComparacoes) {
    (*qntdComparacoes)++;
    if (ApPai->p[ApPai->n] != NULL) {
        Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu, qntdComparacoes);
        (*qntdComparacoes)++;
        if (*Diminuiu)
            Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu, qntdComparacoes);

        return;
    }
    Ap->r[Ind - 1] = ApPai->r[ApPai->n - 1];
    ApPai->n--;
    *Diminuiu = (ApPai->n < M);
}

/**
 * @note   Função auxiliar para retirada de um registro da árvore
 * @param  Ch: chave a ser retirada
 * @param  *Ap: ponteiro para o nó
 * @param  *Diminuiu: booleano que define se página diminuiu ou não
 * @retval None
 */
void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu, int *qntdComparacoes) {
    long j, Ind = 1;
    TipoApontador Pag;
    
    (*qntdComparacoes)++;
    if (*Ap == NULL) {
        printf("Erro: registro nao esta na arvore\n");
        *Diminuiu = FALSE;
        return;
    }
    
    Pag = *Ap;
    
    (*qntdComparacoes)++;
    while (Ind < Pag->n && Ch > Pag->r[Ind - 1].Chave) {
        Ind++;
        (*qntdComparacoes)++;
    }
    
    if (Ch == Pag->r[Ind - 1].Chave) {
        
        (*qntdComparacoes)++;
        if (Pag->p[Ind - 1] == NULL) {  /* TipoPagina folha */
            Pag->n--;
            *Diminuiu = (Pag->n < M);
            
            (*qntdComparacoes)++;
            for (j = Ind; j <= Pag->n; j++) {
                Pag->r[j - 1] = Pag->r[j];
                Pag->p[j] = Pag->p[j + 1];
                (*qntdComparacoes)++;
            }
            
            (*qntdComparacoes)--;
            return;
        }
        /* TipoPagina nao e folha: trocar com antecessor */
        Antecessor(*Ap, Ind, Pag->p[Ind - 1], Diminuiu, qntdComparacoes);
        (*qntdComparacoes)++;
        if (*Diminuiu) {
            Reconstitui(Pag->p[Ind - 1], *Ap, Ind - 1, Diminuiu, qntdComparacoes);
        }
        
        return;
    }

    (*qntdComparacoes)++;
    if (Ch > Pag->r[Ind - 1].Chave){
        Ind++;
    }

    Ret(Ch, &Pag->p[Ind - 1], Diminuiu, qntdComparacoes);
    
    (*qntdComparacoes)++;
    if (*Diminuiu) {
        Reconstitui(Pag->p[Ind - 1], *Ap, Ind - 1, Diminuiu, qntdComparacoes);
    }
}

/**
 * @note   Retira um registro da árvore a partir da raiz
 * @param  Ch: chave a ser retirada
 * @param  *Ap: nó raiz da árvore
 * @retval 1 se a chave foi retirada e 0 se não havia a chave na árvore
 */
int Retira(TipoChave Ch, TipoApontador *Ap, int *qntdComparacoes, char *msg) {
    TipoRegistro x;
    x.Chave = Ch;
    
    (*qntdComparacoes)++;
    if (Pesquisa(&x, *Ap, qntdComparacoes, msg)) {
        short Diminuiu;
        TipoApontador Aux;
        Ret(Ch, Ap, &Diminuiu, qntdComparacoes);
        
        (*qntdComparacoes)++;
        if (Diminuiu && (*Ap)->n == 0) {  // Arvore diminui na altura
            Aux = *Ap;
            *Ap = Aux->p[0];
            free(Aux);
        }
        
        sprintf(msg,"Registro %ld retirado com sucesso! %d comparacoes foram feitas", Ch, *qntdComparacoes);
        return 1;
    }
    sprintf(msg, "Registro %ld nao foi encontrado na arvore! %d comparacoes foram feitas", Ch, *qntdComparacoes);
    return 0; //Não existe tal chave na arvore
}

/**
 * @note   Função auxiliar ao imprimir a árvore
 * @param  p: nó a ser impresso
 * @param  nivel: nível onde ele se encontra
 * @retval None
 */
void ImprimeI(TipoApontador p, int nivel) {
    long i;
    if (p == NULL)
        return;
    printf("Nivel %d : ", nivel);
    for (i = 0; i < p->n; i++)
        printf("%ld ", (long)p->r[i].Chave);
    putchar('\n');
    nivel++;
    for (i = 0; i <= p->n; i++)
        ImprimeI(p->p[i], nivel);
}

/**
 * @note   Imprime a árvore a partir de sua raiz
 * @param  p: raiz da árvore
 * @retval None
 */
void Imprime(TipoApontador p) {
    int n = 0;
    ImprimeI(p, n);
}

/**
 * @note Função auxiliar que realiza testes para visualizar o comportamento da árvore
 * @param  p: nó atual da árvore
 * @param  pai: quem é o pai do nó
 * @param  direita: página a direita
 * @retval None
 */
void TestaI(TipoApontador p, int pai, short direita) {
    int i;
    int antecessor = 0;
    if (p == NULL)
        return;
    if (p->r[0].Chave > pai && direita == FALSE) {
        printf("Erro: filho %12ld maior que pai %d\n", p->r[0].Chave, pai);
        return;
    }
    for (i = 0; i < p->n; i++) {
        if (p->r[i].Chave <= antecessor) {
            printf("Erro: irmao %ld maior que irmao a esquerda %d\n",
                   (long)p->r[i].Chave, antecessor);
            return;
        }
        antecessor = p->r[i].Chave;
    }
    for (i = 0; i < p->n; i++)
        TestaI(p->p[i], p->r[i].Chave, FALSE);
    TestaI(p->p[p->n], p->r[i].Chave, TRUE);
}

/**
 * @note Testa o comportamento da árvore
 * @param  p: nó raiz da árvore
 * @retval None
 */
void Testa(TipoApontador p) {
    int i;
    if (p == NULL)
        return;
    for (i = 0; i < p->n; i++)
        TestaI(p->p[i], p->r[i].Chave, FALSE);
    TestaI(p->p[p->n], p->r[i].Chave, TRUE);
}

void AlturaI(TipoApontador Ap, int *height){
    long i;
    if (Ap == NULL)
        return;
    (*height)++;
    for (i = 0; i <= Ap->n; i++)
        AlturaI(Ap->p[i], height);    
};

void Altura(TipoApontador *Ap, int *qntdComparacoes, char *msg) {
    int height = 0;
    AlturaI((*Ap), &height);    
    sprintf(msg, "A altura da arvore e %d", height);
    return;
}
