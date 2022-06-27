#include "GerenciadorDeDisco.h"

int main()
{
    Disco *disco = disco_cria("dados de arquivos", 60);
    char path[50] = "arquivos/grupo.txt";

    disco_grava(disco, path);
    disco_lista(disco);

    return 0;
}