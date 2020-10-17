#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Nome do arquivo binario que contem os numeros
#define ARQUIVO "vetor_100000.txt"

// Quantidade de numeros que serao ordenados
enum {QNT_IMAGENS = 100000};

//===============================================
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
//===============================================
void selectionSort(int arr[], int n) { 
    int i, j, min, aux;

    for (i = 0; i < (n-1); i++) {

        min = i;

        for (j = (i+1); j < n; j++) {

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
//===============================================
int quickSort_partition(int arr[], int start, int end) {
    int i = start;
    int j, aux;

    for (j = start; j < end; j++) {

        if (arr[j] < arr[end]) {
            aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
            i++;
        }
    }

    aux = arr[i];
    arr[i] = arr[end];
    arr[end] = aux;

    return i;
}

void quickSort_sort(int arr[], int start, int end) {
    int pivot;

    if (start < end) {

        pivot = quickSort_partition(arr, start, end);

        quickSort_sort(arr, start, pivot - 1);
        quickSort_sort(arr, pivot + 1, end);
    }
}

void quickSort(int arr[], int n) {
    quickSort_sort(arr, 0, n - 1);
}
//===============================================

/* Preenche o vetor com os valores contidos no arquivo
que sera retornado pela funcao
Cria o arquivo se ele nao existir */
FILE* iniciaVetor(int vet[]) {
    FILE *arq;
    int i;

    arq = fopen(ARQUIVO, "rb");

    if (arq == NULL) {
        
        arq = fopen(ARQUIVO, "wb");

        srand(time(NULL));

        for (i = 0; i < QNT_IMAGENS; i++) {

            // Gera um numero inteiro aleatorio na faixa de -500000 a 499999
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

/* Retorna para o comeco do arquivo e preenche o vetor
novamente com os valores iniciais */
void reiniciaVetor(int vet[], FILE *arq) {
	rewind(arq);
	fread(vet, sizeof(int), QNT_IMAGENS, arq);
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

    arquivo = iniciaVetor(vetor);

	imprimeVetor(vetor);

    //=== Insertionsort

    tempo = clock(); // Tempo inicial
	insertionSort(vetor, QNT_IMAGENS);
    tempo = clock() - tempo; // Tempo final - inicial

    tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

	printf("\nInsertionsort\nTempo total: %.3lf segundos\n", tempo_total);
	imprimeVetor(vetor);

    //=== Selectionsort

	reiniciaVetor(vetor, arquivo);

    tempo = clock(); // Tempo inicial
	selectionSort(vetor, QNT_IMAGENS);
    tempo = clock() - tempo; // Tempo final - inicial

    tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

	printf("\nSelectionsort\nTempo total: %.3lf segundos\n", tempo_total);
	imprimeVetor(vetor);

    //=== Quicksort

    reiniciaVetor(vetor, arquivo);

    tempo = clock(); // Tempo inicial
    quickSort(vetor, QNT_IMAGENS);
    tempo = clock() - tempo; // Tempo final - inicial

    tempo_total = ((double) tempo) / CLOCKS_PER_SEC; // Tempo em segundos

    printf("\nQuicksort\nTempo total: %.3lf segundos\n", tempo_total);
    imprimeVetor(vetor);

	fclose(arquivo);

	printf("\nPressione a tecla Enter para continuar. . . ");
	getchar();
	return 0;
}
