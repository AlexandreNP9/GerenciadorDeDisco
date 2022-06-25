#include "GerenciadorDeDisco.h"

// DADOS
typedef struct nosetor
{
    unsigned long inicio;
    unsigned long fim;
    struct nosetor *prox;
    struct nosetor *ant;
} NoSetor;

typedef struct noarquivo
{
    char nome[40];
    long int tam;
    NoSetor *setores;
    struct noarquivo *prox;
    struct noarquivo *ant;
} NoArquivo;

struct disco
{
    char nome[40];
    void *disco;
    NoSetor *setoresLivres;
    NoArquivo *arquivos;
    long int tamDisco;
    long int espacoLivre;
    long int qtdArquivos;
};

// AUXILIARES ================================================================================================================================================
unsigned int getTamanhoArquivo(FILE *arq)
{
    rewind(arq);
    fseek(arq, 0, SEEK_END);
    int tamanho = ftell(arq);
    rewind(arq);
    return tamanho;
}

bool duplicarArquivo(char *nomeArquivoEntrada, char *nomeArquivoSaida)
{
    FILE *arqEntrada;
    FILE *arqSaida;

    // abre o arquivo de entrada no modo leitura
    arqEntrada = fopen(nomeArquivoEntrada, "rb");
    if (arqEntrada == NULL)
    {
        perror("Erro na abertura do arquivo de entrada");
        return false;
    }

    // Captura o tamanho do arquivo
    int tamanhoArquivo = getTamanhoArquivo(arqEntrada);

    // Aloca um espaço na memória para copiar o conteúdo do arquivo
    void *buffer = malloc(tamanhoArquivo);
    if (buffer == NULL)
    {
        perror("Memoria insuficiente");
        return false;
    }

    // copia o conteúdo do arquivo para a memória
    fread(buffer, tamanhoArquivo, 1, arqEntrada);

    // copia o conteúdo da memória para o arquivo
    arqSaida = fopen(nomeArquivoSaida, "wb");
    if (arqSaida == NULL)
    {
        perror("Erro na abertura do arquivo de SAIDA");
        return false;
    }
    fwrite(buffer, tamanhoArquivo, 1, arqSaida);

    // fecha os arquivos
    fclose(arqEntrada);
    fclose(arqSaida);
}

void imprimeSetoresLivres(Disco *d)
{
    printf("LIVRES: ");
    NoSetor *aux = d->setoresLivres->prox;
    while (aux != d->setoresLivres)
    {
        printf("[%ld,%ld] ", aux->inicio, aux->fim);
        aux = aux->prox;
    }
    printf("\n");
}

bool inserir_setor(Disco *disco, unsigned long inicio, unsigned long fim)
{
    NoSetor *new = (NoSetor *)malloc(sizeof(NoSetor));

    if (new == NULL)
        return false;
    if (fim <= inicio)
        return false;

    // checa conflito de espaços
    NoSetor *aux = disco->setoresLivres->prox;
    while (aux != disco->setoresLivres)
    {
        if (inicio == aux->inicio || fim == aux->fim) // sobreposição de valores
            return false;
        if (fim > aux->inicio && inicio < aux->fim) // intersecção no setor a frente
            return false;
        if (aux->fim > inicio && aux->inicio < fim) // intersecção no setor a tras
            return false;
        aux = aux->prox;
    }

    // implementação do espaço livre
    new->inicio = inicio;
    new->fim = fim;

    // ajusta as posições dos nós setores
    new->prox = disco->setoresLivres;
    disco->setoresLivres->ant->prox = new;
    new->ant = disco->setoresLivres->ant;
    disco->setoresLivres->ant = new;
    return true;
}

// IMPLEMENTAÇÃO ================================================================================================================================================
Disco *disco_cria(char *nome, long int tamanho)
{
    Disco *disco = (Disco *)malloc(sizeof(Disco));
    if (disco == NULL)
    {
        perror("there is not enough memory to create >>disco<<");
        return NULL;
    }
    strcpy(disco->nome, nome);
    disco->disco = malloc(tamanho);
    disco->tamDisco = tamanho;
    disco->espacoLivre = tamanho;
    disco->qtdArquivos = 0;

    // Cria setores livres
    NoSetor *newSetor = (NoSetor *)malloc(sizeof(NoSetor));
    if (newSetor == NULL)
    {
        perror("theres is not enough memory to create >>sectors<<");
        return NULL;
    }
    newSetor->prox = newSetor;
    newSetor->ant = newSetor;
    disco->setoresLivres = newSetor;
    if (!inserir_setor(disco, 0, tamanho - 1))
    {
        perror("Unable to create a sector of empty space >>empty sector<<");
        return false;
    }

    // Cria arquivo
    NoArquivo *newArquivo = (NoArquivo *)malloc(sizeof(NoArquivo));
    if (newArquivo == NULL)
    {
        perror("theres is not enough memory to create >>arquivo<<");
        return NULL;
    }
    strcpy(newArquivo->nome, nome);
    newArquivo->prox = newArquivo;
    newArquivo->ant = newArquivo;
    newArquivo->setores = NULL;
    newArquivo->tam = 0;
    disco->arquivos = newArquivo;

    return disco;
}

// nome arquivo deve conter o caminho absoluto ou relativo do arquivo
bool disco_grava(Disco *d, char *arquivo);
bool disco_remove(Disco *d, char *nome);                         // somente o nome do arquivo sem o caminho
bool disco_recupera(Disco *d, char *nome, char *arquivoDestino); // nome arquivo deve conter o caminho absoluto ou relativo do arquivo

void disco_lista(Disco *d)
{
    printf("-----------------\n");
    imprimeSetoresLivres(d);
    printf("-----------------\n");
    printf("ARQUIVOS: \n");

    NoArquivo *arquivo = d->arquivos->prox;
    while (arquivo != d->arquivos)
    {
        printf("%-30s (%9ld) ", arquivo->nome, arquivo->tam);

        NoSetor *segmento = arquivo->setores->prox;
        while (segmento != arquivo->setores)
        {
            printf("[%ld,%ld] ", segmento->inicio, segmento->fim);
            segmento = segmento->prox;
        }
        printf("\n");
        arquivo = arquivo->prox;
    }
    printf("\n");
}