import java.io.*;
import java.net.*;

public class Servidor {
    public static void main(String[] args) {
        int puerto = 5000;
        
        try (ServerSocket serverSocket = new ServerSocket(puerto)) {
            System.out.println("Servidor en Java escuchando en el puerto " + puerto + "...");

            while (true) {
                try (Socket clientSocket = serverSocket.accept();
                     BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream(), "UTF-8"));
                     PrintWriter out = new PrintWriter(new OutputStreamWriter(clientSocket.getOutputStream(), "UTF-8"), true)) {

                    System.out.println("Cliente conectado desde: " + clientSocket.getInetAddress());

                    // Leer el mensaje del cliente C. 
                    // readLine() lee hasta encontrar un salto de línea (\n)
                    String mensajeCliente = in.readLine();
                    System.out.println("El cliente C dice: " + mensajeCliente);

                    // Enviar respuesta al cliente C.
                    // println agrega automáticamente el salto de línea (\n) al final.
                    String respuesta = "Hola que tal, saludos desde el servidor en Java";
                    out.println(respuesta);
                    System.out.println("Respuesta enviada al cliente.");
                    System.out.println("-------------------------------------------------");
                } catch (IOException e) {
                    System.err.println("Error al comunicarse con el cliente: " + e.getMessage());
                }
            }
        } catch (IOException e) {
            System.err.println("No se pudo escuchar en el puerto " + puerto);
            e.printStackTrace();
        }
    }
}
