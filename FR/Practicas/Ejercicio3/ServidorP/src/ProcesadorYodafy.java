
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Random;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.lang.Thread;


public class ProcesadorYodafy extends Thread{

    private Socket socketServicio;

    private InputStream inputStream;

    private OutputStream outputStream;


    private Random random;

        private PrintWriter outPrinter;
        private BufferedReader inReader;

        public ProcesadorYodafy(Socket socketServicio) throws IOException {
		this.socketServicio=socketServicio;
		random=new Random();
                this.inReader= new BufferedReader(new InputStreamReader(this.socketServicio.getInputStream()));
                this.outPrinter = new PrintWriter(socketServicio.getOutputStream(),true);
        }

        public void run(){
            procesa();
        }
        
        
        public void procesa(){

		try {
                        String peticion=this.inReader.readLine();
                  
			String respuesta=yodaDo(peticion);

                        this.outPrinter.println(respuesta);


		} catch (IOException e) {
			System.err.println("Error al obtener los flujso de entrada/salida.");
		}

	
        }

	private String yodaDo(String peticion) {
		// Desordenamos las palabras:
		String[] s = peticion.split(" ");
		String resultado="";

		for(int i=0;i<s.length;i++){
			int j=random.nextInt(s.length);
			int k=random.nextInt(s.length);
			String tmp=s[j];

			s[j]=s[k];
			s[k]=tmp;
		}

		resultado=s[0];
		for(int i=1;i<s.length;i++){
		  resultado+=" "+s[i];
		}

		return resultado;
	}
}
