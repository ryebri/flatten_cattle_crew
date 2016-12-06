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
	private int bitfield;
	
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
	//Rex Borseth
	//send
	public void send_bitfield(int id, boolean pressed)
	{
		switch(id){
		case 0:
			// make function for sending specific bit
			break;
		case 3:
			break;
		case 6:
			break;
		case 7:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
		}
		out.print(bitfield);
		out.flush();
	}
	
	
	private void set_button_bit(int bit, boolean pressed){// adds the button bit to the bitfield
		if(pressed){
			bitfield +=bit^2;
		}
		else{
			bitfield -= bit^2;
		}
	}
	private void send_trigger_bit(int speed, boolean isright,boolean pressed){	// adds bits for the trigger to the bitfield
		if(isright){
			if(pressed){
				bitfield += speed * 8;
			}
			else{
				bitfield &= 65479; // clears bits
			}
		}
		else{
			if(pressed){
				bitfield += speed;
			}
			else{
				bitfield &= 65528;// clears bits
			}
		}
	}
	
	//Rex Borseth
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
		} else if(inst == 0x01){
			resp = "{\"ir\": [044,045,047,045,045,045,039,043,041,043,045,044,041,035,038,037,037,034,035,035,040,045,039,038,039,041,042,037,043,039,041,041,042,036,045,041,047,045,046,046,037,034,118,063,046,053,074,049,059,050,062,103,291,287,366,366,369,296,369,366,366,293,316,366,369,311,295,350,363,363,369,293,366,310,292,363,366,366,366,289,366,369,291,366,357,366,298,363,369,297,325,366,366,321,293,366,363,347,366,292,310,363,300,369,293,366,304,369,290,366,291,366,291,270,090,055,073,292,304,363,363,369,295,366,317,363,330,363,301,366,366,294,293,369,292,369,369,290,342,328,366,366,369,366,366,273,366,293,366,366,303,363,292,340,296,291,366,358,363,363,366,293,366,369,304,325,369,369,366,366,319,293,366,292,287,366,294,369,369,369]}";
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
