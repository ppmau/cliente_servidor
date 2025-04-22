#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 10
#define MAX_LENGTH 100

// Prototipo
void analizadorDeSintaxis(char *consulta[], int numTokens);

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
                    printf("Haciendo insert con los valores %s", consulta[4]);
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
        printf("Haciendo un UPDATE\n");
        if(numTokens > 1 && strcmp(consulta[1], "Alumno") == 0){
            if (numTokens > 2 && strcasecmp(consulta[2], "SET") == 0){
                //Solo verifica que exista un elemento despues del set, la verificacion del campo a insertar la hara despues
                if(numTokens > 3){
                    if (numTokens > 4 && strcasecmp(consulta[4], "WHERE") == 0){
                        if(numTokens > 5){
                            printf("Haciendo update a la tabla %s los valores de %s en donde el id sea %s", consulta[1],consulta[3], consulta[5]);
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
