#include "GerenciadorDeDisco.h"

int main()
{
    // verifica a existencia do disco
    Disco *disco = disco_cria("dados de arquivos", 15);
    if (disco != NULL)
        printf("\nexiste\n");
    else
        printf("\nNULL\n");

    // checando os arquivos
    char path[50] = "arquivos/grupo.txt";
    unsigned long tamFile = getTamanhoArquivo(fopen(path, "r"));
    printf("tamanho do arquivo: %d bytes\n", tamFile);

    // verificando os espaços livres dentro do disco
    imprimeSetoresLivres(disco);
    printf("\n");

    return 0;
}