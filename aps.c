#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Nome do arquivo binário que contém os números
#define ARQUIVO "vetor_100000.txt"

// Quantidade de números que serão ordenados
enum {QNT_IMAGENS = 100000};

enum {INICIO, MEIO, FIM};

void insertionSort(int arr[], int n) {
    int i, key, j;

    for (i = 1; i < n; i++) {

        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
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
    j = last;
    pivot = arr[first];

    while(i <= j) {

        while(arr[i] < pivot && i < last)
            i++;

        while(arr[j] > pivot && j > first)
            j--;

        if(i <= j) {
            aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
            i++;
            j--;
        }
    }

    if(j > first)
        quickSort(arr, first, j);
    
    if(i < last)
        quickSort(arr, i, last);
}

/* Preenche o vetor com os valores contidos no arquivo
que será retornado pela função
Cria o arquivo se ele não existir */
FILE* iniciaVetor(int vet[]) {
    FILE *arq;
    int i;

    arq = fopen(ARQUIVO, "rb");

    if (arq == NULL) {
        
        arq = fopen(ARQUIVO, "wb");

        srand(time(NULL));

        for (i = 0; i < QNT_IMAGENS; i++) {

            // Gera um número inteiro aleatório na faixa de -500000 a 499999
            vet[i] = 1000 * (rand() % 1000) + (rand() % 1000) - 500000;
        }

        fwrite(vet, sizeof(int), QNT_IMAGENS, arq);

        fclose(arq);

        arq = fopen(ARQUIVO, "rb");

        return arq;
    }

    i = fread(vet, sizeof(int), QNT_IMAGENS, arq);
    if (i != QNT_IMAGENS) {
        printf("Erro na leitura do arquivo "ARQUIVO"!\n");
        getchar();
        exit(1);
    }

    return arq;
}

/* Retorna para o começo do arquivo e preenche o vetor
novamente com os valores iniciais */
void reiniciaVetor(int vet[], FILE *arq) {
	rewind(arq);
	fread(vet, sizeof(int), QNT_IMAGENS, arq);
}

void ordenaPercentualmente(int vet[], float porcentagem, int posicao) {
    int tamanho = porcentagem * QNT_IMAGENS;

    switch (posicao) {
        case INICIO:
            quickSort(vet, 0, tamanho - 1);
            break;
        case MEIO:
            quickSort(vet, (QNT_IMAGENS / 2) - (tamanho / 2), (QNT_IMAGENS / 2) + (tamanho / 2) - 1);
            break;
        case FIM:
            quickSort(vet, QNT_IMAGENS - tamanho, QNT_IMAGENS - 1);
            break;
    }
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
	FILE *arquivo;
	int vetor[QNT_IMAGENS];
    clock_t tempo;
    double tempo_total;
    float porcentagem;
    int posicao;

    arquivo = iniciaVetor(vetor);

    for (porcentagem = 0; porcentagem < 1; porcentagem += 0.25) {
        
        for (posicao = INICIO; posicao <= FIM; posicao++) {

            ordenaPercentualmente(vetor, porcentagem, posicao);
            //imprimeVetor(vetor);
            printf("Vetor %.f%% ordenado no %d\n", porcentagem * 100, posicao);

            //=== Insertionsort

            tempo = clock(); // Tempo inicial
            insertionSort(vetor, QNT_IMAGENS);
            tempo = clock() - tempo; // Tempo final - inicial

            tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

            printf("\nInsertionsort\nTempo total: %.3lf segundos\n", tempo_total);
            //imprimeVetor(vetor);

            //=== Selectionsort

            reiniciaVetor(vetor, arquivo);
            ordenaPercentualmente(vetor, porcentagem, posicao);

            tempo = clock(); // Tempo inicial
            selectionSort(vetor, QNT_IMAGENS);
            tempo = clock() - tempo; // Tempo final - inicial

            tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

            printf("\nSelectionsort\nTempo total: %.3lf segundos\n", tempo_total);
            //imprimeVetor(vetor);

            //=== QuickSort

            reiniciaVetor(vetor, arquivo);
            ordenaPercentualmente(vetor, porcentagem, posicao);

            tempo = clock(); // Tempo inicial
            quickSort(vetor, 0, QNT_IMAGENS - 1);
            tempo = clock() - tempo; // Tempo final - inicial

            tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

            printf("\nQuicksort\nTempo total: %.3lf segundos\n", tempo_total);
            //imprimeVetor(vetor);

            printf("\n===============================\n\n");

            reiniciaVetor(vetor, arquivo);
            
            if (porcentagem == 0)
                break;
        }
    }

	fclose(arquivo);

	printf("Pressione a tecla Enter para continuar. . . ");
	getchar();
	return 0;
}
