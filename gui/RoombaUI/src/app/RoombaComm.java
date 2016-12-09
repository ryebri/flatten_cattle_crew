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

///Communication Class
/**
 * Object for communicating with the roomba.
 * @author ryebri
 *
 */
public class RoombaComm {
	//constructor
	private Socket socket;
	private BufferedReader in;
	private PrintWriter out;
	private OutputTextController outputTxtCntr;
	private int bitfield;
	private int status;
	
	private Random rand = new Random(3);
	
	///Default Constructor
	/**
	 * Creates a new Roomba Communication object with no connection to the roomba. Used for testing
	 */
	public RoombaComm()
	{
		outputTxtCntr = null;
		status = 0;
	}
	
	///Constructor
	/**
	 * Constructor used to create a useful roomba control object, this is used during regular
	 * operations.
	 * @param outputTextController an OutputTextController object so that RoombaComm can use data
	 * included in the OutputTextController object.
	 */
	public RoombaComm(OutputTextController outputTextController)
	{
		outputTxtCntr = outputTextController;
		status = 0;
	}
	
	///Makes connection to the GUI uart
	/**
	 * Method to create a connection between the GUI and the Roomba.  Computer must be connected
	 * to the wifi of the roomba in order to communicate.
	 * @param server_addr String for the server address.
	 * @param port Int value that specifies the port to listen/send to.
	 * @throws IOException Exception thrown if it cannot connect to the server and port.
	 */
	public void connect(String server_addr, int port) throws IOException
	{
		socket = new Socket();

		InetSocketAddress sa = new InetSocketAddress(InetAddress.getByName(server_addr), port);

		socket.connect(sa, 2000);
		in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		out = new PrintWriter(socket.getOutputStream(), true);
	}
	
	///Closes the socket
	/**
	 * Closes the socket so we clean up things that we create.
	 * @throws IOException Exception thrown if the socket is already closed or if it is null
	 */
	public void close() throws IOException
	{
		socket.close();
	}
	
	///Send a String to Roomba
	/**
	 * Method sends a string message to the roomba.
	 * @param send String to send to the roomba.
	 * @param test Char value to specify whether testing or not.  0x01 = Testing
	 */
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
	///Sends bitfield to Roomba
	/**
	 * Sends a bitfield to the roomba, and waits to receive either position data, or a full
	 * suite (ir, object, position, bot sensors) of sensor data.
	 */
	public void send_bitfield()
	{
		out.print(bitfield);
		out.flush();
		
		//call receive, then check to see if the status bit has been set.  if it has, run receive_all.
		if(bitfield == 64|| status == 1){
			//print message to screen
			outputTxtCntr.print_string("Waiting to receive scan", false);
			outputTxtCntr.receive_all();
			
			
		} else {
			outputTxtCntr.print_string("Waiting to receive position", false);
			outputTxtCntr.receive_position();
		}
		
		if(status == 1){
			//print message to screen
			outputTxtCntr.print_string("Waiting to receive scan", false);
			outputTxtCntr.receive_all();
		}
	}
	
	///Sets the bitfield object when a button is used
	/**
	 * Method sets the bit based upon whether a button has been pressed or released.
	 * @param bit Int value for the bit which is to be sent
	 * @param pressed Boolean value as to whether the button was pressed or released
	 */
	public void set_button_bit(int bit, boolean pressed){// adds the button bit to the bitfield
		if(pressed){
			bitfield +=bit^2;
		}
		else{
			bitfield -= bit^2;
		}
	}
	
	///Sets the bitfield object when a trigger is used
	/**
	 * Method is called when a trigger is used.  Interprets the information about the button and
	 * sets the proper bits for the bitfield.
	 * @param speed Int value for the speed to be sent.
	 * @param isright Boolean value to determine whether the trigger is the left or right trigger.
	 * @param pressed Boolean value to determine whether it has been pressed or released.
	 */
	public void set_trigger_bit(int speed, boolean isright, boolean pressed){	// adds bits for the trigger to the bitfield
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
	///Receive the response from the Roomba
	/**
	 * Receives the response from the roomba when test value is 0x00.  When 0x01 it will send back
	 * preset data for testing purposes.  If it encounters an exception it prints the stack trace.
	 * @param test Char value used to determine whether the program is testing or not.
	 * @return String value from roomba.
	 */
	public String get_response(char test)
	{
		String resp = "Timeout";
		//add in option for different responses, and actual response from robot
		if(test == 0x00)
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
		} else if(test == 0x01){
			resp = "{\"ir\": [044,045,047,045,045,045,039,043,041,043,045,044,041,035,038,037,037,034,035,035,040,045,039,038,039,041,042,037,043,039,041,041,042,036,045,041,047,045,046,046,037,034,118,063,046,053,074,049,059,050,062,103,291,287,366,366,369,296,369,366,366,293,316,366,369,311,295,350,363,363,369,293,366,310,292,363,366,366,366,289,366,369,291,366,357,366,298,363,369,297,325,366,366,321,293,366,363,347,366,292,310,363,300,369,293,366,304,369,290,366,291,366,291,270,090,055,073,292,304,363,363,369,295,366,317,363,330,363,301,366,366,294,293,369,292,369,369,290,342,328,366,366,369,366,366,273,366,293,366,366,303,363,292,340,296,291,366,358,363,363,366,293,366,369,304,325,369,369,366,366,319,293,366,292,287,366,294,369,369,369]}";
		}
		
		return resp;
	}
	
	///Set the controller object
	/**
	 * Method to set the controller object.
	 * @param output OutputTextController object.
	 */
	public void set_controller(OutputTextController output)
	{
		outputTxtCntr = output;
	}
	
	///Sets the status bit
	/**
	 * Method to set the status bit.  This bit is used to say whether the receive function should
	 * expect a position or full suite (ir, object, position, bot sensor) of data.
	 * @param status Int value stating whether waiting for position(0) or full data(1)
	 */
	public void set_status(int status)
	{
		this.status = status;
	}
	
	///Test function
	/**
	 * Creates random values for the ir heatmap and places them in the proper place so the draw
	 * function can be called immediately after this function.
	 */
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
