package testing;

import ch.aplu.xboxcontroller.*;
	public class xboxcontrollerinterface {
		private XboxController xp;

		
		private class MyXboxControllerAdapter extends XboxControllerAdapter
		  {
			public void buttonA(boolean pressed)//scan
		    {
		    	System.out.println("world");
		    }

			public void buttonB(boolean pressed)// free from collision alert 
		    {
		    }

			public void buttonX(boolean pressed)//swap between forward and reverse
		    {
		    }

		    public void buttonY(boolean pressed)
		    {
		    }

		    public void back(boolean pressed)
		    {
		    }

		    public void start(boolean pressed)
		    {
		    }

			public void leftShoulder(boolean pressed)// 90 degree left turn
		    {
		    }

			public void rightShoulder(boolean pressed)// 90 degree right turn
		    {
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

		public void test(){
		    xp = new XboxController("U:\\Desktop\\New folder (2)\\dll\\xboxcontroller64.dll",1,50,50);
		    xp.addXboxControllerListener(new MyXboxControllerAdapter());
		}
		
		
		public static void main(String[] args){
			xboxcontrollerinterface xca = new xboxcontrollerinterface();
			xca.test();
			
		}
	
}
