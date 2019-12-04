package es.fwhibbit.sqli;

import java.awt.event.ActionEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

public class SQLInjection extends JFrame implements WindowListener {
	private static final long serialVersionUID = 1L;
	
	// Elementos gráficos
	JLabel 		lblUsuario 	= new JLabel("Usuario: ");
	JLabel 		lblPass		= new JLabel("Pass: ");
	JTextField 	txtUsuario 	= new JTextField("");
	JTextField 	txtPass		= new JTextField("");
	JLabel 		lblSecreto	= new JLabel("Secreto", SwingConstants.CENTER);
	JButton 	btnAceptar 	= new JButton("Aceptar");
	JButton 	btnLimpiar 	= new JButton("Limpiar");
	
	public SQLInjection() throws ClassNotFoundException, SQLException {
		
		// Configuración general de la ventana
		setLayout(null);
		setSize(400,250);
		setTitle("SQL Injection Test");
		setLocationRelativeTo(null);
		setVisible(true);
		
		// Añadimos los elementos a la ventana
		add(lblUsuario);	lblUsuario	.setBounds(1, 1, 100, 50);
		add(txtUsuario);	txtUsuario	.setBounds(100, 1, 300, 50);
		add(lblPass);		lblPass		.setBounds(1, 60, 100, 50);
		add(txtPass);		txtPass		.setBounds(100, 60, 300, 50);
		add(lblSecreto);	lblSecreto	.setBounds(0, 120, 400, 50);
		add(btnLimpiar);	btnLimpiar	.setBounds(95, 180, 100, 50);
		add(btnAceptar);	btnAceptar	.setBounds(205, 180, 100, 50);

		// Bloque de conexión a la base de datos
		Class.forName("org.mariadb.jdbc.Driver"); 
		String dbpass = "";
		Connection connection = DriverManager.getConnection("jdbc:mysql://localhost/dbftwr", "root", dbpass); 
		Statement s = connection.createStatement();
		
		// Listeners
		addWindowListener(this);
		// Botón cuya función es limpiar los campos de texto
		btnLimpiar.addActionListener((ActionEvent event) -> {
        	txtUsuario.setText("");
        	txtPass.setText("");
        });
		// Botón para iniciar sesión, está sección de código será la encargada de realizar las consultas
		btnAceptar.addActionListener((ActionEvent event) -> {
			try {
				ResultSet rs = s.executeQuery("select secreto from Usuarios where (nombre = '" + txtUsuario.getText() + "' and pass = '" + txtPass.getText() + "');");
	        	rs.next();
				lblSecreto.setText(rs.getString(1));
			} 
			catch (SQLException e) {e.printStackTrace();}
        });
		
		//connection.close();
	}
	public static void main(String[] args) throws NumberFormatException, IOException, ClassNotFoundException, SQLException {new SQLInjection();}

	public void actionPerformed(ActionEvent ae) {} 
	public void windowActivated(WindowEvent we) {}
	public void windowClosed(WindowEvent we) {}
	public void windowClosing(WindowEvent we) {this.setVisible(false);}
	public void windowDeactivated(WindowEvent we) {}
	public void windowDeiconified(WindowEvent we) {}
	public void windowIconified(WindowEvent we) {}
	public void windowOpened(WindowEvent we) {}
}

