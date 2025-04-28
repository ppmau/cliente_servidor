#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    // Verificar que se proporcionó la IP del servidor
    if (argc != 2) {
        printf("Uso: %s <IP del servidor>\n", argv[0]);
        return -1;
    }

    // Crear el socket del cliente
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nError al crear socket \n");
        return -1;
    }
    
    // Configurar la estructura de dirección del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convertir dirección IP de texto a binario
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nDirección inválida / Dirección no soportada \n");
        return -1;
    }
    
    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConexión fallida \n");
        return -1;
    }
    
    printf("Conectado al servidor %s\n", argv[1]);
    
    // Comunicación con el servidor
    while(1) {
        printf("Escribe un mensaje (o 'exit' para salir): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        
        // Eliminar el salto de línea del final
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // Salir si el usuario escribe 'exit'
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        
        // Enviar mensaje al servidor
        send(sock, buffer, strlen(buffer), 0);
        
        // Limpiar el buffer
        memset(buffer, 0, BUFFER_SIZE);
        
        // Recibir respuesta del servidor
        int valread = read(sock, buffer, BUFFER_SIZE);
        printf("Respuesta del servidor: %s\n", buffer);
    }
    
    // Cerrar socket
    close(sock);
    
    return 0;
}