import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class YodafyServidorIterativo {

	public static void main(String[] args) {

		// Puerto de escucha
		int port=8483;

				ServerSocket socketServidor;
				Socket socketServicio=null;
		try {
				socketServidor=new ServerSocket(port);
			
			do {

				
			socketServicio=socketServidor.accept();
				
				ProcesadorYodafy procesador=new ProcesadorYodafy(socketServicio);
				procesador.start(); 
                              
			} while (true);

		} catch (IOException e) {
			System.err.println("Error al escuchar en el puerto "+port);
		}

	}

}
