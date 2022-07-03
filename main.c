#include "GerenciadorDeDisco.h"

int main()
{
    Disco *disco = disco_cria("dados de arquivos", 1000);
    char path_txt[50] = "arquivos/grupo.txt";
    char path_png[50] = "arquivos/img.png";
    char path_jpg[50] = "arquivos/img2.jpg";
    char path_RPGIcon[50] = "arquivos/RPGIcon.png";

    printf("\nGrava arquivos no disco\n");
    disco_grava(disco, path_txt);
    disco_grava(disco, path_png);
    disco_grava(disco, path_jpg);
    disco_lista(disco);

    printf("Remove um arquivo do disco\n");
    disco_remove(disco, "img.png");
    disco_lista(disco);

    printf("Grava um arquivo no disco fragmentado\n");
    disco_grava(disco, path_RPGIcon);
    disco_lista(disco);

    printf("Remove arquivo fragmentado\n");
    disco_remove(disco, "RPGIcon.png");
    disco_lista(disco);

    printf("Grava um arquivo no disco fragmentado novamente\n");
    disco_grava(disco, path_RPGIcon);
    disco_lista(disco);

    printf("Recupera arquivo do disco\n");
    disco_recupera(disco, "img2.jpg", "arquivos/img2 (copia).jpg");

    printf("Recupera arquivo fragmentado do disco\n");
    disco_recupera(disco, "RPGIcon.png", "arquivos/RPGIcon (copia).png");

    return 0;
}