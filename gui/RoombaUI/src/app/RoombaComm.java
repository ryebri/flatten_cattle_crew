package app;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Random;

import app.view.OutputTextController;

//make this a new thread object
public class RoombaComm {
	//constructor
	private Socket socket;
	private BufferedReader in;
	private PrintWriter out;
	private OutputTextController outputTxtCntr;
	
	private Random rand = new Random(3);
	public RoombaComm()
	{
		outputTxtCntr = null;
	}
	
	public RoombaComm(OutputTextController outputTextController)
	{
		outputTxtCntr = outputTextController;
	}
	//connect
	public void connect(String server_addr, int port) throws IOException
	{
		socket = new Socket();

		InetSocketAddress sa = new InetSocketAddress(InetAddress.getByName(server_addr), port);

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
	public void send_string(String send, char test)
	{
		//do some error checking
		if(test == 0x00)
		{
			out.print(send);
			out.flush();
		}
	}
	
	//receive
	public String get_response(char inst)
	{
		String resp = "Timeout";
		//add in option for different responses, and actual response from robot
		if(inst == 0x00)
		{
			try {
				//need a timeout/a better way to read until there is nothing left to send
				resp = in.readLine();
				System.out.println(resp);
				//check to see if it is a json object, if it is, then it will have ir values
				//in.reset();
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		return resp;
	}
	
	public void set_controller(OutputTextController output)
	{
		outputTxtCntr = output;
	}
	
	public void test()
	{
		int ir_sensor[] = new int[180];
		
		for(int i = 0; i < 180; i++)
		{
			ir_sensor[i] = rand.nextInt(130);
		}
		
		outputTxtCntr.set_sensor(ir_sensor);
		
	}
}
