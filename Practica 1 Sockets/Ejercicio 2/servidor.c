#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h> // Para usar int32_t y asegurar el tamaño de 4 bytes

#define PORT 5001

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int32_t numero_recibido, numero_enviar;

    // 1. Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Fallo en socket");
        exit(EXIT_FAILURE);
    }

    // Configurar el socket para reusar el puerto inmediatamente
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Enlazar el socket al puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Fallo en bind");
        exit(EXIT_FAILURE);
    }

    // 3. Escuchar conexiones
    if (listen(server_fd, 3) < 0) {
        perror("Fallo en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor C escuchando en el puerto %d...\n", PORT);

    // 4. Aceptar la conexión del cliente Java
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Fallo en accept");
        exit(EXIT_FAILURE);
    }

    printf("Cliente Java conectado.\n");

    // Bucle para recibir números hasta que llegue un 0
    while (1) {
        // Leemos 4 bytes (tamaño de int32_t)
        int valread = read(new_socket, &numero_recibido, sizeof(numero_recibido));
        
        if (valread <= 0) {
            printf("Cliente desconectado o error en lectura.\n");
            break;
        }

        // Convertir de Network Byte Order (Big-Endian de Java) a Host Byte Order
        numero_recibido = ntohl(numero_recibido);
        printf("Cliente Java envió: %d\n", numero_recibido);

        // Si el cliente envía 0, terminamos 
        if (numero_recibido == 0) {
            printf("Cero recibido. Cerrando conexión...\n");
            break;
        }

        // Incrementar en 1 
        numero_enviar = numero_recibido + 1;

        // Convertir de nuevo a Network Byte Order para enviarlo a Java
        int32_t numero_red = htonl(numero_enviar);
        send(new_socket, &numero_red, sizeof(numero_red), 0);
        printf("Enviado al cliente: %d\n", numero_enviar);
        printf("-------------------------\n");
    }

    // 5. Cerrar sockets
    close(new_socket);
    close(server_fd);
    return 0;
}
