#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // Agregar \n al final para que readLine() en Java funcione correctamente
    char *mensaje = "Hola, soy el cliente en C\n"; 
    char buffer[BUFFER_SIZE] = {0};

    // 1. Crear el socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error en la creación del socket \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. Convertir la dirección IP de texto a formato binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nDirección inválida o no soportada \n");
        return -1;
    }

    // 3. Conectar al servidor Java
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nFallo en la conexión. Asegúrate de que el servidor Java esté corriendo.\n");
        return -1;
    }

    // 4. Enviar el mensaje
    // Usamos strlen(mensaje) para enviar solo los bytes necesarios, no el \0 final de C.
    send(sock, mensaje, strlen(mensaje), 0);
    printf("Mensaje enviado al servidor: %s", mensaje);

    // 5. Leer la respuesta del servidor
    int valread = read(sock, buffer, BUFFER_SIZE - 1); // Dejamos espacio para el caracter nulo
    if (valread > 0) {
        buffer[valread] = '\0'; // Aseguramos que la cadena termine correctamente en C
        printf("El servidor Java responde: %s\n", buffer);
    }

    // 6. Cerrar el socket
    close(sock);
    return 0;
}
