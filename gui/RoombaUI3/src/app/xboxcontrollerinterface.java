package app;

import ch.aplu.xboxcontroller.XboxController;
import ch.aplu.xboxcontroller.XboxControllerAdapter;
	public class xboxcontrollerinterface {
		private XboxController xp;
		private RoombaComm rc;
		
		private class MyXboxControllerAdapter extends XboxControllerAdapter
		  {
			private boolean forward = true;
			public void buttonA(boolean pressed)//scan
		    {
				rc.set_button_bit(6, pressed);
				if(pressed == true){
		    		rc.send_bitfield();
		    	}
			}

			public void buttonB(boolean pressed)// free from collision alert 
		    {
				rc.set_button_bit(7, pressed);
				if(pressed == true){
		    		rc.send_bitfield();
		    	}
		    }

			public void buttonX(boolean pressed)//swap between forward and reverse
		    {
				if(forward)forward = false;
				else forward = true;
		    }

		    public void buttonY(boolean pressed)
		    {
		    	rc.set_button_bit(11, pressed);
		    	if(pressed == true){
		    		rc.send_bitfield();
		    	}
		    }

		    public void back(boolean pressed)
		    {
		    }

		    public void start(boolean pressed)
		    {
		    }

			public void leftShoulder(boolean pressed)// 90 degree left turn
		    {
				rc.set_button_bit(9, pressed);
				if(pressed == true){
					rc.send_bitfield();
				}
		    }

			public void rightShoulder(boolean pressed)// 90 degree right turn
		    {
				rc.set_button_bit(10, pressed);
				if(pressed == true){
		    		rc.send_bitfield();
		    	}
		    }

		    public void leftThumb(boolean pressed)
		    {
		    }

		    public void rightThumb(boolean pressed)
		    {
		    }

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

			public void leftTrigger(double value)// left wheel speed
		    {
				
		    }

			public void rightTrigger(double value)// right wheel speed
		    {

		    }

		    public void leftThumbMagnitude(double magnitude)
		    {
		    }

		    public void leftThumbDirection(double direction)
		    {
		    }

		    public void rightThumbMagnitude(double magnitude)
		    {
		    }

		    public void rightThumbDirection(double direction)
		    {
		    }
		    
		    public void isConnected(boolean connected)
		    {
		    	System.out.println("connected");
		    }
		  }

		public xboxcontrollerinterface(RoombaComm rc){
//			xboxcontrollerinterface xca = new xboxcontrollerinterface();
			this.rc = rc;
		    xp = new XboxController("D:\\Documents\\flatten_cattle_crew\\dll\\xboxcontroller64.dll",1,50,50);
		    xp.addXboxControllerListener(new MyXboxControllerAdapter());
		}
		
		public void set_roomba_control(RoombaComm rc){
			this.rc = rc;
		}
		
		
//		public static void main(String[] args){
//			xboxcontrollerinterface xca = new xboxcontrollerinterface();
//			xca.test();
//			
//		}
	
}
