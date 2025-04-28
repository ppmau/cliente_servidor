#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 1024

/* Prototipos de funciones */
int crear_conexion(const char *ip_servidor);
void manejar_error(const char *mensaje, int cerrar_socket, int sock);

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <IP del servidor>\n", argv[0]);
        return -1;
    }

    char buffer[BUFFER_SIZE] = {0};
    const char *ip_servidor = argv[1];

    while(1) {
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            manejar_error("Error leyendo entrada", 0, -1);
            break;
        }

        size_t msg_len = strcspn(buffer, "\n");
        buffer[msg_len] = '\0';
        
        if (strcmp(buffer, "SALIR") == 0) {
            break;
        }

        int sock = crear_conexion(ip_servidor);

        ssize_t bytes_sent = send(sock, buffer, msg_len + 1, 0);
        if (bytes_sent <= 0) {
            manejar_error("Error enviando mensaje", 1, sock);
            continue;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE - 1);
        if (valread <= 0) {
            if (valread == 0) {
                printf("Servidor cerró la conexión\n");
            } else {
                manejar_error("Error leyendo respuesta", 1, sock);
            }
        } else {
            buffer[valread] = '\0';
            printf("%s\n", buffer);
        }

        close(sock);
    }
    
    return 0;
}

int crear_conexion(const char *ip_servidor) {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        manejar_error("Error al crear socket", 0, -1);
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip_servidor, &serv_addr.sin_addr) <= 0) {
        manejar_error("Dirección inválida", 1, sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        manejar_error("Conexión fallida", 1, sock);
        return -1;
    }

    return sock;
}

void manejar_error(const char *mensaje, int cerrar_socket, int sock) {
    perror(mensaje);
    if (cerrar_socket && sock >= 0) {
        close(sock);
    }
}