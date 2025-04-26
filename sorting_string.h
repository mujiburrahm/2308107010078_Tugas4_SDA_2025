#ifndef SORTING_STRING_H
#define SORTING_STRING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Bubble Sort String
void bubbleSortStr(char arr[][100], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (strcmp(arr[j], arr[j+1]) > 0) {
                char temp[100];
                strcpy(temp, arr[j]);
                strcpy(arr[j], arr[j+1]);
                strcpy(arr[j+1], temp);
            }
        }
    }
}

// Selection Sort String
void selectionSortStr(char arr[][100], int n) {
    for (int i = 0; i < n-1; i++) {
        int minIdx = i;
        for (int j = i+1; j < n; j++) {
            if (strcmp(arr[j], arr[minIdx]) < 0)
                minIdx = j;
        }
        char temp[100];
        strcpy(temp, arr[minIdx]);
        strcpy(arr[minIdx], arr[i]);
        strcpy(arr[i], temp);
    }
}

// Insertion Sort String
void insertionSortStr(char arr[][100], int n) {
    for (int i = 1; i < n; i++) {
        char key[100];
        strcpy(key, arr[i]);
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            strcpy(arr[j+1], arr[j]);
            j--;
        }
        strcpy(arr[j+1], key);
    }
}

// Merge Sort String
void mergeStr(char arr[][100], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Alokasi ke heap
    char (*L)[100] = (char (*)[100]) malloc(n1 * 100);
    char (*R)[100] = (char (*)[100]) malloc(n2 * 100);

    // Copy data ke array sementara
    for (int i = 0; i < n1; i++) strcpy(L[i], arr[l + i]);
    for (int j = 0; j < n2; j++) strcpy(R[j], arr[m + 1 + j]);

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (strcmp(L[i], R[j]) <= 0)
            strcpy(arr[k++], L[i++]);
        else
            strcpy(arr[k++], R[j++]);
    }
    while (i < n1) strcpy(arr[k++], L[i++]);
    while (j < n2) strcpy(arr[k++], R[j++]);

    // Bebaskan memori
    free(L);
    free(R);
}

void mergeSortStr(char arr[][100], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortStr(arr, l, m);
        mergeSortStr(arr, m+1, r);
        mergeStr(arr, l, m, r);
    }
}

// Quick Sort String
int partitionStr(char arr[][100], int low, int high) {
    char pivot[100];
    strcpy(pivot, arr[high]);
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j], pivot) <= 0) {
            i++;
            char temp[100];
            strcpy(temp, arr[i]);
            strcpy(arr[i], arr[j]);
            strcpy(arr[j], temp);
        }
    }
    char temp[100];
    strcpy(temp, arr[i+1]);
    strcpy(arr[i+1], arr[high]);
    strcpy(arr[high], temp);
    return (i + 1);
}

void quickSortStr(char arr[][100], int low, int high) {
    if (low < high) {
        int pi = partitionStr(arr, low, high);
        quickSortStr(arr, low, pi-1);
        quickSortStr(arr, pi+1, high);
    }
}

// Shell Sort String
void shellSortStr(char arr[][100], int n) {
    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            char temp[100];
            strcpy(temp, arr[i]);
            int j;
            for (j = i; j >= gap && strcmp(arr[j-gap], temp) > 0; j -= gap) {
                strcpy(arr[j], arr[j-gap]);
            }
            strcpy(arr[j], temp);
        }
    }
}

#endif
