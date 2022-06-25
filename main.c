#include "GerenciadorDeDisco.h"

int main()
{
    Disco *disco = disco_cria("dados de arquivos", 15);
    if (disco != NULL)
        printf("\nexiste\n");
    else
        printf("\nNULL\n");

    imprimeSetoresLivres(disco);
    printf("\n");

    return 0;
}