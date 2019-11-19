//
// YodafyServidorIterativo
// (CC) jjramos, 2012
//
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.lang.Thread;
import java.net.Socket;
import java.net.UnknownHostException;

public class YodafyClienteTCP {

	public static void main(String[] args) {

		String buferEnvio;
		String buferRecepcion;
		int bytesLeidos=0;

		String host="localhost";

                int port=8483;

		Socket socketServicio=null;

		try {

			socketServicio=new Socket (host,port);
                            PrintWriter outPrinter = new PrintWriter(socketServicio.getOutputStream(),true);
                            BufferedReader inReader = new BufferedReader(new InputStreamReader(socketServicio.getInputStream()));
			
			buferEnvio="Al monte del volcán debes ir sin demora";

                       
                        outPrinter.println(buferEnvio);
			
			System.out.println("Recibido: ");
			System.out.println(inReader.readLine());
                        socketServicio.close();
                        
			// Excepciones:
		} catch (UnknownHostException e) {
			System.err.println("Error: Nombre de host no encontrado.");
		} catch (IOException e) {
			System.err.println("Error de entrada/salida al abrir el socket.");
		}
	}
}
