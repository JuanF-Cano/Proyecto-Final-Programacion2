#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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

typedef int (*Compare)(const void*, const void*);

void CreateSalesTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpSales = fopen("Sales.table", "ab");
    
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
        sscanf(token, "%hhu/%hhu/%hu", &data.OrderDate.DD, &data.OrderDate.MM, &data.OrderDate.AAAA);

        token = strtok(NULL, ",");
        if (token[0] == ' ') {
            data.DeliveryDate.DD = 0;
            data.DeliveryDate.MM = 0;
            data.DeliveryDate.AAAA = 0;
        } else {
            sscanf(token, "%hhu/%hhu/%hu", &data.DeliveryDate.DD, &data.DeliveryDate.MM, &data.DeliveryDate.AAAA);
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
    printf("\nRecords entered: %i", countRegisters);
    fclose(fp);fclose(fpSales);
    return;
}

void CreateCustomersTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpCustomers = fopen("Customers.table", "ab");

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

        sscanf(strtok(NULL, ","), "%hhu/%hhu/%hu", &data.Birthday.DD, &data.Birthday.MM, &data.Birthday.AAAA);

        if (fwrite(&data, sizeof(data), 1, fpCustomers) != 1) {
            printf("\nError writing to file. Customers key: %i\n", data.CustomersKey);
        } else {
            countRegisters++;
        }
    }
    printf("\nRecords entered: %i", countRegisters);
    fclose(fp); fclose(fpCustomers);
    return;
}

float parseToken(char **token) {
    if (*token[0] == '"') {
        *token = strtok(NULL, "\"");
    }
    return (float)atof(*token);
}

void CreateProductsTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpProducts = fopen("Products.table", "ab");

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
    printf("\nRecords entered: %i", countRegisters);
    fclose(fp);fclose(fpProducts);
    return;
}

void CreateStoresTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpStores = fopen("Stores.table", "ab");

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
            sscanf(token, "%hhu/%hhu/%hu", &data.OpenDate.DD, &data.OpenDate.MM, &data.OpenDate.AAAA);
        } else {
            data.SquareMeters = (unsigned short int)atoi(token);
            token = strtok(NULL, ",");
            sscanf(token, "%hhu/%hhu/%hu", &data.OpenDate.DD, &data.OpenDate.MM, &data.OpenDate.AAAA);
        }
        
        if (fwrite(&data, sizeof(data), 1, fpStores) != 1) {
            printf("\nError writing to file. Store key: %i\n", data.StoreKey);
        } else {
            countRegisters++;
        }

        printf("%i\t", data.StoreKey);
        printf("%s\t", data.Country);
        printf("%s\t", data.State);
        printf("%i\n", data.SquareMeters);
    }
    printf("\nRecords entered: %i", countRegisters);
    fclose(fp); fclose(fpStores);
    return;
}

void CreateExchangeRatesTable(char fileName[]) {
    FILE *fp = fopen(fileName, "r");
    FILE *fpExchangeRates = fopen("ExchangeRates.table", "ab");

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
    printf("\nRecords entered: %i", countRegisters);
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

int CompareSaleByProductKey(const void *register1, const void *register2) {
    const SalesData *sale1 = (const SalesData*)register1;
    const SalesData *sale2 = (const SalesData*)register2;
    return (sale1->ProductKey > sale2->ProductKey) - (sale1->ProductKey < sale2->ProductKey);
}

int CompareCustomerKey(const void *key1, const void *key2) {
    const CustomersData *customer1 = (const CustomersData*)key1;
    const CustomersData *customer2 = (const CustomersData*)key2;
    return (customer1->CustomersKey > customer2->CustomersKey) - (customer1->CustomersKey < customer2->CustomersKey);
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
    printf("Company Global Electronics Retailer\nValid to 2024-May-06 at 14:22 hours\nProducts list ordered by ProductName + Continent + Country + Sate + City\n");

    while (fread(&product, sizeof(ProductsData), 1, fpProducts) == 1) {
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
    printf("\t\t\t\t\tTime used to produce this listing: %d'%d\"\n************************************************LAST LINE OF THE REPORT*************************************************\n", minutes, seconds);
    printf("------------------------------------------------------------------------------------------------------------------------\n");

    fclose(fpProducts);
    fclose(fpSales);
    fclose(fpCustomers);
}

int main() {
    float option = 0.0; 
    do {
        printf("\n\nCompany Global Electronics Retailer\nOptions menu\n");
        printf("0. Exit program\n1. Construction of the Database with the dataset tables\n");
        printf("2. List of ¿What types of products does the company sell, and where are customers located?\n\t2.1 Utility bubbleSort\n\t2.2 Utility mergeSort\n");
        printf("3. List of ¿Are there any seasonal patterns or trends for order volume or revenue?\n\t3.1 Utility bubbleSort\n\t3.2 Utility mergeSort\n");
        printf("4. List of ¿How long is the average delivery time in days? Has that changed over time?\n\t4.1 Utility bubbleSort\n\t4.2 Utility mergeSort\n");
        printf("5. List of sales order by \"Customer Name\"+\"Order Date\"+\"ProductKey\";\n\t5.1 Utility bubbleSort\n\t5.2 Utility mergeSort\nWhat is your option: ");
        scanf("%f", &option);
        option = (option * 10);

        if (option == 10) {
            char file[30];
            printf("Enter the file name: ");
            scanf("%s", file);
            if (strcmp(file, "Sales.dat") == 0) {
                CreateSalesTable(file);
            } else if (strcmp(file, "Customers.dat") == 0) {
                CreateCustomersTable(file);
            } else if (strcmp(file, "Stores.dat") == 0) {
                CreateStoresTable(file);
            } else if (strcmp(file, "Exchange_Rates.dat") == 0) {
                CreateExchangeRatesTable(file);
            } else if (strcmp(file, "Products.dat") == 0) {
                CreateProductsTable(file);
            } else {
                printf("File not found.");
            }
        } else if (option == 21) {
            PrintReport2(1);
        } else if (option == 22) {
            PrintReport2(2);
        } else {
            printf("\nInvalid option\n");
        }
    } while (option != 0);

    return 0;
}