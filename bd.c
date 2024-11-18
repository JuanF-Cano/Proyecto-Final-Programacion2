#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* int number(char token[], int count) { // str = "12"     ;    str2 = "1/1/2020"
    if (token[count] != '\0') {
        if (isdigit(token[count])) { // pregunta si hay un numero en la posicion count
            number(token, count + 1);
        } else {
            return 0; //FALSE
        }
    } else {
        return 1; // TRUE 
    }
} */

int main() {
    /* char line[] = "806791;Male;A�ron Blokdijk;Bleiswijk;ZH;Zuid-Holland;;Netherlands;Europe;7/10/1944";
    char *token = strtok(line, ";"); // 806791
    token = strtok(NULL, ";"); // Male
    token = strtok(NULL, ";"); // Aáron Blokdijk
    token = strtok(NULL, ";"); // Bleiswijk
    token = strtok(NULL, ";"); // ZH
    token = strtok(NULL, ";"); // Zuid-Holland
    printf("%s\n", token);

    token = strtok(NULL, ";");
    if (isdigit(token[0])) {
        data.ZipCode = (unsigned int)atoi(token);
        token = strtok(NULL, ";");
        strcpy(data.Country, token);
    } else {
        data.ZipCode = 0;
        strcpy(data.Country, token);
    } */
    /* token = strtok(NULL, ",");
   if (strlen(token) <= 85) {
        strcpy(data.Nombre, token);
   } else {
        counterErrors++;
        printf("Error(%i): Overflow Nombre: %s", counterErrors, token);
   } */
    /* char str[] = "hola,12,1/1/2020";
    char str2[] = "hola,,1/1/2020";
    
    char *token = strtok(str, ",");
    printf("%s\n", token);
    
    token = strtok(NULL, ","); // str = "12"  ;  "str2 = 1/1/2020"
    if (number(token, 0)) { //    str = true  ;   str2 = false
        // str
        data.squareMeters = (unsigned short int) atoi(token);// --> 12
        token = strtok(NULL, ","); //     "1/1/2020"
    } else {
        //str2
        data.squareMeters = 0; //             --> 0
    }
    sscanf(token, "%hhu/%hhu/%hu", &data.fecha.DD, &data.fecha.MM, &data.fecha.AAAA); //     --> 1/1/2020 */
    
    return 0;
}
