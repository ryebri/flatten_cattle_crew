package app;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;


public class RoombaComm {
	//constructor
	Socket socket;
	BufferedReader in;
	PrintWriter out;
	
	public RoombaComm()
	{
		
	}
	//connect
	public void connect(String server_addr, int port) throws IOException
	{
		socket = new Socket();
//		ServerSocket s = new ServerSocket(5000, 1000);
		InetSocketAddress sa = new InetSocketAddress(InetAddress.getByName(server_addr), port);
//		socket = new Socket(server_addr, 2000);
		socket.connect(sa, 2000);
		in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		out = new PrintWriter(socket.getOutputStream(), true);
	}
	
	//close
	public void close() throws IOException
	{
		socket.close();
	}
	
	//send
	public void send_string(String send)
	{
		out.print(send);
		out.flush();
	}
	
	//receive
}
