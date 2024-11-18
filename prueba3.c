#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    unsigned char DD;
    unsigned char MM;
    unsigned short int AAAA;
} date;

typedef struct {
  long int OrderNumber;
  unsigned char LineItem;
  date OrderDate;
  date DeliveryDate;
  unsigned int CustomerKey;
  unsigned short int StoreKey;
  unsigned short int ProductKey;
  unsigned short int Quantity;
  char CurrencyCode[3];
} SalesData;

typedef struct {
    unsigned int CustomersKey;
    char Gender[8];
    char Name[40];
    char City[40];
    char StateCode[30];
    char State[30];
    unsigned int ZipCode;
    char Country[20];
    char Continent[20];
    date Birthday;
} CustomersData;

typedef struct {
    unsigned short int ProductKey;
    char ProductName[85];
    char Brand[30];
    char Color[15];
    float UnitCostUSD;
    float UnitPriceUSD;
    char SubcategoryKey[4];
    char Subcategory[35];
    char CategoryKey[2];
    char Category[30];
} ProductsData;

typedef struct {
    unsigned short int StoreKey;
    char Country[35];
    char State[35];
    unsigned short int SquareMeters;
    date OpenDate;
} StoresData;

typedef struct {
    char Date[10];
    char Currency[3];
    float Exchange;
} ExchangeRates;

typedef int (*CompareFunc)(const void*, const void*);

void Merge(void* arr, int left, int right, int medium, size_t size, CompareFunc compare) {
    int firstArray = medium - left + 1;
    int secondArray = right - medium;

    void* temporalLeft = malloc(firstArray * size);
    void* temporalRight = malloc(secondArray * size);

    memcpy(temporalLeft, (char*)arr + left * size, firstArray * size);
    memcpy(temporalRight, (char*)arr + (medium + 1) * size, secondArray * size);

    int i = 0, j = 0, k = left;

    while (i < firstArray && j < secondArray) {
        if (compare((char*)temporalLeft + i * size, (char*)temporalRight + j * size) <= 0) {
            memcpy((char*)arr + k * size, (char*)temporalLeft + i * size, size);
            i++;
        } else {
            memcpy((char*)arr + k * size, (char*)temporalRight + j * size, size);
            j++;
        }
        k++;
    }

    while (i < firstArray) {
        memcpy((char*)arr + k * size, (char*)temporalLeft + i * size, size);
        i++;
        k++;
    }

    while (j < secondArray) {
        memcpy((char*)arr + k * size, (char*)temporalRight + j * size, size);
        j++;
        k++;
    }

    free(temporalLeft);
    free(temporalRight);
}

void MergeSort(void* arr, int left, int right, size_t size, CompareFunc compare) {
    if (left < right) {
        int medium = left + (right - left) / 2;
        MergeSort(arr, left, medium, size, compare);
        MergeSort(arr, medium + 1, right, size, compare);
        Merge(arr, left, right, medium, size, compare);
    }
}

// Ejemplo de función de comparación para ordenar por ProductKey
int compareProductKey(const void* a, const void* b) {
    const ProductsData* prodA = (const ProductsData*)a;
    const ProductsData* prodB = (const ProductsData*)b;
    return (prodA->ProductKey - prodB->ProductKey);
}

void MainMergeSort(char nameFile[], char nameDest[], size_t size, CompareFunc compare) {
    FILE* fp = fopen(nameFile, "rb");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    fseek(fp, 0, SEEK_END);
    int numberOfRegisters = ftell(fp) / size;
    fseek(fp, 0, SEEK_SET);

    void* arr = malloc(numberOfRegisters * size);
    fread(arr, size, numberOfRegisters, fp);
    fclose(fp);

    MergeSort(arr, 0, numberOfRegisters - 1, size, compare);

    fp = fopen(nameDest, "wb");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        free(arr);
        return;
    }

    fwrite(arr, size, numberOfRegisters, fp);
    fclose(fp);
    free(arr);
}

int main() {
    // Ejemplo: ordenar ProductsData
    MainMergeSort("Products.table", "PruebaOrder.table", sizeof(ProductsData), compareProductKey);

    return 0;
}
