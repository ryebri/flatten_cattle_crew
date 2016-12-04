package testing;

import ch.aplu.xboxcontroller.*;
	public class xboxcontrollerinterface {
		private XboxController xp;

		
		private class MyXboxControllerAdapter extends XboxControllerAdapter
		  {
			public void buttonA(boolean pressed)//scan
		    {
				send_bitfield(6, pressed);
			}

			public void buttonB(boolean pressed)// free from collision alert 
		    {
				send_bitfield(7,pressed);
		    }

			public void buttonX(boolean pressed)//swap between forward and reverse
		    {
				send_bitfield(9,pressed);
		    }

		    public void buttonY(boolean pressed)
		    {
				send_bitfield(11,pressed);
		    }

		    public void back(boolean pressed)
		    {
		    }

		    public void start(boolean pressed)
		    {
		    }

			public void leftShoulder(boolean pressed)// 90 degree left turn
		    {
				send_bitfield(9,pressed);
		    }

			public void rightShoulder(boolean pressed)// 90 degree right turn
		    {
				send_bitfield(10,pressed);
		    }

		    public void leftThumb(boolean pressed)
		    {
		    }

		    public void rightThumb(boolean pressed)
		    {
		    }

		    public void dpad(int direction, boolean pressed)
		    {
		    }

			public void leftTrigger(double value)// left wheel speed
		    {
				if(value > .5){
					send_bitfield(0,true);
				}
		    }

			public void rightTrigger(double value)// right wheel speed
		    {
				if(value > .5){
					send_bitfield(3,true);
				}
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

		public void test(){
		    xp = new XboxController("U:\\Desktop\\New folder (2)\\dll\\xboxcontroller64.dll",1,50,50);
		    xp.addXboxControllerListener(new MyXboxControllerAdapter());
		}
		
		
		public static void main(String[] args){
			xboxcontrollerinterface xca = new xboxcontrollerinterface();
			xca.test();
			
		}
	
}
