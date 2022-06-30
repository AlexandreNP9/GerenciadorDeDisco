#include "GerenciadorDeDisco.h"

int main()
{
    Disco *disco = disco_cria("dados de arquivos", 1000);
    char path_txt[50] = "arquivos/grupo.txt";
    char path_png[50] = "arquivos/img.png";
    char path_jpg[50] = "arquivos/img2.jpg";

    disco_grava(disco, path_txt);
    disco_grava(disco, path_png);
    disco_grava(disco, path_jpg);
    disco_lista(disco);

    return 0;
}