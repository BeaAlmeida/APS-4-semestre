#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Nome do arquivo binario que contem um vetor desordenado com um milhao de posicoes
#define ARQ_VET_DESORD "vetor_1000000_desordenado.txt"

// Nome do arquivo texto que contem os resultados das ordenacoes
#define ARQ_SAIDA "saida.txt"

// Nome do arquivo binario com o vetor ordenado
#define ARQ_VET_ORD "vetor_ordenado.txt"

// Nome do arquivo binario com o vetor percentualmente ordenado
#define ARQ_VET_PCTO "vetor_percentualmente_ordenado.txt"

// Quantidade de numeros que serao ordenados
enum {QNT_IMAGENS = 1000000};

// Indicadores de qual parte do vetor ordenar
enum {INICIO, MEIO, FIM};

void insertionSort(int arr[], int n) {
    int i, j, key;

    for (i = 1; i < n; i++) {

        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void selectionSort(int arr[], int n) { 
    int i, j, min, aux;

    for (i = 0; i < (n - 1); i++) {

        min = i;

        for (j = (i + 1); j < n; j++) {

            if (arr[j] < arr[min])
                min = j;
        }

        if (arr[i] != arr[min]) {
            aux = arr[i];
            arr[i] = arr[min];
            arr[min] = aux;
        }
    }
}

void quickSort(int arr[], int first, int last) {
    int i, j, pivot, aux;
    i = first;
    j = last - 1;
    pivot = arr[(first + last) / 2];

    while(i <= j) {

        while(arr[i] < pivot)
            i++;

        while(arr[j] > pivot)
            j--;

        if(i <= j) {

            if (arr[i] != arr[j]) {
                aux = arr[i];
                arr[i] = arr[j];
                arr[j] = aux;
            }

            i++;
            j--;
        }
    }

    if(j > first)
        quickSort(arr, first, j + 1);
    
    if(i < last)
        quickSort(arr, i, last);
}

/* Preenche o vetor com os valores contidos no arquivo do vetor desordenado
Cria o arquivo se ele nao existir */
void iniciaVetorDesordenado(int vet[]) {
    FILE *arq;
    int i;

    arq = fopen(ARQ_VET_DESORD, "rb");

    if (arq == NULL) {
        int num;
        
        arq = fopen(ARQ_VET_DESORD, "wb");

        srand(time(NULL));

        for (i = 0; i < 1000000; i++) {

            // Gera um numero inteiro aleatorio na faixa de -1000000 a 999999
            num = 1000 * (rand() % 2000) + (rand() % 1000) - 1000000;
            fwrite(&num, sizeof(int), 1, arq);
        }

        fclose(arq);

        arq = fopen(ARQ_VET_DESORD, "rb");
    }

    i = fread(vet, sizeof(int), QNT_IMAGENS, arq);
    if (i != QNT_IMAGENS) {
        printf("Erro na leitura do arquivo "ARQ_VET_DESORD"!\n\n");
        printf("Pressione a tecla Enter para continuar. . . ");
        getchar();
        exit(1);
    }

    fclose(arq);
}

// Ordena o vetor e escreve o resultado em um arquivo
void iniciaVetorOrdenado(int vet[]) {
    FILE *arq;

    quickSort(vet, 0, QNT_IMAGENS);

    arq = fopen(ARQ_VET_ORD, "wb");

    fwrite(vet, sizeof(int), QNT_IMAGENS, arq);

    fclose(arq);
}

// Preenche o vetor com os valores contidos no arquivo
void sobrescreverVetor(int vet[], char nome_arq[]) {
	FILE *arq;

    arq = fopen(nome_arq, "rb");

    fread(vet, sizeof(int), QNT_IMAGENS, arq);

    fclose(arq);
}

// Troca a posicao dos numeros do vetor aleatoriamente para desordenar
void embaralhaVetor(int vet[], int primeiro, int ultimo) {
    int i, j, aux;
    int faixa = (ultimo - primeiro) / 100;

    srand(1); // define uma semente fixa para gerar sempre os mesmos numeros aleatorios

    for (i = primeiro; i < ultimo; i++) {
        
        j = 100 * (rand() % faixa) + (rand() % 100) + primeiro;

        if (vet[i] != vet[j]) {
            aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
        }
    }
}

/* Ordena uma porcentagem de um vetor desordenado e escreve o resultado em um arquivo
A ordenacao pode ser feita no inicio, meio, ou final do vetor */
void ordenaPercentualmente(int vet[], float porcentagem, int posicao) {
    FILE *arq;
    int tamanho = (1 - porcentagem) * QNT_IMAGENS;

    switch (posicao) {
        case INICIO:
            sobrescreverVetor(vet, ARQ_VET_ORD);

            embaralhaVetor(vet, QNT_IMAGENS - tamanho, QNT_IMAGENS);
            break;
        case MEIO:
            sobrescreverVetor(vet, ARQ_VET_ORD);

            embaralhaVetor(vet, 0, (tamanho / 2));
            embaralhaVetor(vet, QNT_IMAGENS - (tamanho / 2), QNT_IMAGENS);
            break;
        case FIM:
            sobrescreverVetor(vet, ARQ_VET_ORD);

            embaralhaVetor(vet, 0, tamanho);
            break;
        default:
            sobrescreverVetor(vet, ARQ_VET_DESORD);
    }

    arq = fopen(ARQ_VET_PCTO, "wb");

    fwrite(vet, sizeof(int), QNT_IMAGENS, arq);

    fclose(arq);
}

void imprimeVetor(int vet[]) {
	int i;

	for (i = 0; i < 5; i++) {
		printf("%d ", vet[i]);
	}

	for (i = QNT_IMAGENS - 5; i < QNT_IMAGENS; i++) {
		printf("%d ", vet[i]);
	}

	printf("\n");
}

int main() {
    FILE *arq_saida;
	int vetor[QNT_IMAGENS];
    clock_t tempo;
    double tempo_total;
    float porcentagem;
    int posicao;

    iniciaVetorDesordenado(vetor);
    iniciaVetorOrdenado(vetor);

    arq_saida = fopen(ARQ_SAIDA, "a");
    printf("Comparacao de metodos de ordenacao com um vetor de %d posicoes\n\n", QNT_IMAGENS);
    fprintf(arq_saida, "\tComparação de métodos de ordenação com um vetor de %d posições\n\n\n", QNT_IMAGENS);

    for (porcentagem = 0; porcentagem < 1; porcentagem += 0.25) {
        
        for (posicao = INICIO; posicao <= FIM; posicao++) {

            if (porcentagem == 0)
                posicao = FIM + 1;

            switch (posicao) {
                case INICIO:
                    printf("  Vetor %.f%% ordenado no inicio", porcentagem * 100);
                    fprintf(arq_saida, "Vetor %.f%% ordenado no início", porcentagem * 100);
                    break;
                case MEIO:
                    printf("  Vetor %.f%% ordenado no meio", porcentagem * 100);
                    fprintf(arq_saida, "Vetor %.f%% ordenado no meio", porcentagem * 100);
                    break;
                case FIM:
                    printf("  Vetor %.f%% ordenado no fim", porcentagem * 100);
                    fprintf(arq_saida, "Vetor %.f%% ordenado no fim", porcentagem * 100);
                    break;
                default:
                    printf("  Vetor desordenado");
                    fputs("Vetor desordenado", arq_saida);
            }

            //=== Insertionsort

            ordenaPercentualmente(vetor, porcentagem, posicao);

            printf("\n\n- Insertionsort\n\nOrdenando. . . ");

            tempo = clock(); // Tempo inicial
            insertionSort(vetor, QNT_IMAGENS);
            tempo = clock() - tempo; // Tempo final - inicial

            tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

            printf("\n\nTempo total: %lf segundos\n", tempo_total);
            fprintf(arq_saida, "\n\n\tInsertionsort\n\n\t\tTempo total: %lf segundos\n", tempo_total);

            //=== Selectionsort

            sobrescreverVetor(vetor, ARQ_VET_PCTO);

            printf("\n- Selectionsort\n\nOrdenando. . . ");

            tempo = clock(); // Tempo inicial
            selectionSort(vetor, QNT_IMAGENS);
            tempo = clock() - tempo; // Tempo final - inicial

            tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

            printf("\n\nTempo total: %lf segundos\n", tempo_total);
            fprintf(arq_saida, "\n\tSelectionsort\n\n\t\tTempo total: %lf segundos\n", tempo_total);
            
            //=== QuickSort

            sobrescreverVetor(vetor, ARQ_VET_PCTO);

            printf("\n- Quicksort\n\nOrdenando. . . ");

            tempo = clock(); // Tempo inicial
            quickSort(vetor, 0, QNT_IMAGENS);
            tempo = clock() - tempo; // Tempo final - inicial

            tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

            printf("\n\nTempo total: %lf segundos\n", tempo_total);
            fprintf(arq_saida, "\n\tQuicksort\n\n\t\tTempo total: %lf segundos\n", tempo_total);

            printf("\n================================\n\n");
            fputs("\n===============================================\n\n", arq_saida);
        }
    }
    
    fclose(arq_saida);

	printf("Pressione a tecla Enter para continuar. . . ");
	getchar();
	return 0;
}
