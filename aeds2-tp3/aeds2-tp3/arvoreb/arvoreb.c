#include "arvoreb.h"
#include <stddef.h>

/**   
 * @note   Inicializa uma nova árvore
 * @param  *Dicionario: nó inicial da árvore
 * @retval None
 */
void Inicializa(TipoApontador *Dicionario)
{
    *Dicionario = NULL;
}

/** 
 * @note Pesquisa um dado registro na árvore   
 * @param  *x: registro buscado
 * @param  Ap: apontador do nó
 * @retval None
 */
void Pesquisa(TipoRegistro *x, TipoApontador Ap)
{
    long i = 1;
    if (Ap == NULL)
    {
        printf("TipoRegistro nao esta presente na arvore\n");
        return;
    }
    while (i < Ap->n && x->Chave > Ap->r[i - 1].Chave)
        i++;
    if (x->Chave == Ap->r[i - 1].Chave)
    {
        *x = Ap->r[i - 1];
        return;
    }
    if (x->Chave < Ap->r[i - 1].Chave)
        Pesquisa(x, Ap->p[i - 1]);
    else
        Pesquisa(x, Ap->p[i]);
}

/** 
 * @note Insere um novo registro na página  
 * @param  Ap: apontador do nó
 * @param  Reg: registro a ser inserido
 * @param  ApDir: nó da direita
 * @retval None
 */
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir)
{
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao)
    {
        if (Reg.Chave >= Ap->r[k - 1].Chave)
        {
            NaoAchouPosicao = FALSE;
            break;
        }
        Ap->r[k] = Ap->r[k - 1];
        Ap->p[k + 1] = Ap->p[k];
        k--;
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
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno)
{
    long i = 1;
    long j;
    TipoApontador ApTemp;
    if (Ap == NULL)
    {
        *Cresceu = TRUE;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave)
        i++;
    if (Reg.Chave == Ap->r[i - 1].Chave)
    {
        printf(" Erro: Registro ja esta presente\n");
        *Cresceu = FALSE;
        return;
    }
    if (Reg.Chave < Ap->r[i - 1].Chave)
        i--;
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
    if (!*Cresceu)
        return;
    if (Ap->n < MM) /* Pagina tem espaco */
    {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = FALSE;
        return;
    }
    /* Overflow: Pagina tem que ser dividida */
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i < M + 1)
    {
        InsereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM]);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    }
    else
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    for (j = M + 2; j <= MM; j++)
        InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M + 1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
}

/** 
 * @note Insere um dado registro na árvore   
 * @param  Reg: registro a ser inserido
 * @param  *Ap: nó pai da árvore
 * @retval None
 */
void Insere(TipoRegistro Reg, TipoApontador *Ap)
{
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if (Cresceu) /* Arvore cresce na altura pela raiz */
    {
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
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
void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, int PosPai, short *Diminuiu)
{
    TipoPagina *Aux;
    long DispAux, j;
    if (PosPai < ApPai->n) /* Aux = TipoPagina a direita de ApPag */
    {
        Aux = ApPai->p[PosPai + 1];
        DispAux = (Aux->n - M + 1) / 2;
        ApPag->r[ApPag->n] = ApPai->r[PosPai];
        ApPag->p[ApPag->n + 1] = Aux->p[0];
        ApPag->n++;
        if (DispAux > 0) /* Existe folga: transfere de Aux para ApPag */
        {
            for (j = 1; j < DispAux; j++)
                InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
            ApPai->r[PosPai] = Aux->r[DispAux - 1];
            Aux->n -= DispAux;
            for (j = 0; j < Aux->n; j++)
                Aux->r[j] = Aux->r[j + DispAux];
            for (j = 0; j <= Aux->n; j++)
                Aux->p[j] = Aux->p[j + DispAux];
            *Diminuiu = FALSE;
        }
        else /* Fusao: intercala Aux em ApPag e libera Aux */
        {
            for (j = 1; j <= M; j++)
                InsereNaPagina(ApPag, Aux->r[j - 1], Aux->p[j]);
            free(Aux);
            for (j = PosPai + 1; j < ApPai->n; j++)
            {
                ApPai->r[j - 1] = ApPai->r[j];
                ApPai->p[j] = ApPai->p[j + 1];
            }
            ApPai->n--;
            if (ApPai->n >= M)
                *Diminuiu = FALSE;
        }
    }
    else /* Aux = TipoPagina a esquerda de ApPag */
    {
        Aux = ApPai->p[PosPai - 1];
        DispAux = (Aux->n - M + 1) / 2;
        for (j = ApPag->n; j >= 1; j--)
            ApPag->r[j] = ApPag->r[j - 1];
        ApPag->r[0] = ApPai->r[PosPai - 1];
        for (j = ApPag->n; j >= 0; j--)
            ApPag->p[j + 1] = ApPag->p[j];
        ApPag->n++;
        if (DispAux > 0) /* Existe folga: transf. de Aux para ApPag */
        {
            for (j = 1; j < DispAux; j++)
                InsereNaPagina(ApPag, Aux->r[Aux->n - j], Aux->p[Aux->n - j + 1]);
            ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
            ApPai->r[PosPai - 1] = Aux->r[Aux->n - DispAux];
            Aux->n -= DispAux;
            *Diminuiu = FALSE;
        }
        else /* Fusao: intercala ApPag em Aux e libera ApPag */
        {
            for (j = 1; j <= M; j++)
                InsereNaPagina(Aux, ApPag->r[j - 1], ApPag->p[j]);
            free(ApPag);
            ApPai->n--;
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
void Antecessor(TipoApontador Ap, int Ind, TipoApontador ApPai, short *Diminuiu)
{
    if (ApPai->p[ApPai->n] != NULL)
    {
        Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
        if (*Diminuiu)
            Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu);
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
void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu)
{
    long j, Ind = 1;
    TipoApontador Pag;
    if (*Ap == NULL)
    {
        printf("Erro: registro nao esta na arvore\n");
        *Diminuiu = FALSE;
        return;
    }
    Pag = *Ap;
    while (Ind < Pag->n && Ch > Pag->r[Ind - 1].Chave)
        Ind++;
    if (Ch == Pag->r[Ind - 1].Chave)
    {
        if (Pag->p[Ind - 1] == NULL) /* TipoPagina folha */
        {
            Pag->n--;
            *Diminuiu = (Pag->n < M);
            for (j = Ind; j <= Pag->n; j++)
            {
                Pag->r[j - 1] = Pag->r[j];
                Pag->p[j] = Pag->p[j + 1];
            }
            return;
        }
        /* TipoPagina nao e folha: trocar com antecessor */
        Antecessor(*Ap, Ind, Pag->p[Ind - 1], Diminuiu);
        if (*Diminuiu)
            Reconstitui(Pag->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
        return;
    }
    if (Ch > Pag->r[Ind - 1].Chave)
        Ind++;
    Ret(Ch, &Pag->p[Ind - 1], Diminuiu);
    if (*Diminuiu)
        Reconstitui(Pag->p[Ind - 1], *Ap, Ind - 1, Diminuiu);
}

/** 
 * @note   Retira um registro da árvore a partir da raiz
 * @param  Ch: chave a ser retirada
 * @param  *Ap: nó raiz da árvore 
 * @retval None
 */
void Retira(TipoChave Ch, TipoApontador *Ap)
{
    short Diminuiu;
    TipoApontador Aux;
    Ret(Ch, Ap, &Diminuiu);
    if (Diminuiu && (*Ap)->n == 0) /* Arvore diminui na altura */
    {
        Aux = *Ap;
        *Ap = Aux->p[0];
        free(Aux);
    }
}

/** 
 * @note   Função auxiliar ao imprimir a árvore
 * @param  p: nó a ser impresso
 * @param  nivel: nível onde ele se encontra
 * @retval None
 */
void ImprimeI(TipoApontador p, int nivel)
{
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
void Imprime(TipoApontador p)
{
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
void TestaI(TipoApontador p, int pai, short direita)
{
    int i;
    int antecessor = 0;
    if (p == NULL)
        return;
    if (p->r[0].Chave > pai && direita == FALSE)
    {
        printf("Erro: filho %12ld maior que pai %d\n", p->r[0].Chave, pai);
        return;
    }
    for (i = 0; i < p->n; i++)
    {
        if (p->r[i].Chave <= antecessor)
        {
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
void Testa(TipoApontador p)
{
    int i;
    if (p == NULL)
        return;
    for (i = 0; i < p->n; i++)
        TestaI(p->p[i], p->r[i].Chave, FALSE);
    TestaI(p->p[p->n], p->r[i].Chave, TRUE);
}