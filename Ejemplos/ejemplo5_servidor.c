#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <limits.h>

void main(int argc, char *argv[])
{
  struct sockaddr_in servidor;
  struct sockaddr_in cliente;
  struct hostent* info_cliente;
  int fd_s, fd_c;
  int longClient;
  char buf_peticion[256];
  char buf_respuesta[256];

  fd_s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	      
  memset((char *) &servidor, 0, sizeof(servidor));
  servidor.sin_family = AF_INET;
  servidor.sin_addr.s_addr = INADDR_ANY;
  servidor.sin_port = htons((u_short) atoi(argv[1]) );
  memset(&(servidor.sin_zero), '\0', 8);

  bind(fd_s, (struct sockaddr *) &servidor, sizeof(servidor));

  listen(fd_s, 1);

  longClient = sizeof(cliente);

  while(1)
  {
    fd_c = accept(fd_s, (struct sockaddr *) &cliente, &longClient);

    info_cliente = gethostbyaddr((char *) &cliente.sin_addr, sizeof(struct in_addr), AF_INET);
    if( info_cliente != NULL )
    {
      /* Lo siguiente es para obtener la fecha y hora y se impriman en pantalla */
      time_t T = time(NULL);
      struct tm tm = *localtime(&T);
      printf("%02d/%02d/%04d %02d:%02d:%02d-Cliente conectado ", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
      printf(" desde: %s\n\n", info_cliente -> h_name);
    }

    do
    {
      // memset(&(buf_peticion), '\0', 256);
      printf("1. buf_peticion=%s\n", buf_peticion);
      int n = recv(fd_c, buf_peticion, sizeof(buf_peticion), 0);
      printf("n=%d\n", n);
      printf("2. longitud de buf_peticion=%d\n", strlen(buf_peticion));
      printf("3. El mensaje del cliente es %s\n", buf_peticion);
      // buf_peticion[n-1] = '\n';
      printf("strcmp(buf_peticion,\"adios\")=%d\n", strcmp(buf_peticion,"adios\n"));

      fgets(buf_respuesta, 256, stdin);
      printf("El mensaje para cliente es %s\n", buf_respuesta);
      send( fd_c, buf_respuesta, strlen(buf_respuesta), 0 );
    }
    while( strcmp(buf_peticion,"adios\n") != 0 );
    printf("Se ha cerrado la conexion con el cliente... Esperando al siguiente\n");
    close(fd_c);
  }
  close(fd_s);
  shutdown( fd_s, SHUT_RDWR );
  exit(0);
}