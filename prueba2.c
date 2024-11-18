#include <stdio.h>

typedef struct{
    unsigned short int ProductKey;
    char ProductName[85];
    char Brand[30];
    char Color[15];
    float UnitCostUSD;
    float UnitPriceUSD;
    char SubcatergoryKey[4];
    char Subcategory[35];
    char CategoryKey[2];
    char Category[30];
} ProductsData;

void Products(char nameDest[], char nameFuent[]){
    FILE *fp2 = fopen(nameDest, "wb");
    FILE *fp = fopen(nameFuent,"r");
   
    if (fp == NULL) {
        printf("Error opening file: %s\n", nameFuent);
        return;
    }
    if (fp2 == NULL) {
        printf("Error opening file: %s\n", nameDest);
        return;
    }

    ProductsData data;
    char line[1024] = "";
    while(fgets(line, sizeof(line), fp)){
        char *token = strtok(line, ",");
        data.ProductKey = (unsigned short int)atoi(token);

        if(line[strlen(token) + 1] == '"' && (line[strlen(token) + 2] == 'P' || line[strlen(token) + 2] == 'M')){
            token = strtok(NULL, "\"");
        }else{
            token = strtok(NULL, ",");
        }
        if (token[0] == '"') {
        token++;
        }
        strcpy(data.ProductName, token); 
        
        strcpy(data.Brand, strtok(NULL, ","));
        strcpy(data.Color, strtok(NULL, ","));

        token = strtok(NULL, ",$");
        if(token[0] == '"'){
            token = strtok(NULL, "\"");
            data.UnitCostUSD = (float)atof(token);
            token = strtok(NULL, ",$");
            if(token[0] == '"'){
                token = strtok(NULL, "\"");
                data.UnitPriceUSD = (float)atof(token);
            } else {
                data.UnitPriceUSD = (float)atof(token);
            }
        }else{
            data.UnitCostUSD = (float)atof(token);
            token = strtok(NULL, ",$");
            if(token[0] == '"'){
                token = strtok(NULL, "\"");
                data.UnitPriceUSD = (float)atof(token);
            } else {
                data.UnitPriceUSD = (float)atof(token);
            }
        }

        strcpy(data.SubcatergoryKey, strtok(NULL, ","));

        token = strtok(NULL, "^");
        if(token[0] == '"' || token[1] == '"'){
            token = strtok(token, "\"");
        }else{
            token = strtok(token, ",");
        }
        
        strcpy(data.Subcategory, token);

        token = strtok(NULL, ",");
        if(strlen(token) <= 2 || strlen(token) == 11 && token[2] == ' '){
        strcpy(data.CategoryKey, token);
        }

        token = strtok(NULL, "\"");
        strcpy(data.Category, token);

        fwrite(&data, sizeof(data), 1, fp2);
        
        printf("%s\n", data.ProductName);
    }
    fclose(fp); 
    fclose(fp2);
}