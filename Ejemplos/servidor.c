#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080 //Se define el puerto mediante el cual se establecera la comunicacion
#define BUFFER_SIZE 1024 //Se define el tamanio del buffer para recibir datos

int main() {
    int server_fd; //Variable en la que se guardara el descriptor del servidor
    int new_socket; //Variable en la que se guardara el descriptor de las nuevas conexiones
    struct sockaddr_in servidor; //Estructura de definicion del socket
    struct sockaddr_in cliente; //Estructurad de definicion del cliente
    int servidorLen = sizeof(servidor); //Variable para guardar el tamanio de la estructura de direccion del socket
    char buffer[BUFFER_SIZE] = {0}; //Arreglo que contiene el buffer en que se guarda el mensaje recibido
    
    // Creando el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { // Crea el socket con los parametros (protocolos IPv4, Conexion tipo TCP, Protocolo TCP)
        perror("Fallo al crear el servidor");
        exit(EXIT_FAILURE); //Termina el programa y libera recursos
    }

    int valorConfig = 1;
    // Configurar opciones del socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &valorConfig, sizeof(valorConfig))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    memset(&servidor,0, sizeof(servidor)); //Inicializando la estructua que contiene al servidor
    servidor.sin_family = AF_INET; //Se determina la familia de direcciones IPv4
    servidor.sin_addr.s_addr = INADDR_ANY;  // Aceptar conexiones de cualquier IP
    servidor.sin_port = htons(PORT); //Convierte el puerto definido a formato de red
    
    
    if (bind(server_fd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) { // Vincular el socket al puerto definido y lo asocia con INADDR_ANY definidos anteriormente
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    
    if (listen(server_fd, 3) < 0) { //Listen crea el socket pasivo, usando el descriptor del socket y mandando como parametro 3 (numero maximo de conexiones pendientes)
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Servidor escuchando en el puerto %d...\n", PORT);
    
    // Aceptar una nueva conexión
    int clienteLen = sizeof(cliente);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&cliente, (socklen_t*)&clienteLen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    // Obtener la dirección IP del cliente
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &cliente.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Conexión establecida con %s\n", client_ip);
    
    // Leer mensaje del cliente
    read(new_socket, buffer, BUFFER_SIZE); //Se utiliza read para leer el mensaje de cliente y se almacena en la variable BUFFER
    printf("Mensaje recibido: %s\n", buffer);
    
    // Enviar respuesta al cliente
    char *response = "Hola desde el servidor!";
    send(new_socket, response, strlen(response), 0);
    printf("Respuesta enviada\n");
    
    // Cerrar sockets
    close(new_socket);
    close(server_fd);
    
    return 0;
}