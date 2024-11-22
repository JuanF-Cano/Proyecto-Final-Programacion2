/*
- Fecha de publicación: 22/11/2024
- Hora de publicación: 2:52 AM
- Versión de su código: 5.2
- Autor. Ing(c) Juan Fernando Cano Duque
- Nombre del lenguaje utilizado: C
- Versión del lenguaje utilizado: C11
- Versión del compilador utilizado: MinGW.org GCC-6.3.0-1
- Versión del S.O sobre el que compilo y probo su código: Windows 11 Home Single Language, 23H2
- Presentado a: Doctor Ricardo Moreno Laverde 
- Universidad Tecnológica de Pereira 
- Programa de Ingeniería de Sistemas y Computación 
- Asignatura IS284 Programación II 

- Descrptivo: El programa recibe un dataset cn registros de una empresa y genera informes utilizando busquda binaria y ordenamiento
              de archivos por Bubble Sort y Merge Sort
    
- Salvedades:
  No se garantiza su funcionamiento en otras versiones del sistema operativo, en otros sistemas operativos, en otras versiones del 
  lenguaje, del compilador u otro compilador.
  Primero se debe ejecutar la opcion 1 del menu, creando las tablas, asi mismo, esta debe de ejecutarse antes de cualquier otra opcion
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

typedef struct {
    unsigned char MM;
    unsigned char DD;
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

typedef int (*Compare)(const void*, const void*);

void CreateSalesTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpSales = fopen("Sales.table", "wb");
    
    if (fp == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpSales == NULL) {
        printf("Error opening file: Sales.table\n");
        return;
    }
    int countRegisters = 0;
    int counterErrors = 0;
    SalesData data;
    char line[1024] = "";
    
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        data.OrderNumber = atol(token);

        token = strtok(NULL, ",");
        data.LineItem = (unsigned char)atoi(token);

        token = strtok(NULL, ",");
        sscanf(token, "%hhu/%hhu/%hu", &data.OrderDate.MM, &data.OrderDate.DD, &data.OrderDate.AAAA);

        token = strtok(NULL, ",");
        if (token[0] == ' ') {
            data.DeliveryDate.DD = 0;
            data.DeliveryDate.MM = 0;
            data.DeliveryDate.AAAA = 0;
        } else {
            sscanf(token, "%hhu/%hhu/%hu", &data.OrderDate.MM, &data.OrderDate.DD, &data.OrderDate.AAAA);
        }

        token = strtok(NULL, ",");
        data.CustomerKey = (unsigned int)atoi(token);
        
        token = strtok(NULL, ",");
        data.StoreKey = (unsigned short int)atoi(token);
        
        token = strtok(NULL, ",");
        data.ProductKey = (unsigned short int)atoi(token);
        
        token = strtok(NULL, ",");
        data.Quantity = (unsigned short int)atoi(token);

        token = strtok(NULL, ";");
        if (strlen(token) - 1 <= 3) {
            strcpy(data.CurrencyCode, token);
        } else {counterErrors++; printf("\nError (%i): Overflow CurrencyCode: %s", counterErrors, data.CurrencyCode);}

        if (fwrite(&data, sizeof(data), 1, fpSales) != 1) {
                printf("\nError writing to file. Order number: %li\n", data.OrderNumber);
            } else {
                countRegisters++;
            }
    }
    printf("\nSales Records entered: %i", countRegisters);
    fclose(fp);fclose(fpSales);
    return;
}

void CreateCustomersTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpCustomers = fopen("Customers.table", "wb");

    if (fp == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpCustomers == NULL) {
        printf("Error opening file: Sales.table\n");
        return;
    }

    int countRegisters = 0;
    int counterErrors = 0;
    CustomersData data;
    char line[250] = "";

    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ";");
        data.CustomersKey = (unsigned int)atoi(token);

        token = strtok(NULL, ";");
        if (strlen(token) <= 8) {
            strcpy(data.Gender, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Gender: %s", counterErrors, token);}

        token = strtok(NULL, ";");
        if (strlen(token) <= 40) {
            strcpy(data.Name, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Name: %s", counterErrors, token);}

        token = strtok(NULL, ";");
        if (strlen(token) <= 40) {
            strcpy(data.City, token);
        } else {counterErrors++; printf("\nError (%i): Overflow City: %s", counterErrors, token);}

        token = strtok(NULL, ";");
        if (strlen(token) <= 30) {
            strcpy(data.StateCode, token);
        } else {counterErrors++; printf("\nError (%i): Overflow StateCode: %s", counterErrors, token);}

        token = strtok(NULL, ";");
        if (strlen(token) <= 30) {
            strcpy(data.State, token);
        } else {counterErrors++; printf("\nError (%i): Overflow State: %s", counterErrors, token);}

        token = strtok(NULL, ";");
        if (isdigit((unsigned char)token[0])) {
            data.ZipCode = (unsigned int)atoi(token);
            token = strtok(NULL, ";");
            if (strlen(token) <= 20) {
                strcpy(data.Country, token);
            } else {counterErrors++; printf("\nError (%i): Overflow Country: %s", counterErrors, token);}
        } else {
            data.ZipCode = 0;
            if (strlen(token) <= 20) {
                strcpy(data.Country, token);
            } else {counterErrors++; printf("\nError (%i): Overflow Country: %s", counterErrors, token);}
        }

        token = strtok(NULL, ";");
        if (strlen(token) <= 20) {
            strcpy(data.Continent, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Continent: %s", counterErrors, token);}
            sscanf(strtok(NULL, ","), "%hhu/%hhu/%hu", &data.Birthday.MM, &data.Birthday.DD, &data.Birthday.AAAA);
        if (fwrite(&data, sizeof(data), 1, fpCustomers) != 1) {
            printf("\nError writing to file. Customers key: %i\n", data.CustomersKey);
        } else {
            countRegisters++;
        }
    }
    printf("\nCustomers Records entered: %i", countRegisters);
    fclose(fp); fclose(fpCustomers);
    return;
}

float parseToken(char **token) {
    if ((*token)[0] == '"') {
        *token = strtok(NULL, "\"");
    }
    char buffer[1024];
    int j = 0;
    for (int i = 0; (*token)[i] != '\0'; i++) {
        if ((*token)[i] != ',') {
            buffer[j++] = (*token)[i];
        }
    }
    buffer[j] = '\0';

    return (float)atof(buffer);
}

void CreateProductsTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpProducts = fopen("Products.table", "wb");

    if (fp == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpProducts == NULL) {
        printf("Error opening file: Sales.table\n");
        return;
    }

    int countRegisters = 0;
    int counterErrors = 0;
    ProductsData data;
    char line[1024] = "";
    
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        data.ProductKey = (unsigned short int)atoi(token);

        if (line[strlen(token) + 1] == '"' && (line[strlen(token) + 2] == 'P' || line[strlen(token) + 2] == 'M')) {
            token = strtok(NULL, "\"");
        } else {
            token = strtok(NULL, ",");
        }
        if (token[0] == '"') token++;
        if (strlen(token) <= 85) {
            strcpy(data.ProductName, token);
        } else {counterErrors++; printf("\nError (%i): Overflow ProductName: %s", counterErrors, token);}

        token = strtok(NULL, ",");
        if (strlen(token) <= 30) {
            strcpy(data.Brand, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Brand: %s", counterErrors, token);}

        token = strtok(NULL, ",");
        if (strlen(token) <= 15) {
            strcpy(data.Color, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Color: %s", counterErrors, token);}

        token = strtok(NULL, ",$");
        data.UnitCostUSD = parseToken(&token);
        
        token = strtok(NULL, ",$");
        data.UnitPriceUSD = parseToken(&token);

        token = strtok(NULL, ",");
        if (strlen(token) <= 4) {
            strcpy(data.SubcategoryKey, token);
        } else {counterErrors++; printf("\nError (%i): Overflow SubcategoryKey: %s", counterErrors, token);}

        token = strtok(NULL, "¬");
        if (token[0] == '"' || token[1] == '"') {
            token = strtok(token, "\"");
        } else {
            token = strtok(token, ",");
        }
        if (strlen(token) <= 35) {
            strcpy(data.Subcategory, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Subcategory: %s", counterErrors, token);}

        token = strtok(NULL, ",");
        if (strlen(token) <= 2 || (strlen(token) == 11 && token[2] == ' ')) {
            strcpy(data.CategoryKey, token);
        } else {counterErrors++; printf("\nError (%i): Overflow CategoryKey: %s", counterErrors, token);}

        token = strtok(NULL, "\"");
        if (strlen(token)  <= 30) {
            strcpy(data.Category, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Category: %s", counterErrors, token);}

        if (fwrite(&data, sizeof(data), 1, fpProducts) != 1) {
            printf("\nError writing to file. Product key: %i\n", data.ProductKey);
        } else {
            countRegisters++;
        }
    }
    printf("\nProducts Records entered: %i", countRegisters);
    fclose(fp);fclose(fpProducts);
    return;
}

void CreateStoresTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpStores = fopen("Stores.table", "wb");

    if (fp == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpStores == NULL) {
        printf("Error opening file: Sales.table\n");
        return;
    }

    int countRegisters = 0;
    int counterErrors = 0;
    StoresData data;
    char line[1024] = "";

    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        data.StoreKey = (unsigned short int)atoi(token);

        token = strtok(NULL, ",");
        if (strlen(token) <= 35) {
            strcpy(data.Country, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Country: %s", counterErrors, token);}

        token = strtok(NULL, ",");
        if (strlen(token) <= 35) {
            strcpy(data.State, token);
        } else {counterErrors++; printf("\nError (%i): Overflow State: %s", counterErrors, token);}

        token = strtok(NULL, ",");
        if (isdigit((unsigned char)token[strlen(token) - 5])) {
            data.SquareMeters = 0;
            sscanf(token, "%hhu/%hhu/%hu", &data.OpenDate.MM, &data.OpenDate.DD, &data.OpenDate.AAAA);
        } else {
            data.SquareMeters = (unsigned short int)atoi(token);
            token = strtok(NULL, ",");
            sscanf(token, "%hhu/%hhu/%hu", &data.OpenDate.MM, &data.OpenDate.DD, &data.OpenDate.AAAA);
        }
        
        if (fwrite(&data, sizeof(data), 1, fpStores) != 1) {
            printf("\nError writing to file. Store key: %i\n", data.StoreKey);
        } else {
            countRegisters++;
        }
    }
    printf("\nStores Records entered: %i", countRegisters);
    fclose(fp); fclose(fpStores);
    return;
}

void CreateExchangeRatesTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpExchangeRates = fopen("ExchangeRates.table", "wb");

    if (fp == NULL) {
        printf("Error opening file: %s\n", fileName);
        return;
    }
    if (fpExchangeRates == NULL) {
        printf("Error opening file: Sales.table\n");
        return;
    }

    int countRegisters = 0;
    int counterErrors = 0;
    ExchangeRates data;
    char line[1024] = "";
    
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (strlen(token) <= 10) {
            strcpy(data.Date, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Date: %s", counterErrors, token);}

        token = strtok(NULL, ",");
        if (strlen(token) <= 3 || (token[3] == ' ' && token[4] == ' ' && token[5] == ' ' && token[6] == ' '&& token[7] == ' ')) {
            strcpy(data.Currency, token);
        } else {counterErrors++; printf("\nError (%i): Overflow Currency: %s", counterErrors, token);}

        token = strtok(NULL, ",");
        data.Exchange = (float)atof(token);

        if (fwrite(&data, sizeof(data), 1, fpExchangeRates) != 1) {
            printf("\nError writing to file. Date: %s. Currency: %s\n", data.Date, data.Currency);
        } else {
            countRegisters++;
        }
    }
    printf("\nExchanges Rates Records entered: %i", countRegisters);
    fclose(fp); fclose(fpExchangeRates);
    return;
}


void Merge(FILE *fp, int left, int right, int medium, size_t size, Compare compare) {
    FILE *fpAux = fopen("auxiliar.table", "wb+");
    if (fpAux == NULL) {
        printf("Error opening file: auxiliar.\n");
        return;
    }
    int i = left, j = medium + 1;
    void *temporalLeft = malloc(size);
    void *temporalRight = malloc(size);

    while (i <= medium && j <= right) {
        fseek(fp, i * size ,SEEK_SET); fread(temporalLeft, size, 1, fp);
        fseek(fp, j * size ,SEEK_SET); fread(temporalRight, size, 1, fp);
        if (compare(temporalLeft, temporalRight) <= 0) {
            fwrite(temporalLeft, size, 1, fpAux); i++;
        } else {
            fwrite(temporalRight, size, 1, fpAux); j++;
        }
    }
    while (i <= medium) {
        fseek(fp, i * size, SEEK_SET);
        fread(temporalLeft, size, 1, fp);
        fwrite(temporalLeft, size, 1, fpAux); 
        i++;
    }
    while (j <= right) {
        fseek(fp, j * size, SEEK_SET);
        fread(temporalRight, size, 1, fp);
        fwrite(temporalRight, size, 1, fpAux); 
        j++;
    }

    free(temporalLeft);
    free(temporalRight);

    void *aux = malloc(size);
    fseek(fpAux, 0, SEEK_SET); fseek(fp, left * size, SEEK_SET); 
    while(fread(aux, size, 1, fpAux)) {
        fwrite(aux, size, 1, fp);
    }
    
    free(aux);
    fclose(fpAux); 
}

void Sort(FILE *fp, int left, int right, size_t size, Compare compare) {
    if (left < right) {
        int medium = left + (right - left) / 2;
        Sort(fp, left, medium, size, compare);
        Sort(fp, medium + 1, right, size, compare);
        Merge(fp, left, right, medium, size, compare);
    }
}

int MergeSort(char fileName[], char nameDest[], size_t size, Compare compare) {
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("Error opening file: %s\n", fileName);
        return 1;
    }
    FILE *fpDest = fopen(nameDest, "wb+");
    if (fpDest == NULL) {
        printf("Error opening file: %s\n", nameDest);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    int registers = ftell(fp) / size;
    fseek(fp, 0, SEEK_SET);
    void *record = malloc(size);
    while (fread(record, size, 1, fp)) {
        fwrite(record, size, 1, fpDest);
    }

    fseek(fpDest, 0, SEEK_SET);
    Sort(fpDest, 0, registers - 1, size, compare);
    
    fclose(fp); fclose(fpDest);
    remove("auxiliar.table");
    return registers;
}

void swap(FILE *fpDest, int pos1, int pos2, size_t size) {
    void *temp1 = malloc(size);
    void *temp2 = malloc(size);

    fseek(fpDest, pos1 * size, SEEK_SET);
    fread(temp1, size, 1, fpDest);
    fseek(fpDest, pos2 * size, SEEK_SET);
    fread(temp2, size, 1, fpDest);

    fseek(fpDest, pos1 * size, SEEK_SET);
    fwrite(temp2, size, 1, fpDest);
    fseek(fpDest, pos2 * size, SEEK_SET);
    fwrite(temp1, size, 1, fpDest);

    free(temp1);
    free(temp2);
}

int BubbleSort(char fileName[], char nameDest[], size_t size, Compare compare) {
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("Error opening file: %s", fileName);
        return 1;
    }
    FILE *fpDest = fopen(nameDest, "wb+");
    if (fp == NULL) {
        fclose(fp);
        printf("Error opening file: %s", nameDest);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    int registers = ftell(fp) / size;
    fseek(fp, 0, SEEK_SET); 
    void *record = malloc(size);
    while (fread(record, size, 1, fp)) {
        fwrite(record, size, 1, fpDest);
    }

    fseek(fpDest, 0, SEEK_SET);

    for (int i = 0; i < registers - 1; i++) {
        printf("%s: %i\n",fileName, i + 1);
        for (int j = 0; j < registers - i - 1; j++) {
            void *temporal1 = malloc(size);
            void *temporal2 = malloc(size);
            fseek(fpDest, j * size, SEEK_SET);
            fread(temporal1, size, 1, fpDest);
            fseek(fpDest, (j + 1) * size, SEEK_SET);
            fread(temporal2, size, 1, fpDest);
            if (compare(temporal1, temporal2) > 0) {
                swap(fpDest, j, j + 1, size);
            }
            free(temporal1);
            free(temporal2);
        }
    }

    fclose(fp); fclose(fpDest);
    
    return registers;
}

int CompareProductName(const void *product1, const void *product2) {
    const ProductsData *productA = (const ProductsData*)product1;
    const ProductsData *productB = (const ProductsData*)product2;
    return strcmp(productA->ProductName, productB->ProductName);
}

int CompareCustomerName(const void *customer1, const void *customer2) {
    const CustomersData *customerA = (const CustomersData*)customer1;
    const CustomersData *customerB = (const CustomersData*)customer2;
    return strcmp(customerA->Name, customerB->Name);
}

int CompareSaleByProductKey(const void *register1, const void *register2) {
    const SalesData *sale1 = (const SalesData*)register1;
    const SalesData *sale2 = (const SalesData*)register2;
    return (sale1->ProductKey > sale2->ProductKey) - (sale1->ProductKey < sale2->ProductKey);
}

int CompareSaleByCustomerKey(const void *register1, const void *register2) {
    const SalesData *sale1 = (const SalesData*)register1;
    const SalesData *sale2 = (const SalesData*)register2;
    return (sale1->CustomerKey > sale2->CustomerKey) - (sale1->CustomerKey < sale2->CustomerKey);
}

int CompareCustomerKey(const void *key1, const void *key2) {
    const CustomersData *customer1 = (const CustomersData*)key1;
    const CustomersData *customer2 = (const CustomersData*)key2;
    return (customer1->CustomersKey > customer2->CustomersKey) - (customer1->CustomersKey < customer2->CustomersKey);
}

int CompareProductKey(const void *key1, const void *key2) {
    const ProductsData *product1 = (const ProductsData*)key1;
    const ProductsData *product2 = (const ProductsData*)key2;
    return (product1->ProductKey > product2->ProductKey) - (product1->ProductKey < product2->ProductKey);
}

int CompareCustomerLocation(const void *record1, const void *record2) {
    const CustomersData *customer1 = (const CustomersData*)record1;
    const CustomersData *customer2 = (const CustomersData*)record2;
    int result = strcmp(customer1->Continent, customer2->Continent);
    if(result == 0) {
        result = strcmp(customer1->Country, customer2->Country);
        if(result == 0) {
            result = strcmp(customer1->State, customer2->State);
            if(result == 0) {
                result = strcmp(customer1->City, customer2->City);
            }
        }
    }
    return result;
}

int CompareSalesDate(const void *reg1, const void *reg2) {
    const SalesData *sale1 = (const SalesData*)reg1;
    const SalesData *sale2 = (const SalesData*)reg2;
    int result = sale1->OrderDate.AAAA - sale2->OrderDate.AAAA;
    if (result == 0) {
        result = sale1->OrderDate.MM - sale2->OrderDate.MM;
        if (result == 0) {
            result = sale1->OrderDate.DD - sale2->OrderDate.DD;
        }
    }
    return result;
}

int CompareSalesByOrderNumber(const void *a, const void *b){
    const SalesData *saleA = (const SalesData*)a;
    const SalesData *saleB = (const SalesData*)b;
    int comparation = saleA->OrderNumber - saleB->OrderNumber;
    if (comparation == 0) {
        comparation = (int)(saleA->ProductKey - saleB->ProductKey);
    }
    return comparation;
}

int BinarySearch(char fileName[], unsigned int item, size_t size, int registers, int option) {
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("Error opening file: %s\n", fileName);
        return -1;
    }
    int start = 0;
    int end = registers - 1;
    int medium = (registers - 1) / 2;
    void *record = malloc(size);
    int result = -1;

    while (start <= end) {
        medium = start + (end - start) / 2;
        fseek(fp, medium * size, SEEK_SET);
        fread(record, size, 1, fp);
        unsigned int key = 0;
        if (option == 1) {
            ProductsData *product = (ProductsData *)record;
            key = product->ProductKey;
        } else if (option == 2) {
            SalesData *sale = (SalesData *)record;
            key = sale->ProductKey;
        } else if (option == 3) {
            CustomersData *customer = (CustomersData *)record;
            key = customer->CustomersKey;
        } else if (option == 4) {
            SalesData *sale = (SalesData *)record;
            key = sale->CustomerKey;
        }
        if (key == item) {
            result = medium;
            start = end + 1;
        } else if (key > item) {
            end = medium - 1;
        } else {
            start = medium + 1;
        }
    }

    free(record);
    fclose(fp);
    return result;
}

int BinarySearchExchangeDate(char fileName[], SalesData recordSale) {
    unsigned int start = 0, middle = 0, end = 0, sizeOfRecord = sizeof(ExchangeRates);
    int result = 0;
    ExchangeRates record;
    FILE *fp = fopen(fileName, "rb");
    fseek(fp, 0 ,SEEK_END);
    end = ( ftell(fp) / sizeOfRecord ) - 1;
    while (start <= end) {
        middle = start + ((end - start) / 2);
        fseek(fp, middle * sizeOfRecord, SEEK_SET);
        fread(&record, sizeOfRecord, 1, fp);
        char key[11] = "";
        strcpy(key, record.Date);
        int currentMonth = 0, currentDay = 0, currentYear = 0;
        int month = 0, day = 0, year = 0;
        month = recordSale.OrderDate.MM;
        day = recordSale.OrderDate.DD;
        year = recordSale.OrderDate.AAAA;

        sscanf(key, "%d/%d/%d", &currentMonth, &currentDay, &currentYear);
        if(currentYear == year){
            if(currentMonth == month){
                if(currentDay == day){
                    result = middle;
                    start = end + 1;
                } else if(currentDay > day){
                    end = middle - 1;
                } else {
                    start = middle + 1;
                }
            } else if(currentMonth > month){
                end = middle - 1;
            } else {
                start = middle + 1;
            }
        } else if(currentYear > year){
            end = middle - 1;
        } else {
            start = middle + 1;
        }
    }
	return result;
}


void ShowDateTime() {
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);
    char formattedTime[50] = "";

    strftime(formattedTime, 50, "Valid to %Y-%b-%d at %H:%M hours", local_time);
    printf("%s\n", formattedTime);

}

void PrintReport2(int option) {
    clock_t start = clock();
    int salesRegisters = 0;
    int customersRegisters = 0;
    if (option == 1) {
        BubbleSort("Products.table", "ProductsOrder.table", sizeof(ProductsData), CompareProductName);
        salesRegisters = BubbleSort("Sales.table", "SalesOrder.table", sizeof(SalesData), CompareSaleByProductKey);
        customersRegisters = BubbleSort("Customers.table", "CustomersOrder.table", sizeof(CustomersData), CompareCustomerKey);
    } else if (option == 2) {
        MergeSort("Products.table", "ProductsOrder.table", sizeof(ProductsData), CompareProductName);
        salesRegisters = MergeSort("Sales.table", "SalesOrder.table", sizeof(SalesData), CompareSaleByProductKey);
        customersRegisters = MergeSort("Customers.table", "CustomersOrder.table", sizeof(CustomersData), CompareCustomerKey);
    }

    FILE *fpProducts = fopen("ProductsOrder.table", "rb");
    if (fpProducts == NULL) {
        printf("Error opening file: ProductsOrder.table.\n");
        return;
    }
    FILE *fpSales = fopen("SalesOrder.table", "rb");
    if (fpSales == NULL) {
        printf("Error opening file: SalesOrder.table.\n");
        fclose(fpProducts);
        return;
    }
    FILE *fpCustomers = fopen("CustomersOrder.table", "rb");
    if (fpCustomers == NULL) {
        printf("Error opening file: CustomersOrder.table.\n");
        fclose(fpProducts);
        fclose(fpSales);
        return;
    }
    ProductsData product;
    SalesData sale;
    CustomersData customer;
    int result = 0;
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Company Global Electronics Retailer\nValid to");
    ShowDateTime();
    printf("\nProducts list ordered by ProductName + Continent + Country + Sate + City\n");

    while (fread(&product, sizeof(ProductsData), 1, fpProducts)) {
        FILE *fpAux = fopen("CustomersAux.table", "wb");
        if (fpAux == NULL) {
            printf("Error opening file: CustomersAux.table.\n");
            fclose(fpProducts);
            fclose(fpSales);
            fclose(fpCustomers);
            return;
        }

        printf("ProductName: %s", product.ProductName);
        result = BinarySearch("SalesOrder.table", product.ProductKey, sizeof(SalesData), salesRegisters, 2);
        if (result == -1) {
            printf(" - No sales reported\n");
        } else {
            printf("\n%-22s %-22s %-32s %-42s\n", "Continent", "Country", "State", "City");
            printf("________________________________________________________________________________________________________________________________\n");

            fseek(fpSales, result * sizeof(SalesData), SEEK_SET);
            while (fread(&sale, sizeof(SalesData), 1, fpSales) == 1 && sale.ProductKey == product.ProductKey) {
                int customerPos = BinarySearch("CustomersOrder.table", sale.CustomerKey, sizeof(CustomersData), customersRegisters, 3);
                if (customerPos != -1) {
                    fseek(fpCustomers, customerPos * sizeof(CustomersData), SEEK_SET);
                    fread(&customer, sizeof(CustomersData), 1, fpCustomers);
                    fwrite(&customer, sizeof(CustomersData), 1, fpAux);
                }
            }
            fclose(fpAux);
            if (option == 1) {
                BubbleSort("CustomersAux.table", "CustomersAuxOrder.table", sizeof(CustomersData), CompareCustomerLocation);
            } else if (option == 2) {
                MergeSort("CustomersAux.table", "CustomersAuxOrder.table", sizeof(CustomersData), CompareCustomerLocation);
            }
            FILE *fpOrder = fopen("CustomersAuxOrder.table", "rb");
            if (fpOrder != NULL) {
                while (fread(&customer, sizeof(CustomersData), 1, fpOrder) == 1) {
                    printf("%-22s %-22s %-32s %-42s\n", customer.Continent, customer.Country, customer.State, customer.City);
                }
                fclose(fpOrder);
            } else {
                printf("Error opening file: CustomersAuxOrder.table.\n");
            }
            printf("________________________________________________________________________________________________________________________________\n\n");
        }
    }
    remove("CustomersAux.table");
    remove("CustomersAuxOrder.table");

    clock_t end = clock();
    double totalTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    int minutes = (int)(totalTime / 60);
    int seconds = (int)(totalTime) % 60;
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Time used to produce this listing: %d' %d\"\n", minutes, seconds);
    printf("***************************LAST LINE OF THE REPORT***************************\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    fclose(fpProducts);
    fclose(fpSales);
    fclose(fpCustomers);
}

void Graph(char screen[24][56], float array[12]){
    float arrAux[12] = {0.0};
    for (int i = 0; i < 12; i++) {
        arrAux[i] = array[i];
    }
    double yMax = arrAux[0]; 
    if((int)yMax / 1000000 >= 1){
        for(int i = 0; i < 12; i++){
            arrAux[i] = arrAux[i] / 1000000;
        }
        yMax = arrAux[0]; 
    }
    
    for(int i = 0; i < 12; i++){
        if(arrAux[i] > yMax){
            yMax = arrAux[i];
        }
    }
    double scaleY = 24.0 / fabs(yMax);

    for(int i = 0; i < 24; i++){
        screen[i][9] = 179;
    }
    for(int i = 9; i < 56; i++){
        if(i != 9){
            screen[23][i] = 196;
        }else{
            screen[23][i] = 192;
        }
    }
    for(int i = 0; i < 12; i++){
        int placeY = (int) 24.0 - round(scaleY * arrAux[i]);
        char str[12] = "";
        sprintf(str, "%.2f", arrAux[i]);
        int length = strlen(str);
        for(int j = 0; j < length; j++){
                screen[placeY][j] = str[j];
        }
        screen[placeY][11 + (4 * i)] = 'x';
    }
    return;
}

float GetProductPrice(unsigned short int productKey, int registers) {
    FILE *fp = fopen("ProductsOrder.table", "rb");
    if (fp == NULL) {
        printf("Error opening file: ProductsOrder.table\n");
        return 1.0;
    }
    ProductsData product;
    fseek(fp, BinarySearch("ProductsOrder.table", productKey, sizeof(ProductsData), registers, 1) * sizeof(ProductsData), SEEK_SET);
    fread(&product, sizeof(ProductsData), 1, fp);
    fclose(fp);
    return product.UnitPriceUSD;
}

void PrintSeasonalAnalysis(int option) {
    clock_t start = clock();
    int productsRegisters = 0;
    if (option == 1) {
        productsRegisters = BubbleSort("Products.table", "ProductsOrder.table", sizeof(ProductsData), CompareProductKey);
        BubbleSort("Sales.table", "SalesOrder.table", sizeof(SalesData), CompareSalesDate);
    } else if (option == 2)  {
        productsRegisters = MergeSort("Products.table", "ProductsOrder.table", sizeof(ProductsData), CompareProductKey);
        MergeSort("Sales.table", "SalesOrder.table", sizeof(SalesData), CompareSalesDate);
    }
    SalesData sale;
    float monthlyOrders[12] = {0.0};
    float monthlyIncome[12] = {0.0};
    FILE *fpSales = fopen("SalesOrder.table", "rb");
    if (fpSales == NULL) {
        printf("Error opening file: Sales.table\n");
        return;
    }
    SalesData startDate, finalDate;
    int yearsAnalyzed = 0;
    fread(&sale, sizeof(SalesData), 1, fpSales);
    startDate = sale;
    fseek(fpSales, -1 * sizeof(SalesData), SEEK_END);
    fread(&sale, sizeof(SalesData), 1, fpSales);
    finalDate = sale;
    fseek(fpSales, 0, SEEK_SET);
    yearsAnalyzed = finalDate.OrderDate.AAAA - startDate.OrderDate.AAAA + 1;

    int monthIndex = 0;
    while (fread(&sale, sizeof(SalesData), 1, fpSales)) {
        monthIndex = sale.OrderDate.MM - 1;
        monthlyOrders[monthIndex] += sale.Quantity;
        monthlyIncome[monthIndex] += sale.Quantity * GetProductPrice(sale.ProductKey, productsRegisters);
    }
    fclose(fpSales);

    float averageMonthlyOrders[12];
    float averageMonthlyIncome[12];
    for (int i = 0; i < 12; i++) {
        averageMonthlyOrders[i] = monthlyOrders[i] / yearsAnalyzed;
        averageMonthlyIncome[i] = monthlyIncome[i] / yearsAnalyzed;
    }

    char screen[24][56] = {{}};
    memset(screen, ' ', sizeof(screen));
    char meses[] = " ene feb mar abr may jun jul ago sep oct nov dic";

    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Company Global Electronics Retailer\n");
    printf("Valid to ");
    ShowDateTime();
    printf("Title: Analysis of Seasonal Patterns in Orders and Income for Company Global Electronics Retailer\n");
    printf("This report aims to analyze whether there are seasonal patterns or trends in order volume and Income.\n");
    printf("Methodology: Data collection: Historical order and revenue data is collected, with an appropriate time range to identify seasonal patterns.\n");
    printf("Data preparation: The data will be organized to facilitate analysis.\n\n");
    printf("Order Volume and Monthly income Analysis\nWhen examining the order volume and monthly income data, a direct relationship between the two is observed, making it easy to identify the increases and decreases in both income and the number of orders. This analysis is especially useful when visualizing these patterns through charts and tables, facilitating the understanding of the peaks and drops that affect both sales volume and income. Additionally, there is a noticeable stability in certain months, highlighting the seasonality of the business.");
    printf("Order and income Peaks\nFebruary and December stand out with the highest order volumes (%.0f and %.0f orders) and incomes ($%.2f and $%.2f), due to year-end promotions and January sales.\n", monthlyOrders[1], monthlyOrders[11], monthlyIncome[1], monthlyIncome[11]);
    printf("Months with Fewer Orders and income\nIn March and April, both orders and income drop significantly, with %.0f and %.0f orders respectively. This is due to lower demand after the holidays and the lack of promotions or incentives.\n", monthlyOrders[2], monthlyOrders[3]);
    printf("Stability in the Intermediate Months\nMay, June, and July show steady demand, with 4,000 to 5,000 orders per month, suggesting that activity remains balanced during these intermediate months.\n");
    printf("Monthly Averages\nThe average order volumes follow the same seasonal trend, with February and December reaching the highest peaks, while March and April show the lowest values, confirming the post-holiday drop.\n\n");
    printf("Results:\nChart 1: Order volume per month from %i/%i/%i to %i/%i/%i\n", startDate.OrderDate.MM, startDate.OrderDate.DD, startDate.OrderDate.AAAA, finalDate.OrderDate.MM, finalDate.OrderDate.DD, finalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Order Volume\n-------------------------\t");
    Graph(screen, monthlyOrders);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.0f\t\t|\t", (i / 2) + 1, monthlyOrders[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 2: Income per month from %i/%i/%i to %i/%i/%i\n", startDate.OrderDate.MM, startDate.OrderDate.DD, startDate.OrderDate.AAAA, finalDate.OrderDate.MM, finalDate.OrderDate.DD, finalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Total Income\t|\t\tMillions USD\n-------------------------\t");
    memset(screen, ' ', sizeof(screen));
    Graph(screen, monthlyIncome);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.2f\t|\t", (i / 2) + 1, monthlyIncome[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 3: Monthly average order volume from %i/%i/%i to %i/%i/%i\n", startDate.OrderDate.MM, startDate.OrderDate.DD, startDate.OrderDate.AAAA, finalDate.OrderDate.MM, finalDate.OrderDate.DD, finalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Monthly Average|\n-------------------------\t");
    memset(screen, ' ', sizeof(screen));
    Graph(screen, averageMonthlyOrders);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.0f\t\t|\t", (i / 2) + 1, averageMonthlyOrders[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\n\nChart 4: Monthly average income from %i/%i/%i to %i/%i/%i\n", startDate.OrderDate.MM, startDate.OrderDate.DD, startDate.OrderDate.AAAA, finalDate.OrderDate.MM, finalDate.OrderDate.DD, finalDate.OrderDate.AAAA);
    printf("-------------------------\n|Month\t|Total Income\t|\t\tMillions USD\n-------------------------\t");
    memset(screen, ' ', sizeof(screen));
    Graph(screen, averageMonthlyIncome);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 56; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
        if (i % 2 == 0) {
            printf("|%d\t|%.2f\t|\t", (i / 2) + 1, averageMonthlyIncome[(i / 2)]);
        } else if (i == 23) {
            printf("-------------------------\t         %s", meses);
        } else {
            printf("-------------------------\t");
        }
    }
    printf("\nConclusions:\nClear seasonal patterns show sales peaks in December and February, followed by drops in March and April.\n");
    printf("It is recommended to take advantage of demand peaks with marketing strategies and promotions, while managing the low-demand months with offers and discounts to maintain income flow.\n");
    printf("\nRecommendations:\nStrengthen marketing efforts during low-demand months (March and April) with promotions and discounts.\nIncrease inventory during high-demand months (November, December, and January) to meet the sales peaks.\n");
    printf("Reduce inventory during months of lower activity (March and April) to avoid overstocking and additional costs.\nBuild customer loyalty with reward programs that encourage purchases in low months\n");
    clock_t end = clock();
    double totalTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    int minutes = (int)(totalTime / 60);
    int seconds = (int)(totalTime) % 60;
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Time used to produce this listing: %d' %d\"\n", minutes, seconds);
    printf("***************************LAST LINE OF THE REPORT***************************\n");
}

void PrintDeliveryTimeAnalysis() {
    clock_t start = clock();
    int salesRegisters = 0;
    FILE *fpSales = fopen("SalesOrder.Table", "rb");
    fseek(fpSales, 0, SEEK_END);
    salesRegisters = ftell(fpSales) / sizeof(SalesData);
    fseek(fpSales, 0, SEEK_SET);
    FILE *fpAux = fopen("auxiliarSales", "wb+");
    SalesData sale;
    int deliveredSales = 0;
    int averageDeliveryTime[6][2] = {0};

    for (int i = 0, temporalIndex = 0; i < salesRegisters; i += 1) {
        fseek(fpSales, i * sizeof(SalesData), SEEK_SET);
        fread(&sale, sizeof(SalesData), 1, fpSales);

        if (sale.StoreKey == 0 && sale.DeliveryDate.AAAA != 0 && sale.DeliveryDate.MM != 0 && sale.DeliveryDate.DD != 0) {
            fseek(fpAux, sizeof(SalesData) * temporalIndex, SEEK_SET);
            fwrite(&sale, sizeof(SalesData), 1, fpAux);
            temporalIndex++;
            deliveredSales++;
        }
    }

    for (int i = 0; i < deliveredSales; i += 1) {
        fseek(fpAux, sizeof(SalesData) * i, SEEK_SET);
        fread(&sale, sizeof(SalesData), 1, fpAux);

        if (sale.DeliveryDate.AAAA != 0 && sale.DeliveryDate.MM != 0 && sale.DeliveryDate.DD != 0) {
            int year = (int)sale.DeliveryDate.AAAA % 2016;
            long int orderDate = 0, deliveryDate = 0, deliveryTimeInDays = 0;
            orderDate = sale.OrderDate.AAAA * 360 + sale.OrderDate.MM * 30 + sale.OrderDate.DD;
            deliveryDate = sale.DeliveryDate.AAAA * 360 + sale.DeliveryDate.MM * 30 + sale.DeliveryDate.DD;
            deliveryTimeInDays = deliveryDate - orderDate;
            averageDeliveryTime[year][0] += deliveryTimeInDays;
            averageDeliveryTime[year][1] += 1;
        
        } else {
            printf("\t\t\t\t\tOrderNumber: %li has an invalid DeliveryDate.\n", sale.OrderNumber);
        }
    }

    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Company Global Electronics Retailer\nValid to ");
    ShowDateTime();
    printf("\nTitle: Analysis of delivery time in days for Company Global Electronics Retailer\n");
    printf("\nOver the years analyzed, a progressive decrease in delivery times is observed, suggesting that both\nlocal and online stores have managed to improve their delivery efficiency over time.\n\n");
    printf("Once the data was sorted by delivery date, the average delivery time in days was calculated, which\nrefers to the average number of days between the order date and the delivery date. This calculation\nis crucial to understand the efficiency of delivery times based on the provided data.\n\n");
    printf("It is important to note that, in the analyzed dataset, there are no records of physical stores with a\ndelivery date. Therefore, the comparison was made only between online store that\ndo have valid delivery date records.\n\n");
    printf("Results:\n");
    printf("After analyzing the data, the following results were obtained for the average delivery time in days:\n\n");
    for(int i = 0; i < 6; i += 1){
        printf("\n\tAverage delivery time in: %i:\t%.2f days\n", i + 2016, (1.0 * averageDeliveryTime[i][0]) / (1.0 * averageDeliveryTime[i][1]));
    }
    printf("\n\nOver the years analyzed, a progressive decrease in delivery times is observed, suggesting that \nthe online store has managed to improve its delivery efficiency over time.\n\n");
    printf("Conclusion:\n\n");
    printf("In the provided dataset, there are no records for physical stores with delivery dates, so the analysis\nhas been limited to online stores. The calculation of average delivery time shows a trend of\nimprovement over the years, with a significant reduction in average delivery time, from 7.25 days in\n2016 to 3.76 days in 2021.\n\n");
    printf("This analysis highlights how, over time, delivery processes have improved, which may reflect\noptimization in logistical systems or operational efficiency in both online and local stores.\n\n");
    fclose(fpAux);
    fclose(fpSales);
    clock_t end = clock();
    double totalTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    int minutes = (int)(totalTime / 60);
    int seconds = (int)(totalTime) % 60;
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Time used to produce this listing: %d' %d\"\n", minutes, seconds);
    printf("***************************LAST LINE OF THE REPORT***************************\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");

}

void PrintReport5(int typeofSort){
    clock_t start = clock();
    int customersRegisters = 15265, salesRegisters = 62884, productsRegisters = 2517;
    /* if (typeofSort == 1) {
        customersRegisters = BubbleSort("Customers.table", "CustomersOrder.table", sizeof(CustomersData), CompareCustomerName);
        productsRegisters = BubbleSort("Products.table", "ProductsOrder.table", sizeof(ProductsData), CompareProductKey);
        salesRegisters = BubbleSort("Sales.table", "SalesOrder.table", sizeof(SalesData), CompareSaleByCustomerKey);
    } else if (typeofSort == 2) {
        customersRegisters = MergeSort("Customers.table", "CustomersOrder.table", sizeof(CustomersData), CompareCustomerName);
        productsRegisters = MergeSort("Products.table", "ProductsOrder.table", sizeof(ProductsData), CompareProductKey);
        salesRegisters = MergeSort("Sales.table", "SalesOrder.table", sizeof(SalesData), CompareSaleByCustomerKey);
    } */

    printf("\n---------------------------------------------------------------------------------------");
    printf("\nCompany Global Electronics Retailer\n");
    printf("Customer list ordered by Customer name + order date for sale + ProductKey\n");
    FILE *fpSales = fopen("SalesOrder.table", "rb");
    FILE *fpCustomers = fopen("CustomersOrder.table", "rb");
    FILE *fpProducts = fopen("ProductsOrder.table", "rb");
	SalesData recordSale;				//Used to store a record of SalesTable and get its information
	CustomersData recordCustomer;		//Used to store a record of CustomersTable and store it temporarely in TemporalFileOption2
	ExchangeRates recordExchange; 
	
	FILE *fpExchangeRates = NULL;
	FILE *fpTemporalSales = NULL;

    char customerName[40] = ""; 		//Used to store the ProductName of each Product in ProductsTable
	unsigned int customerKey = 0; 						//Used to store the ProductKey of each Product in ProductTable

    for(int i = 0; i < customersRegisters && i < 100; i++){
        fseek(fpCustomers, sizeof(CustomersData) * i, SEEK_SET);
    	fread(&recordCustomer, sizeof(CustomersData), 1, fpCustomers);

        strcpy(customerName, recordCustomer.Name);
        printf("\nCustomer name: %-40s", customerName);

        customerKey = recordCustomer.CustomersKey;
        int positionSales = BinarySearch("SalesOrder.table", customerKey, sizeof(SalesData), salesRegisters, 4);
        if(positionSales == -1){
            printf(" - No purchases reported\n");
        } else {
			fpTemporalSales = fopen("TemporalFileSalesOption5", "wb+");
			fpExchangeRates = fopen("ExchangeRates.tables", "wb+");

			if(fpTemporalSales == NULL){
				printf("Error abiendo el temporal de sales\n");
				return;
			}
			if(fpExchangeRates == NULL){
				printf("Error abriendo el archivo de exchange\n");
				return;
			}

            fseek(fpSales, (positionSales - 1) * sizeof(SalesData), SEEK_SET);
            fread(&recordSale, sizeof(SalesData), 1, fpSales);

            for( int i = positionSales - 1; i >= 0 && customerKey == recordSale.CustomerKey; i -= 1){
				//Reading of the previous record in sales to verify if its the first one with the currenr CustomerKey
    	    	fseek(fpSales, sizeof(SalesData) * (i - 1), SEEK_SET);
    	    	fread(&recordSale, sizeof(SalesData), 1, fpSales);

    	    	positionSales = i; //Changing positionSales to be the index of first record in sales with the current CustomerKey
    	    }
            fseek(fpSales, positionSales * sizeof(SalesData), SEEK_SET);
            fread(&recordSale, sizeof(SalesData), 1, fpSales);

            int numOfOrders = 0, totalNumOfPurchases = 0;
			long orderNumber = recordSale.OrderNumber;
            for(int index = positionSales, i = 0; recordSale.CustomerKey == customerKey && index < salesRegisters; index++, i++) {
				//printf("\norder number: %li, order date: %hu/%d/%d, productKey: %d, quantity: %d, currency code: %s\n", recordSale.OrderNumber, recordSale.OrderDate.AAAA, recordSale.OrderDate.MM, recordSale.OrderDate.DD, recordSale.ProductKey, recordSale.Quantity, recordSale.CurrencyCode);
                fseek(fpTemporalSales, i * sizeof(SalesData), SEEK_SET);
				fwrite(&recordSale, sizeof(SalesData), 1, fpTemporalSales);
				totalNumOfPurchases++;
                fseek(fpSales, (index + 1) * sizeof(SalesData), SEEK_SET);
                fread(&recordSale, sizeof(SalesData), 1, fpSales);
				if(orderNumber != recordSale.OrderNumber){
					numOfOrders++;
					orderNumber = recordSale.OrderNumber;
				}
            }


			if(typeofSort == 1){
				BubbleSort("TemporalFileSalesOption5", "TemporalSalesOrder", sizeof(SalesData), CompareSalesByOrderNumber);
			} else if(typeofSort == 2){
				MergeSort("TemporalFileSalesOption5", "TemporalSalesOrder", sizeof(SalesData), CompareSalesByOrderNumber);
			}			
			SalesData tempRecordSale1, tempRecordSale2;

			fseek(fpSales, positionSales * sizeof(SalesData), SEEK_SET);
            fread(&recordSale, sizeof(SalesData), 1, fpSales);
			
			int orderIndex = 0; 			// Inicializar el índice de órdenes
			float totalValue = 0.0;      	// Total acumulado para todas las órdenes

			for (int order = 0; order < numOfOrders; order++) {
			    // Leer el primer registro de la orden actual
			    fseek(fpTemporalSales, sizeof(SalesData) * orderIndex, SEEK_SET);
			    fread(&tempRecordSale1, sizeof(SalesData), 1, fpTemporalSales);

			    // Leer el siguiente registro para comparación
			    fseek(fpTemporalSales, sizeof(SalesData) * (orderIndex + 1), SEEK_SET);
			    fread(&tempRecordSale2, sizeof(SalesData), 1, fpTemporalSales);

			    float exchange = -1.0;

				// Buscar el tipo de cambio para esta orden
				int positionExchange = BinarySearchExchangeDate("ExchangeRates.table", tempRecordSale1);
				if (positionExchange != -1){
					ExchangeRates staticExchangeRecord;
					fseek(fpExchangeRates, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
					fread(&staticExchangeRecord, sizeof(ExchangeRates), 1, fpExchangeRates);
					int index = positionExchange;
					if (strcmp("USD", staticExchangeRecord.Currency) != 0){
						fseek(fpExchangeRates, sizeof(ExchangeRates) * positionExchange, SEEK_SET);
						fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						for (; strcmp("USD", recordExchange.Currency) != 0 && index > 0; index--){
							fseek(fpExchangeRates, sizeof(ExchangeRates) * index, SEEK_SET);
							fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						}
						index++;
					}

					FILE *fpTemporalExchange = tmpfile();
					for (int i = 0; i < 5; i++, index++){
						fseek(fpExchangeRates, sizeof(ExchangeRates) * index, SEEK_SET);
						fread(&recordExchange, sizeof(ExchangeRates), 1, fpExchangeRates);
						fseek(fpTemporalExchange, sizeof(ExchangeRates) * i, SEEK_SET);
						fwrite(&recordExchange, sizeof(ExchangeRates), 1, fpTemporalExchange);
					}

					int indexTemoralExchange = -1;
					if (tempRecordSale1.CurrencyCode[0] == 'U'){
						indexTemoralExchange = 0;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'C'){
						indexTemoralExchange = 1;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'A'){
						indexTemoralExchange = 2;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'E'){
						indexTemoralExchange = 3;
					}
					else if (tempRecordSale1.CurrencyCode[0] == 'G'){
						indexTemoralExchange = 4;
					}
					//printf ("\n\t indextemporal : %i" , indexTemoralExchange);
					fseek(fpTemporalExchange, sizeof(ExchangeRates) * indexTemoralExchange, SEEK_SET);
					fread(&recordExchange, sizeof(ExchangeRates), 1, fpTemporalExchange);
                
					exchange = recordExchange.Exchange;
                   
					fclose(fpTemporalExchange);
				}

			    // Mostrar encabezado de la orden
			    printf("\nOrder date: %hu/%u/%u\tOrder Number: %li\n", 
			           tempRecordSale1.OrderDate.AAAA, tempRecordSale1.OrderDate.MM, tempRecordSale1.OrderDate.DD, 
			           tempRecordSale1.OrderNumber);
				printf("%-17s%-90s%-15s%s %s", "ProductKey", "ProductName", "Quantity", "Value", tempRecordSale1.CurrencyCode);
			    printf("\n_________________________________________________________________________________________________________________________________________________________\n");

			    float subTotal = 0.0;
			    ProductsData tempProductRecord;

			    // Iterar sobre todos los productos de la misma orden
			    while (orderIndex < totalNumOfPurchases - 1 && tempRecordSale1.OrderNumber == tempRecordSale2.OrderNumber) {
			        unsigned int productKey = tempRecordSale1.ProductKey;

			        // Buscar el producto en el archivo de productos
			        int positionProducts = BinarySearch("ProductsOrder.table", productKey, sizeof(ProductsData), productsRegisters, 1);
			        if (positionProducts != -1) {
			            fseek(fpProducts, sizeof(ProductsData) * positionProducts, SEEK_SET);
			            fread(&tempProductRecord, sizeof(ProductsData), 1, fpProducts);

			            // Calcular el valor del producto
			            float price = tempProductRecord.UnitPriceUSD * exchange * tempRecordSale1.Quantity;
			            printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.ProductKey, tempProductRecord.ProductName, tempRecordSale1.Quantity, price);
			            subTotal += price;
			        }

			        // Avanzar al siguiente registro
			        orderIndex++;

			        // Actualizar los datos para la próxima iteración
			        fseek(fpTemporalSales, sizeof(SalesData) * orderIndex, SEEK_SET);
			        fread(&tempRecordSale1, sizeof(SalesData), 1, fpTemporalSales);

			        fseek(fpTemporalSales, sizeof(SalesData) * (orderIndex + 1), SEEK_SET);
			        fread(&tempRecordSale2, sizeof(SalesData), 1, fpTemporalSales);
			    }

			    // Procesar el último producto de la orden
			    unsigned int productKey = tempRecordSale1.ProductKey;

			    int positionProducts = BinarySearch("ProductsOrder.table", productKey, sizeof(ProductsData), productsRegisters, 1);
			    if (positionProducts != -1) {
			        fseek(fpProducts, sizeof(ProductsData) * positionProducts, SEEK_SET);
			        fread(&tempProductRecord, sizeof(ProductsData), 1, fpProducts);

			        float price = tempProductRecord.UnitPriceUSD * exchange * tempRecordSale1.Quantity;
			        printf("%-17hu%-100s%-15hu%.2f\n", tempProductRecord.ProductKey, tempProductRecord.ProductName, tempRecordSale1.Quantity, price);
			        subTotal += price;
			    }

			    orderIndex++; // Avanzar al siguiente índice
			    printf("_________________________________________________________________________________________________________________________________________________________\n");
				printf("%-117s%-15s%.2lf\n", "", "Subtotal", subTotal);
			    totalValue += subTotal;
			}

			// Mostrar el total acumulado
			printf("%-117s%-15s%.2lf\n", "", "TOTAL", totalValue);

		}
	}
    clock_t end = clock();
    double totalTime = ((double)(end - start)) / CLOCKS_PER_SEC;
    int minutes = (int)(totalTime / 60);
    int seconds = (int)(totalTime) % 60;
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    printf("Time used to produce this listing: %d' %d\"\n", minutes, seconds);
    printf("***************************LAST LINE OF THE REPORT***************************\n");
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    fclose(fpTemporalSales); fclose(fpExchangeRates);
}

int main() {
    float option = 0.0; 
    do {
        printf("\n\nCompany Global Electronics Retailer\nOptions menu\n");
        printf("0. Exit program\n1. Construction of the Database with the dataset tables\n");
        printf("2. List of %cWhat types of products does the company sell, and where are customers located?\n\t2.1 Utility bubbleSort\n\t2.2 Utility mergeSort\n", 168);
        printf("3. List of %cAre there any seasonal patterns or trends for order volume or Income?\n\t3.1 Utility bubbleSort\n\t3.2 Utility mergeSort\n", 168);
        printf("4. List of %cHow long is the average delivery time in days? Has that changed over time?\n\t4.1 Utility bubbleSort\n\t4.2 Utility mergeSort\n", 168);
        printf("5. List of sales order by \"Customer Name\"+\"Order Date\"+\"ProductKey\";\n\t5.1 Utility bubbleSort\n\t5.2 Utility mergeSort\nWhat is your option: ");
        scanf("%f", &option);
        option = (option * 10);

        if (option == 10) {
            CreateSalesTable("Sales.dat");
            CreateCustomersTable("Customers.dat");
            CreateStoresTable("Stores.dat");
            CreateExchangeRatesTable("Exchange_Rates.dat");
            CreateProductsTable("Products.dat");
        } else if (option == 21) {
            PrintReport2(1);
        } else if (option == 22) {
            PrintReport2(2);
        } else if (option == 31) {
            PrintSeasonalAnalysis(1);  
        } else if (option == 32) {
            PrintSeasonalAnalysis(2);  
        } else if (option == 41 || option == 42) {
            PrintDeliveryTimeAnalysis();
        } else if (option == 51) {
            PrintReport5(1);
        } else if (option == 52) {
            PrintReport5(2);
        } else if (option != 0) {
            printf("\nInvalid option\n");
        } 
    } while (option != 0);

    return 0;
}

/* 

Ordena:
    Customers, customer name.
    Sales, customer key.
    Products, procduct key.

FILE TemporalFileSalesOption5 
 */