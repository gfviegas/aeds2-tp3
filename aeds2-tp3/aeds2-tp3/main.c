#include <stdlib.h>
#include <stdio.h>
#include "arvoreb/arvoreb.h"

int main()
{
    TipoRegistro x;
    TipoPagina *Arvore;
    Inicializa(&Arvore);
    printf("Chave: ");
    scanf("%ld%*[^\n]", &x.Chave);
    getchar();
    while (x.Chave != 0)
    {
        Insere(x, &Arvore);
        Imprime(Arvore);
        printf("Chave: ");
        scanf("%ld%*[^\n]", &x.Chave);
        getchar();
    }
    printf("Chave: ");
    scanf("%ld%*[^\n]", &x.Chave);
    getchar();
    while (x.Chave != 0)
    {
        Retira(x.Chave, &Arvore);
        Imprime(Arvore);
        printf("Chave: ");
        scanf("%ld%*[^\n]", &x.Chave);
        getchar();
    }
    return 0;
}