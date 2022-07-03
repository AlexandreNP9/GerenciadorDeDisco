#include "GerenciadorDeDisco.h"

int main()
{
    Disco *disco = disco_cria("dados de arquivos", 1000);

    printf("\nGrava arquivos no disco\n");
    disco_grava(disco, "grupo.txt");
    disco_grava(disco, "img.png");
    disco_grava(disco, "img2.jpg");
    disco_lista(disco);

    printf("Remove um arquivo do disco\n");
    disco_remove(disco, "img.png");
    disco_lista(disco);

    printf("Grava um arquivo no disco fragmentado\n");
    disco_grava(disco, "RPGIcon.png");
    disco_lista(disco);

    printf("Remove arquivo fragmentado\n");
    disco_remove(disco, "RPGIcon.png");
    disco_lista(disco);

    printf("Grava um arquivo no disco fragmentado novamente\n");
    disco_grava(disco, "RPGIcon.png");
    disco_lista(disco);

    printf("Recupera arquivo do disco\n");
    disco_recupera(disco, "img2.jpg", "arquivos/img2 (copia).jpg");

    printf("Recupera arquivo fragmentado do disco\n");
    disco_recupera(disco, "RPGIcon.png", "arquivos/RPGIcon (copia).png");

    return 0;
}