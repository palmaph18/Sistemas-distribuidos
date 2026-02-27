import java.io.*;
import java.net.*;
import java.util.Scanner;

public class Cliente {
    public static void main(String[] args) {
        String host = "127.0.0.1";
        int puerto = 5001;
        
        try (Socket socket = new Socket(host, puerto);
             DataInputStream in = new DataInputStream(socket.getInputStream());
             DataOutputStream out = new DataOutputStream(socket.getOutputStream());
             Scanner scanner = new Scanner(System.in)) {
             
            System.out.println("Conectado al servidor C en el puerto " + puerto);
            
            while (true) {
                System.out.print("Ingresa un número entero (escribe 0 para salir): ");
                
                // Validar que el usuario ingrese un número entero
                if (!scanner.hasNextInt()) {
                    System.out.println("Por favor, ingresa un número entero válido.");
                    scanner.next(); // Limpiar el buffer
                    continue;
                }
                
                int numero = scanner.nextInt();
                
                // Enviar el número al servidor C en formato binario (4 bytes, Big-Endian)
                out.writeInt(numero);
                
                if (numero == 0) {
                    System.out.println("Enviando 0. Cerrando el programa...");
                    break; // Salir del bucle y cerrar 
                }
                
                // Leer la respuesta del servidor (el número incrementado) 
                int respuesta = in.readInt();
                System.out.println("El servidor C respondió: " + respuesta);
                System.out.println("----------------------------------------");
            }
            
        } catch (IOException e) {
            System.err.println("Error de conexión: " + e.getMessage());
        }
    }
}
