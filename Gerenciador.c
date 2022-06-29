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

bool inserir_setor(NoSetor *listaSetores, NoSetor *setor)
{
    if (listaSetores == NULL)
        return false;
    if (setor == NULL)
        return false;
    if (setor->fim <= setor->inicio)
        return false;

    // checa conflito de espaços
    NoSetor *aux = listaSetores->prox;
    while (aux != listaSetores)
    {
        if (setor->inicio == aux->inicio || setor->fim == aux->fim) // sobreposição de valores
            return false;
        if (setor->fim > aux->inicio && setor->inicio < aux->fim) // intersecção no setor a frente
            return false;
        if (aux->fim > setor->inicio && aux->inicio < setor->fim) // intersecção no setor a tras
            return false;
        aux = aux->prox;
    }

    // encadeia o setor na lista
    setor->prox = listaSetores;
    listaSetores->ant->prox = setor;
    setor->ant = listaSetores->ant;
    listaSetores->ant = setor;
    return true;
}

bool criar_listaSetores(NoSetor **enderecoListaSetores)
{
    NoSetor *new = (NoSetor *)malloc(sizeof(NoSetor));

    if (new == NULL)
        return false;

    new->prox = new;
    new->ant = new;
    *enderecoListaSetores = new;
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

    if (!criar_listaSetores(&(disco->setoresLivres)))
    {
        perror("Unable to create a list of sectors of empty space >>List Sectors<<");
        return NULL;
    }

    // Insere primeiro espaco livre
    NoSetor *new = (NoSetor *)malloc(sizeof(NoSetor));
    new->inicio = 0;
    new->fim = tamanho - 1;
    if (!inserir_setor(disco->setoresLivres, new))
    {
        perror("Cold not insert the first sector of empty space");
        return NULL;
    }

    // Cria lista de arquivos
    NoArquivo *listaArquivos = (NoArquivo *)malloc(sizeof(NoArquivo));
    if (listaArquivos == NULL)
    {
        perror("theres is not enough memory to create");
        return NULL;
    }
    strcpy(listaArquivos->nome, "Sentinela");
    listaArquivos->prox = listaArquivos;
    listaArquivos->ant = listaArquivos;
    listaArquivos->setores = NULL;
    listaArquivos->tam = 0;
    disco->arquivos = listaArquivos;

    return disco;
}

// nome arquivo deve conter o caminho absoluto ou relativo do arquivo
bool disco_grava(Disco *d, char *arquivo)
{
    // abre arquivo
    FILE *arq = fopen(arquivo, "rb");
    if (arq == NULL)
    {
        perror("The file is null");
        return false;
    }

    unsigned long tamArquivo = getTamanhoArquivo(arq);

    // cria arquivo e implementa os dados
    NoArquivo *newArquivo = (NoArquivo *)malloc(sizeof(NoArquivo));
    strcpy(newArquivo->nome, arquivo);
    newArquivo->tam = tamArquivo;

    // encadeia o arquivo na lista de arquivos
    newArquivo->prox = d->arquivos;
    d->arquivos->ant->prox = newArquivo;
    newArquivo->ant = d->arquivos->ant;
    d->arquivos->ant = newArquivo;

    if (!criar_listaSetores(&(newArquivo->setores)))
    {
        perror("Unable to create a list of sectors >>List sectors<<");
        return false;
    }

    // verifica se há espaços livres
    if (tamArquivo > d->espacoLivre)
    {
        perror("There is not enough memory on disk!!");
        return false;
    }

    // aloca setor do arquivo
    unsigned long espacoArquivo = tamArquivo;
    while (espacoArquivo > 0)
    {
        NoSetor *livre = d->setoresLivres->prox;
        unsigned long espacoLivre = livre->fim - (livre->inicio - 1);
        if (espacoArquivo < espacoLivre)
        {
            NoSetor *new = (NoSetor *)malloc(sizeof(NoSetor));
            if (new == NULL)
                return false;
            new->inicio = livre->inicio;
            new->fim = (livre->inicio - 1) + espacoArquivo;
            livre->inicio = new->fim + 1;

            // aloca setor do arquivo no espaco solicitado
            if (!inserir_setor(newArquivo->setores, new))
            {
                perror("Could not possible to insert file sector (Unexpected error)");
                return false;
            }
            espacoArquivo = 0;
        }
        else
        {
            // desencadeia o setor livre da lista
            livre->ant->prox = livre->prox;
            livre->prox->ant = livre->ant;

            // aloca setor de parte do arquivo no espaco solicitado
            if (!inserir_setor(newArquivo->setores, livre))
            {
                perror("Could not possible to insert file sector (Unexpected error)");
                return false;
            }
            espacoArquivo -= espacoLivre;
        }
    }
    d->espacoLivre -= tamArquivo;

    // TODO copia o conteudo do arquivo para a memoria
    NoSetor *setor = newArquivo->setores->prox;
    for (int i = 0; i < tamArquivo; i++)
    {
        int index = i + setor->inicio;
        if (index > setor->fim)
            setor = setor->prox;

        d->disco[index] = arq[i]; // PROBLEMA !!!!
    }

    // TODO grava no disco
    fclose(arq);
}

// somente o nome do arquivo sem o caminho
bool disco_remove(Disco *d, char *nome);

// nome arquivo deve conter o caminho absoluto ou relativo do arquivo
bool disco_recupera(Disco *d, char *nome, char *arquivoDestino);

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