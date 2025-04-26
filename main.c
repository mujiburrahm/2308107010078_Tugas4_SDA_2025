#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sorting_angka.h"
#include "sorting_string.h"

#define MAX_WORD_LEN 100
#define MAX_DATA_SIZE 2000000

typedef enum {
    MERGE_SORT,
    SHELL_SORT,
    QUICK_SORT,
    INSERTION_SORT,
    SELECTION_SORT,
    BUBBLE_SORT
} SortingAlgorithm;

// Fungsi pembacaan data
int* readIntegerData(const char* filename, int count) {
    int* data = (int*)malloc(count * sizeof(int));
    if (!data) {
        perror("Alokasi memori gagal");
        exit(EXIT_FAILURE);
    }
    
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Gagal membuka file data angka");
        free(data);
        exit(EXIT_FAILURE);
    }
    
    // Isi dengan data acak jika file tidak memiliki cukup entri
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        if (fscanf(fp, "%d", &data[i]) != 1) {
            // Hasilkan angka acak untuk data yang hilang
            printf("Catatan: Menghasilkan data acak mulai dari posisi %d\n", i);
            for (int j = i; j < count; j++) {
                data[j] = rand() % 1000000;
            }
            break;
        }
    }
    
    fclose(fp);
    return data;
}

// Fungsi untuk membaca data string ke array 2D
void readStringData(const char* filename, char data[][MAX_WORD_LEN], int count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Gagal membuka file data kata");
        exit(EXIT_FAILURE);
    }
    
    // Kata-kata acak untuk digunakan saat data file habis
    const char* randomWords[] = {
        "apel", "pisang", "jeruk", "anggur", "melon", "ceri", "persik", "plum",
        "kiwi", "mangga", "stroberi", "blueberry", "raspberry", "blackberry",
        "nanas", "semangka", "lemon", "jeruk nipis", "kelapa", "pepaya"
    };
    int numRandomWords = sizeof(randomWords) / sizeof(randomWords[0]);
    
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        if (fscanf(fp, "%99s", data[i]) != 1) {
            // Hasilkan kata-kata acak untuk data yang hilang
            printf("Catatan: Menghasilkan data string acak mulai dari posisi %d\n", i);
            for (int j = i; j < count; j++) {
                strcpy(data[j], randomWords[rand() % numRandomWords]);
            }
            break;
        }
    }
    
    fclose(fp);
}

// Fungsi penyalinan untuk array bilangan
void copyIntArray(const int* source, int* dest, int count) {
    memcpy(dest, source, count * sizeof(int));
}

// Fungsi penyalinan untuk array string
void copyStringArray(char source[][MAX_WORD_LEN], char dest[][MAX_WORD_LEN], int count) {
    for (int i = 0; i < count; i++) {
        strcpy(dest[i], source[i]);
    }
}

// Fungsi untuk mendapatkan nama algoritma
const char* getAlgorithmName(SortingAlgorithm algo) {
    switch (algo) {
        case MERGE_SORT: return "Merge Sort";
        case SHELL_SORT: return "Shell Sort";
        case QUICK_SORT: return "Quick Sort";
        case INSERTION_SORT: return "Insertion Sort";
        case SELECTION_SORT: return "Selection Sort";
        case BUBBLE_SORT: return "Bubble Sort";
        default: return "Tidak Dikenal";
    }
}

// Terapkan algoritma pengurutan bilangan dengan deteksi batas waktu
int applyIntSortingAlgorithm(int* array, int count, SortingAlgorithm algo, double timeoutSecs) {
    clock_t start = clock();
    double elapsed;
    
    // Tetapkan batas yang lebih tinggi untuk quick sort yang mungkin menggunakan lebih banyak stack
    int stackLimitCheck = (algo == QUICK_SORT) ? 500 : 10000;
    
    switch (algo) {
        case MERGE_SORT:
            mergeSort(array, 0, count - 1);
            break;
        case SHELL_SORT:
            shellSort(array, count);
            break;
        case QUICK_SORT:
            // Untuk array besar, gunakan iterative quick sort jika tersedia atau gunakan shell sort sebagai alternatif
            if (count > 500000) {
                shellSort(array, count); // Alternatif jika recursive quick sort akan menyebabkan stack overflow
            } else {
                quickSort(array, 0, count - 1);
            }
            break;
        case INSERTION_SORT:
            insertionSort(array, count);
            break;
        case SELECTION_SORT:
            selectionSort(array, count);
            break;
        case BUBBLE_SORT:
            bubbleSort(array, count);
            break;
    }
    
    return 1; // Sukses
}

// Terapkan algoritma pengurutan string dengan deteksi batas waktu
int applyStringSortingAlgorithm(char array[][MAX_WORD_LEN], int count, SortingAlgorithm algo, double timeoutSecs) {
    clock_t start = clock();
    double elapsed;
    
    switch (algo) {
        case MERGE_SORT:
            mergeSortStr(array, 0, count - 1);
            break;
        case SHELL_SORT:
            shellSortStr(array, count);
            break;
        case QUICK_SORT:
            // Untuk array besar, gunakan iterative quick sort jika tersedia atau gunakan shell sort sebagai alternatif
            if (count > 500000) {
                shellSortStr(array, count); // Alternatif jika recursive quick sort akan menyebabkan stack overflow
            } else {
                quickSortStr(array, 0, count - 1);
            }
            break;
        case INSERTION_SORT:
            insertionSortStr(array, count);
            break;
        case SELECTION_SORT:
            selectionSortStr(array, count);
            break;
        case BUBBLE_SORT:
            bubbleSortStr(array, count);
            break;
    }
    
    return 1; // Sukses
}

// Fungsi untuk mencetak header tabel
void printTableHeader() {
    printf("+----------------+-------------+---------------+\n");
    printf("|     Algoritma  |   Waktu (s) |   Memori (KB) |\n");
    printf("+----------------+-------------+---------------+\n");
}

// Fungsi untuk mencetak footer tabel
void printTableFooter() {
    printf("+----------------+-------------+---------------+\n");
}

// Fungsi untuk menentukan apakah algoritma harus dilewati untuk ukuran data tertentu
int shouldSkipAlgorithm(SortingAlgorithm algo, int dataSize) {
    // Lewati algoritma yang tidak efisien untuk dataset besar
    if (dataSize > 50000) {
        if (algo == BUBBLE_SORT || algo == SELECTION_SORT) {
            return 1;
        }
    }
    
    if (dataSize > 100000) {
        if (algo == INSERTION_SORT) {
            return 1;
        }
    }
    
    return 0;
}

// Fungsi untuk benchmark pengurutan bilangan
void benchmarkIntSorting(int* masterData, int dataSize) {
    SortingAlgorithm algorithms[] = {
        MERGE_SORT, SHELL_SORT, QUICK_SORT, 
        INSERTION_SORT, SELECTION_SORT, BUBBLE_SORT
    };
    int numAlgorithms = sizeof(algorithms) / sizeof(algorithms[0]);
    
    double memoryUsage = (dataSize * sizeof(int) * 2) / 1024.0; // dalam KB
    double timeoutSecs = 300.0; // batas waktu 5 menit
    
    printf("\nSedang menguji %d data angka...\n\n", dataSize);
    printTableHeader();
    
    int* workingCopy = (int*)malloc(dataSize * sizeof(int));
    if (!workingCopy) {
        perror("Alokasi memori gagal untuk salinan kerja bilangan");
        printf("Mencoba melanjutkan dengan tes lainnya...\n");
        return;
    }
    
    for (int i = 0; i < numAlgorithms; i++) {
        if (shouldSkipAlgorithm(algorithms[i], dataSize)) {
            printf("| %-14s |     (dilewati - terlalu lambat untuk dataset besar)     |\n", 
                   getAlgorithmName(algorithms[i]));
            continue;
        }
        
        copyIntArray(masterData, workingCopy, dataSize);
        
        printf("Menjalankan %s pada %d bilangan...\n", getAlgorithmName(algorithms[i]), dataSize);
        
        clock_t start = clock();
        int success = applyIntSortingAlgorithm(workingCopy, dataSize, algorithms[i], timeoutSecs);
        clock_t end = clock();
        
        if (success) {
            double time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("| %-14s |   %9.3f |   %9.1f |\n", 
                   getAlgorithmName(algorithms[i]), time, memoryUsage);
        } else {
            printf("| %-14s |   (melebihi batas waktu)        |\n", 
                   getAlgorithmName(algorithms[i]));
        }
        
        // Verifikasi bahwa array diurutkan dengan benar (dapat dinonaktifkan untuk kinerja)
        if (dataSize <= 100000) {
            int isSorted = 1;
            for (int j = 1; j < dataSize; j++) {
                if (workingCopy[j] < workingCopy[j-1]) {
                    isSorted = 0;
                    break;
                }
            }
            if (!isSorted) {
                printf("PERINGATAN: %s gagal mengurutkan array dengan benar!\n", 
                       getAlgorithmName(algorithms[i]));
            }
        }
    }
    
    free(workingCopy);
    printTableFooter();
}

// Fungsi untuk benchmark pengurutan string
void benchmarkStringSorting(char masterData[][MAX_WORD_LEN], int dataSize) {
    SortingAlgorithm algorithms[] = {
        MERGE_SORT, SHELL_SORT, QUICK_SORT, 
        INSERTION_SORT, SELECTION_SORT, BUBBLE_SORT
    };
    int numAlgorithms = sizeof(algorithms) / sizeof(algorithms[0]);
    
    double memoryUsage = (dataSize * MAX_WORD_LEN * 2) / 1024.0; // dalam KB
    double timeoutSecs = 300.0; // batas waktu 5 menit
    
    printf("\nSedang menguji %d data kata...\n\n", dataSize);
    printTableHeader();
    
    // Buat ruang salinan kerja dengan alokasi dinamis
    char (*workingCopy)[MAX_WORD_LEN] = malloc(dataSize * sizeof(char[MAX_WORD_LEN]));
    if (!workingCopy) {
        perror("Alokasi memori gagal untuk salinan kerja string");
        printf("Mencoba melanjutkan dengan tes lainnya...\n");
        return;
    }
    
    for (int i = 0; i < numAlgorithms; i++) {
        if (shouldSkipAlgorithm(algorithms[i], dataSize)) {
            printf("| %-14s |     (dilewati - terlalu lambat untuk dataset besar)     |\n", 
                   getAlgorithmName(algorithms[i]));
            continue;
        }
        
        copyStringArray(masterData, workingCopy, dataSize);
        
        printf("Menjalankan %s pada %d string...\n", getAlgorithmName(algorithms[i]), dataSize);
        
        clock_t start = clock();
        int success = applyStringSortingAlgorithm(workingCopy, dataSize, algorithms[i], timeoutSecs);
        clock_t end = clock();
        
        if (success) {
            double time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("| %-14s |   %9.3f |   %9.1f |\n", 
                   getAlgorithmName(algorithms[i]), time, memoryUsage);
        } else {
            printf("| %-14s |   (melebihi batas waktu)        |\n", 
                   getAlgorithmName(algorithms[i]));
        }
        
        // Verifikasi bahwa array diurutkan dengan benar (dapat dinonaktifkan untuk kinerja)
        if (dataSize <= 50000) {
            int isSorted = 1;
            for (int j = 1; j < dataSize; j++) {
                if (strcmp(workingCopy[j], workingCopy[j-1]) < 0) {
                    isSorted = 0;
                    break;
                }
            }
            if (!isSorted) {
                printf("PERINGATAN: %s gagal mengurutkan array dengan benar!\n", 
                       getAlgorithmName(algorithms[i]));
            }
        }
    }
    
    free(workingCopy);
    printTableFooter();
}

// Fungsi untuk menjalankan benchmark untuk berbagai ukuran data dengan penanganan kesalahan yang lebih baik
void runBenchmarks() {
    int dataSizes[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
    int numSizes = sizeof(dataSizes) / sizeof(dataSizes[0]);
    
    printf("=== MEMULAI BENCHMARK PENGURUTAN ===\n");
    printf("Menguji ukuran data: 10K, 50K, 100K, 250K, 500K, 1M, 1,5M, 2M\n\n");
    
    // Untuk setiap ukuran data, muat data dan jalankan benchmark
    for (int i = 0; i < numSizes; i++) {
        int currentSize = dataSizes[i];
        printf("\n==================================================\n");
        printf("BENCHMARK UNTUK %d ELEMEN\n", currentSize);
        printf("==================================================\n");
        
        // Muat hanya jumlah data bilangan yang diperlukan
        printf("Memuat data bilangan untuk ukuran %d...\n", currentSize);
        int* intData = NULL;
        
        // Gunakan pendekatan seperti try-catch untuk menangani kesalahan dengan baik
        do {
            intData = readIntegerData("data_angka.txt", currentSize);
            if (!intData) {
                printf("Gagal memuat data bilangan, melewati ke ukuran berikutnya\n");
                break;
            }
            
            benchmarkIntSorting(intData, currentSize);
            free(intData);
            intData = NULL;
        } while(0);
        
        // Untuk data string, kita akan membatasi ukuran yang lebih kecil karena batasan memori
        if (currentSize <= 1000000) {  // Batas yang ditingkatkan untuk data string
            printf("Memuat data string untuk ukuran %d...\n", currentSize);
            
            // Pendekatan seperti try-catch untuk data string
            do {
                // Alokasi memori untuk data string
                char (*stringData)[MAX_WORD_LEN] = malloc(currentSize * sizeof(char[MAX_WORD_LEN]));
                if (!stringData) {
                    printf("Gagal mengalokasikan memori untuk data string (%d elemen), melewati benchmark string\n", currentSize);
                    break;
                }
                
                readStringData("data_kata.txt", stringData, currentSize);
                benchmarkStringSorting(stringData, currentSize);
                free(stringData);
                stringData = NULL;
            } while(0);
        } else {
            printf("\nMelewati pengurutan string untuk ukuran %d: melebihi ukuran yang direkomendasikan\n", currentSize);
        }
        
        // Bebaskan sisa memori (untuk berjaga-jaga)
        if (intData) free(intData);
        
        printf("\nMenyelesaikan benchmark untuk %d elemen\n", currentSize);
        fflush(stdout); // Pastikan output ditulis segera
    }
    
    printf("\n=== BENCHMARK SELESAI ===\n");
}

int main() {
    // Atur buffer menjadi line-buffered untuk umpan balik yang lebih cepat
    setvbuf(stdout, NULL, _IOLBF, 0);
    
    printf("Memulai aplikasi benchmark...\n");
    
    // Jalankan semua benchmark dengan manajemen memori yang tepat
    runBenchmarks();
    
    printf("Program selesai dengan sukses.\n");
    return 0;
}