#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // un file descriptor para el socket servidor y
    // otro file descriptor para el socket cliente
    int server_fd, client_fd;

    // una estructura para el socket servidor y
    // otra estructura para el socket cliente 
    struct sockaddr_in server_addr, client_addr;


    // Crear el socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error al crear el socket");
        return 1;
    }

    // Configurar la direcciÃ³n del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(9999);

    // Asociar el socket con la direcciÃ³n y puerto
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error en bind");
        return 1;
    }

    // Escuchar conexiones con backlog = 1
    if (listen(server_fd, 1) == -1) {
        perror("Error en listen");
        return 1;
    }

    printf("Servidor esperando conexiones...\n");
    sleep(10);  // Simulamos que el servidor tarda en llamar a accept()

    socklen_t addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if (client_fd == -1) {
        perror("Error en accept");
        return 1;
    }

    printf("Cliente aceptado.\n");

    close(client_fd);
    close(server_fd);
    return 0;
}