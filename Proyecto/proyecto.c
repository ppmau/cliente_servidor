#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 10
#define MAX_LENGTH 100

// Prototipo
void analizadorDeSintaxis(char *consulta[], int numTokens);
void funcionSelect();
void funcionInsert(char *values);
void funcionUpdate(char *campo, char *id);
void funcionDelete(char *cadenaId);

int main() {
    char input[MAX_LENGTH];
    char *tokens[MAX_TOKENS];
    int count = 0;

    const char *tablas[] = {"Alumno"};
    const char *campos[] = {"Id", "Nombre", "Apellido", "Semestre", "Carrera"};

    printf("Analizador de sintaxis\n");
    printf("Ingrese la instrucción deseada: ");
    fgets(input, sizeof(input), stdin);

    // Eliminar salto de línea
    input[strcspn(input, "\n")] = 0;

    // Hacer token cada una de las palabras, separando por espacios
    char *token = strtok(input, " ");
    while (token != NULL && count < MAX_TOKENS) {
        tokens[count++] = token;
        token = strtok(NULL, " ");
    }

    analizadorDeSintaxis(tokens, count);
    


    return 0;
}

void analizadorDeSintaxis(char *consulta[], int numTokens) {
    if (numTokens == 0) {
        printf("Instrucción vacía.\n");
        return;
    }
    //Validando token en primer paralabra, coinicida con "SELECT", ya sea mayùsucula o minùscula
    if (strcasecmp(consulta[0], "SELECT") == 0) {
        //Validando que se seleccionen todos los campos de la tabla (ùnico requerimiento para el proyecto)
        if (numTokens > 1 && strcmp(consulta[1], "*") == 0) {
            //Validando que la palabra en la posicion 2 sea FROM
            if (numTokens > 2 && strcasecmp(consulta[2], "FROM") == 0) {
                //Validando que el token en la posiciòn 3 sea Alumno (ùnico requerimiento para el proyecto)
                if (numTokens > 3 && strcmp(consulta[3], "Alumno") == 0) {
                    //Se manda llamar a la funcion que realiza el select cuando la sintaxis es correcta y la tabla existe
                    funcionSelect();
                } else {
                    printf("Sintaxis incorrecta. No existe la tabla seleccionada\n");
                }
            } else {
                //Si el token en la posiciòn 2 no es FROM, forzozamente es un error de sintaxis en el FROM
                printf("Sintaxis incorrecta. Quisiste decir 'FROM'?\n");
            }
        } else {
            //Si el token en la posiciòn 1 no es *, se devuelve un mensaje de error.
            printf("Error. No se encontraron los campos indicados.\n");
        }
    } else if (strcasecmp(consulta[0], "INSERT") == 0) {
        printf("Haciendo un INSERT\n");
        //Validando que el token en la posiciòn 1 sea INTO, ya sea mayùscula o minùscula
        if (numTokens > 1 && strcasecmp(consulta[1], "INTO") == 0){
            //Validando que el token en la posicion 2 sea Alumno
            if(numTokens > 2 && strcmp(consulta[2], "Alumno") == 0){
                //Validando que el token en la posicion 3, sea VALUES, ya sea mayuscula o minuscula
                if(numTokens > 3 && strcasecmp(consulta[3], "VALUES") == 0){
                    funcionInsert(consulta[4]);
                }
                else{
                    //Si el token en la posicion 3 no es VALUES, se devuelve un mensaje de error
                    printf("Sintaxis incorrecta. Quisiste decir VALUES?");
                }
            }
            else{
                printf("Error. No existe la tabla seleccionada");
            }
        } 
        //Si el token en la posicion 1 no es INTO, existe un error de sintaxis en INTO
        else{
            printf("Sintaxis incorrecta. Quisiste decir 'INTO'?");
        }

    } else if (strcasecmp(consulta[0], "UPDATE") == 0) {
        if(numTokens > 1 && strcmp(consulta[1], "Alumno") == 0){
            if (numTokens > 2 && strcasecmp(consulta[2], "SET") == 0){
                //Solo verifica que exista un elemento despues del set, la verificacion del campo a insertar la hara despues
                if(numTokens > 3){
                    if (numTokens > 4 && strcasecmp(consulta[4], "WHERE") == 0){
                        if(numTokens > 5){
                            funcionUpdate(consulta[3],consulta[5]);
                        } 
                    }
                    else{
                        printf("Error de sintaxis. Quisiste decir 'WHERE'?");
                    }
                }
                //Avisa al usuario que no esta enviando el valor a actualizar
                else{
                    printf("Error de sintaxis. No se indica cual es el valor a actualizar.");
                }
            }
            else{
                printf("Sintaxis incorrecta. Quisiste decir 'SET'?");
            }
        }
        else{
            printf("Error. No existe la tabla indicada");
        }
    } else if (strcasecmp(consulta[0], "DELETE") == 0) {
        printf("Haciendo un DELETE\n");
        if (numTokens > 1 && strcasecmp(consulta[1], "FROM") == 0) {
            if(numTokens > 2 && strcmp(consulta[2], "Alumno") == 0){
                if (numTokens > 3 && strcasecmp(consulta[3], "WHERE") == 0){
                    if(numTokens > 4){
                        printf("Borrando el valor %s en la tabla %s", consulta[4], consulta[2]);
                        funcionDelete(consulta[4]);
                        printf("Entrando...");
                    }
                    else{
                        printf("Error. No se indico el valor a modificar");
                    }
                }
                else{
                    printf("Error de sintaxis. Quisiste decir 'WHERE'?");
                }
            }
            else{
                printf("Error. No se encontro la tabla indicada");
            }
        }
        else{
            printf("Error de sintaxis. Quisiste decir 'FROM'?");
        }
    } else {
        //Si no coincide con ninguna de las palabras reservadas para la base de datos, envìa este mensaje de error
        printf("Sintaxis incorrecta. No se encontro la instruccion indicada\n");
    }
}


void funcionSelect(){
    FILE *archivo = fopen("alumno.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
    }
    else{
        printf("\n");
        char linea[100];
        int contadorLinea;
        contadorLinea = 0;
        while (fgets(linea, sizeof(linea), archivo)) {
            printf("%s", linea);
            contadorLinea++;
        }
        fclose(archivo);
    }
}

void funcionInsert(char *values){
    FILE *archivo = fopen("alumno.txt", "a+");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
    }
    else{  
        //Verifica que los campos a ingresar se encuentren contenidos dentro de parentesis
        if (values[0] == '(' && values[strlen(values) - 1] == ')') { 

            char linea[256];            // Variable que se utilizara para leer las lineas del archivo
            char ultimaLinea[256] = ""; // Variable donde se guardara la ultima linea obtenida para realizar para obtener el ID
            int nuevoLen = strlen(values)-2; //Valor entero correspondiente a la nueva longitud de los valores a insertar, eliminando 2 parentesis
            char valuesSinParentesis[nuevoLen]; //Variable que guardara la nueva cadena

            for (int i = 1; i <= strlen(values) - 2; i++){//Quitando parentesis del string para insertar
                valuesSinParentesis[i-1] = values[i];   
            }

            valuesSinParentesis[nuevoLen] = '\0';  //Agregando caracter que indica el final del char
            while (fgets(linea, sizeof(linea), archivo) != NULL) { //Determinando la ultima linea para rastrear el ultimo ID 
                strcpy(ultimaLinea, linea);
            }
            char *token = strtok(ultimaLinea, ","); //Variable que contiene al primer token de una cadena separada por comas (ID)
            int id = atoi(token) + 1; //Parseando valor de id y sumando 1 (el autoincremental)
            int lenId = snprintf(NULL, 0, "%d", id);  // Longitud del nuevo ID que previene valores de mas de un digito
            char idChar[lenId + 1]; //Variable para guardar el nuevo id mas un espacio extra para indicar el fin de la cadena
            sprintf(idChar, "%d", id); //Se guarda en idChar el valor entero de id

            
            int totalLen = lenId + 1 + strlen(valuesSinParentesis + 1); // Se crea espacio para ID, coma, values y \0 para la nueva cadena con el nuevo ID concatenado
            char lineaInsert[totalLen]; // +1 para \0 final
            strcpy(lineaInsert, idChar); //Se copia lo que contiene idChar
            lineaInsert[lenId] = ',';  //Se agrega la comma
            lineaInsert[lenId + 1] = '\0'; //Se indica el final temporal de la cadena
            strcat(lineaInsert, valuesSinParentesis); //Se agrega la cadena recibida al ID nuevo con su coma

            fprintf(archivo, "%s\n", lineaInsert); //Se inserta el nuevo registro 

    fclose(archivo);

        } else {
            printf("Error de sintaxis. Incorrecto uso de parentesis\n");
        }
    }
}


void funcionUpdate(char *campo, char *id){
    FILE *archivo = fopen("alumno.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
    }
    else{
        //Definiendo variables auxiliares para descomoner el campo en que se insertara, el valor y el id
        char *tokenID;
        char *charID;
        char *charCampo;
        char *charValorCampo;
        char copiaID[10];
        char copiaCampo[50];
        int contadorTokens = 0;

        strcpy(copiaID, id);
        char *rest = copiaID;
        strcpy(copiaCampo, campo);
        char *restCampo = copiaCampo;

        while ((campo = strtok_r (restCampo,"=", &restCampo))){
            if (contadorTokens == 0){
                charCampo = campo;
                contadorTokens++;
            }
            else{
                charValorCampo = campo;
            }
        }
        printf("\nEl campo a cambiar es: %s el valor es: %s", charCampo, charValorCampo);

        while ((tokenID = strtok_r(rest, "=", &rest))) {
                charID = tokenID;
            }
        printf("\nValor de ID a cambiar: %s" ,charID);

        //Definiendo variables para recorrer y guardar el contenido del archivo para reescribir con el cambio posteriormente
        char linea[256];
        char bufferAntes[4096] = "";
        char bufferDespues[4096] = "";
        char lineaEncontrada[256]  = {0};
        bool encontrado = false;


        while (fgets(linea, sizeof(linea), archivo)) {
            char copiaLinea[256];
            strcpy(copiaLinea, linea); // No alterar la original
            
            char *token = strtok(copiaLinea, ",");
            
            if (strcmp(token, charID) == 0 && !encontrado) {
                strcpy(lineaEncontrada,linea);
                encontrado = true;
            } 
            else{
                if (!encontrado){
                    strcat(bufferAntes, linea);
                } 
                else{
                    strcat(bufferDespues, linea);
                }
            }
        }
        //Habiendo recorrido el archivo en busqueda de un campo ID igual al ingresado
        char nuevaLinea[150] = {0};
        char *restLineaEncontrada;
        char *tokenLinea;
        int contadorCampo = 0;


        if(encontrado){
            if (strcmp(charCampo, "Nombre") == 0){
                tokenLinea = strtok_r(lineaEncontrada, ",", &restLineaEncontrada);
                while (tokenLinea != NULL) {
                    if (contadorCampo == 1){
                        strcat(nuevaLinea,charValorCampo);
                        strcat(nuevaLinea, ","); 
                        contadorCampo++;
                    }
                    else{
                        if (contadorCampo == 4){
                            strcat(nuevaLinea,tokenLinea);
                        }
                        else{
                            strcat(nuevaLinea,tokenLinea);
                            strcat(nuevaLinea, ",");
                            contadorCampo++; 
                        }
                    }
                    
                    tokenLinea = strtok_r(NULL, ",", &restLineaEncontrada);
                }
                fclose(archivo);
                FILE *archivoNuevo = fopen("alumno.txt", "w");
                if (archivoNuevo == NULL) {
                    printf("Error al abrir el archivo\n");
                }
                else{
                    fputs(bufferAntes, archivoNuevo);
                    fputs(nuevaLinea, archivoNuevo);
                    fputs(bufferDespues, archivoNuevo);
                }
                fclose(archivoNuevo);

            }
            else if(strcmp(charCampo, "Apellido") == 0){
                printf("Coincidencia en %s", charCampo);
                tokenLinea = strtok_r(lineaEncontrada, ",", &restLineaEncontrada);
                while (tokenLinea != NULL) {
                    if (contadorCampo == 2){
                        strcat(nuevaLinea,charValorCampo);
                        strcat(nuevaLinea, ","); 
                        contadorCampo++;
                    }
                    else{
                        if (contadorCampo == 4){
                            strcat(nuevaLinea,tokenLinea);
                        }
                        else{
                            strcat(nuevaLinea,tokenLinea);
                            strcat(nuevaLinea, ",");
                            contadorCampo++; 
                        }
                    }
                    
                    tokenLinea = strtok_r(NULL, ",", &restLineaEncontrada);
                }
                fclose(archivo);
                FILE *archivoNuevo = fopen("alumno.txt", "w");
                if (archivoNuevo == NULL) {
                    printf("Error al abrir el archivo\n");
                }
                else{
                    fputs(bufferAntes, archivoNuevo);
                    fputs(nuevaLinea, archivoNuevo);
                    fputs(bufferDespues, archivoNuevo);
                }
                fclose(archivoNuevo);
            }
            else if(strcmp(charCampo, "Semestre") == 0 ){
                printf("Coincidencia en %s", charCampo);
                tokenLinea = strtok_r(lineaEncontrada, ",", &restLineaEncontrada);
                while (tokenLinea != NULL) {
                    if (contadorCampo == 3){
                        strcat(nuevaLinea,charValorCampo);
                        strcat(nuevaLinea, ","); 
                        contadorCampo++;
                    }
                    else{
                        if (contadorCampo == 4){
                            strcat(nuevaLinea,tokenLinea);
                        }
                        else{
                            strcat(nuevaLinea,tokenLinea);
                            strcat(nuevaLinea, ",");
                            contadorCampo++; 
                        }
                    }
                    
                    tokenLinea = strtok_r(NULL, ",", &restLineaEncontrada);
                }
                fclose(archivo);
                FILE *archivoNuevo = fopen("alumno.txt", "w");
                if (archivoNuevo == NULL) {
                    printf("Error al abrir el archivo\n");
                }
                else{
                    fputs(bufferAntes, archivoNuevo);
                    fputs(nuevaLinea, archivoNuevo);
                    fputs(bufferDespues, archivoNuevo);
                }
                fclose(archivoNuevo);
            }
            else if(strcmp(charCampo, "Carrera") == 0){
                printf("Coincidencia en %s", charCampo);
                tokenLinea = strtok_r(lineaEncontrada, ",", &restLineaEncontrada);
                while (tokenLinea != NULL) {
                    if (contadorCampo == 4){
                        strcat(nuevaLinea,charValorCampo);
                        strcat(nuevaLinea,"\n");
                    }
                    else{
                        strcat(nuevaLinea,tokenLinea);
                        strcat(nuevaLinea, ",");
                        contadorCampo++; 
                    }
                    
                    tokenLinea = strtok_r(NULL, ",", &restLineaEncontrada);
                }
                fclose(archivo);
                FILE *archivoNuevo = fopen("alumno.txt", "w");
                if (archivoNuevo == NULL) {
                    printf("Error al abrir el archivo\n");
                }
                else{
                    fputs(bufferAntes, archivoNuevo);
                    fputs(nuevaLinea, archivoNuevo);
                    fputs(bufferDespues, archivoNuevo);
                }
                fclose(archivoNuevo);
            }    
            else{
                printf("No hay coincidencia en el campo a actualizar");
            }

        }
        else{
            printf("\nNo hay coincidencias con el id: %s proporcionado", charID);
        }
    
    }
}


void funcionDelete(char *cadenaId){
    printf("Entro");
    char linea[256];
    char buffer[4096] = "";
    char lineaEncontrada[256]  = {0};
    bool encontrado = false;
    char *charID;
    char *tokenID;
    char copiaCadenaId[100];

    strcpy(copiaCadenaId, cadenaId); 
    char *restCadenaId = copiaCadenaId;


    while ((tokenID = strtok_r(restCadenaId, "=", &restCadenaId))) {
        charID = tokenID;
    }

    FILE *archivo = fopen("alumno.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
    }
    else{

        while (fgets(linea, sizeof(linea), archivo)) {
            char copiaLinea[256];
            strcpy(copiaLinea, linea); // No alterar la original    
            char *token = strtok(copiaLinea, ",");
            //printf("token: %s charID: %s", token, charID);
            if (strcmp(token, charID) != 0) {
                strcat(buffer,linea);
                encontrado = true;
            } 
            
        }
        printf("El nuevo contenido del archivo es: %s", buffer);
    }

    fclose(archivo);

    FILE *archivoNuevo = fopen("alumno.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
    }
    else{
        fputs(buffer, archivoNuevo);    
    }
    


    
}


