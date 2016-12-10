/**
 * @file xboxcontrollerinterface.java
 * @brief An interface to interpret the xbox controller information, and send instructions
 * to the roomba via the RoombaComm object.
 * @author ryebri
 * @date 12/06/2016
 */
package app;

import ch.aplu.xboxcontroller.XboxController;
import ch.aplu.xboxcontroller.XboxControllerAdapter;
	
///Xbox Controller Interface
public class xboxcontrollerinterface {
	private XboxController xp;
	private RoombaComm rc;
	
	///Xbox Controller Adapter
	/**
	 * Each function is called when a button/trigger is called, sends the proper command
	 * to the roomba via RoombaComm object.
	 */
	private class MyXboxControllerAdapter extends XboxControllerAdapter
	  {
		private boolean forward = true;
		///Button A
		public void buttonA(boolean pressed)//scan
	    {
			rc.set_button_bit(6, pressed);
			if(pressed == true){
	    		rc.send_bitfield();
	    	}
		}
		
		///Button B
		public void buttonB(boolean pressed)// free from collision alert 
	    {
			rc.set_button_bit(7, pressed);
			if(pressed == true){
	    		rc.send_bitfield();
	    	}
	    }
		
		///Button X
		public void buttonX(boolean pressed)//swap between forward and reverse
	    {
			if(forward)forward = false;
			else forward = true;
	    }

		///Button Y
	    public void buttonY(boolean pressed)
	    {
	    	rc.set_button_bit(11, pressed);
	    	if(pressed == true){
	    		rc.send_bitfield();
	    	}
	    }

	    ///Button back
	    public void back(boolean pressed)
	    {
	    }

	    ///Button start
	    public void start(boolean pressed)
	    {
	    }

	    ///Left Shoulder
		public void leftShoulder(boolean pressed)// 90 degree left turn
	    {
			rc.set_button_bit(9, pressed);
			if(pressed == true){
				rc.send_bitfield();
			}
	    }

		///Right Shoulder
		public void rightShoulder(boolean pressed)// 90 degree right turn
	    {
			rc.set_button_bit(10, pressed);
			if(pressed == true){
	    		rc.send_bitfield();
	    	}
	    }

		///Left Thumb
	    public void leftThumb(boolean pressed)
	    {
	    }

	    ///Right Thumb
	    public void rightThumb(boolean pressed)
	    {
	    }

	    ///Dpad 
	    public void dpad(int direction, boolean pressed)//0 = up , 4 = down
	    {
	    	if(pressed){
		    	if(direction == 0){
					rc.set_button_bit(1, pressed);
					rc.set_button_bit(4, pressed);
		    	}
		    	else if(direction == 4){
					rc.set_button_bit(2, pressed);
					rc.set_button_bit(5, pressed);
		    	}
	    		rc.send_bitfield();
	    	}
	    	else{
				rc.set_button_bit(0, pressed);
				}
	    }

	    ///Left Trigger
		public void leftTrigger(double value)// left wheel speed
	    {
			
	    }

		///Right Trigger
		public void rightTrigger(double value)// right wheel speed
	    {

	    }

		///Left Thumb Magnitude
	    public void leftThumbMagnitude(double magnitude)
	    {
	    }

	    ///Left Thumb Direction
	    public void leftThumbDirection(double direction)
	    {
	    }

	    ///Right Thumb Magnitude
	    public void rightThumbMagnitude(double magnitude)
	    {
	    }

	    ///Right Thumb Direction
	    public void rightThumbDirection(double direction)
	    {
	    }
	    
	    ///Is connected
	    public void isConnected(boolean connected)
	    {
	    	System.out.println("connected");
	    }
	  }

	///Constructor
	/**
	 * Creates a xboxcontrollerinterface and initializes it.
	 * @param rc RoombaComm object for communication
	 */
	public xboxcontrollerinterface(RoombaComm rc){
//			xboxcontrollerinterface xca = new xboxcontrollerinterface();
		this.rc = rc;
	    xp = new XboxController("D:\\Documents\\flatten_cattle_crew\\dll\\xboxcontroller64.dll",1,50,50);
	    xp.addXboxControllerListener(new MyXboxControllerAdapter());
	}
	
	///Set Roomba Control
	/**
	 * Intended to pass a new communication object in the event that we needed to reboot
	 * the connection/do a hard reset.  Unfortunately we were never able to successfully
	 * disconnect and then reconnect.
	 * @param rc RoombaComm object for communication
	 */
	public void set_roomba_control(RoombaComm rc){
		this.rc = rc;
	}
	
	//for testing purposes
//	public static void main(String[] args){
//		xboxcontrollerinterface xca = new xboxcontrollerinterface();
//		xca.test();
//		
//	}

}
