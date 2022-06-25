#ifndef _GERENCIADOR_DE_DISCO_H_
#define _GERENCIADOR_DE_DISCO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DIRECTORY "./arquivos"

typedef struct disco Disco;

Disco* disco_cria(char* nome, long int tamanho);
bool disco_grava(Disco* d, char* arquivo); // nome arquivo deve conter o caminho absoluto ou relativo do arquivo
bool disco_remove(Disco* d, char* nome); // somente o nome do arquivo sem o caminho
bool disco_recupera(Disco* d, char* nome, char* arquivoDestino); // nome arquivo deve conter o caminho absoluto ou relativo do arquivo
void disco_lista(Disco* d);
void imprimeSetoresLivres(Disco *d);
unsigned int getTamanhoArquivo(FILE *arq);

#endif