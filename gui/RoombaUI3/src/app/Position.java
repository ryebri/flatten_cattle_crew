package app;

import java.awt.Point;

///Position Class Object
/**
 * Class which keeps track of where the roomba is, and where it is on the canvas.
 * @author ryebri
 *
 */
public class Position {
	
	///Enum for the cardinal directions
	/**
	 * Enum which contains the information on where each location is located in the position array.
	 * @author ryebri
	 *
	 */
	public enum CardinalDirection {
		NORTH(0), SOUTH(1), EAST(2), WEST(3);
		
		private final int index;
		CardinalDirection(int index) {this.index = index;}
		public int getValue() { return index; }
	}
	
	private int x_start = 430;
	private int y_start = 275;
	private int[] total_traveled;	//in each direction
	private Point prev_position;	// xy position
	private Point curr_position;	// xy position
	private int orientation;		// orientation of the robot
	private int status;				//status bit used to say whether it hit something or not
	
	///Default Constructor
	/**
	 * Initializes the position private variables to their proper values.
	 */
	public Position(){
		total_traveled = new int[]{0,0,0,0};
		prev_position = new Point(x_start, y_start);	//middle of the canvas
		curr_position = new Point(x_start, y_start);
		orientation = 0;
		status = 0;
	}
	
	//orientation has north at 0
	///Update position data
	/**
	 * Method to update the position variables.  Will update previous location, and current,
	 * also includes a status bit to determine whether the gui next time should listen for just
	 * position data, or the full suite (ir, object, position, bot sensors) of data.
	 * @param x Int value of distance traveled in the x direction
	 * @param x_negative Int value acts as a boolean to determine if x is negative (1 = negative, 0 = positive).
	 * @param y Int value of distance traveled in the y direction
	 * @param y_negative Int value acts as a boolean to determine if y is negative (1 = negative, 0 = positive).
	 * @param orientation Int value of the orientation of the roomba (0 is "north")
	 * @param status Int value determining the status of what the roomba should listen for (0 = position, 1 = full data suite)
	 */
	public void update_positions(int x, int x_negative, int y, int y_negative, int orientation, int status){
		//may need to have more logic here if the robot does not take care of things
		int new_x, new_y;
		if(x_negative == 0){
			new_x = curr_position.x + x;
		} else {
			new_x = curr_position.x - x;
		}
		
		if(y_negative == 0){
			new_y = curr_position.y + y;
		} else {
			new_y = curr_position.y - y;
		}
		set_total_traveled(x, x_negative, y, y_negative);
		set_prev_position(curr_position.x, curr_position.y);
		set_curr_position(new_x, new_y);
		this.orientation = orientation;
		this.status = status;
	}
	
	///Updates position from an array
	/**
	 * Updates the position variables based upon values in the parameter int[].
	 * @param pos int[] containing the 6 different values used by this class.
	 */
	public void update_positions(int[] pos){
		//may need to have more logic here if the robot does not take care of things
		set_total_traveled(pos[0], pos[1], pos[2], pos[3]);
		set_prev_position(curr_position.x, curr_position.y);
		set_curr_position(pos);
		this.orientation = pos[4];
		status = pos[5];
	}
	
	/*
	 * getters
	 */
	///Get previous position
	/**
	 * Method to get the previous position.
	 * @return Point object containing the previous position.
	 */
	public Point get_prev_position(){
		return prev_position;
	}
	
	///Get the current position
	/**
	 * Method to get the current position.
	 * @return Point object containing the current position.
	 */
	public Point get_curr_position(){
		return curr_position;
	}
	
	///Get total traveled
	/**
	 * Method to get the total traveled.  Returns an array with NORTH, SOUTH, EAST, and WEST data
	 * @return int[] of the total traveled in each of the 4 directions NORTH, SOUTH, EAST, WEST.
	 */
	public int[] get_total_traveled(){
		return total_traveled;
	}
	
	///Get current orientation
	/**
	 * Method to get the current orientation of the roomba.
	 * @return Int value, 0 - 360, of where the roomba is pointed.
	 */
	public int get_orientation(){
		return orientation;
	}
	
	///Get the start position of x
	/**
	 * Method to get the start canvas x position of the roomba.
	 * @return Int value of the starting canvas x position of the roomba.
	 */
	public int get_start_x(){
		return x_start;
	}
	
	///Get the start position of y
	/**
	 * Method to get the start canvas y position of the roomba.
	 * @return Int value of the starting canvas y position of the roomba.
	 */
	public int get_start_y(){
		return y_start;
	}
	
	///Get the status value
	/**
	 * Method to get the status value.  Status value is used to determine whether the program
	 * should listen for just position data, or the full suite (ir, object, position, bot sensors)
	 * of data.
	 * @return Int value, 0 = position data, 1 = full suite data.
	 */
	public int get_status(){
		return status;
	}
	
	/*
	 * setters
	 */
	///Set previous position
	/**
	 * Method to set the previous position using an x and y points.
	 * @param x Int value of the previous x position on the canvas.
	 * @param y Int value of the previous y position on the canvas.
	 */
	public void set_prev_position(int x, int y){
		prev_position.setLocation(x, y);
	}
	///Set current position point
	/**
	 * Method to set the current position using an x and y points.
	 * @param x Int value of the current x position on the canvas.
	 * @param y Int value of the current y position on the canvas.
	 */
	public void set_curr_position(int x, int y){
		curr_position.setLocation(x/100, y/100);
	}
	///Set current position point
	/**
	 * Method to set the current position using the position array.
	 * @param position Int[] containing the values that would be used to call update_position method.
	 */
	public void set_curr_position(int position[]){
		int new_x, new_y;
		if(position[1] == 1){
//			new_x = curr_position.x + position[0]/100;
			new_x = x_start + position[0]/100;
		} else {
//			new_x = curr_position.x - position[0]/100;
			new_x = x_start - position[0]/100;
		}
		
		if(position[3] == 1){
//			new_y = curr_position.y + position[2]/100;
			new_y = y_start + position[2]/100;
		} else {
//			new_y = curr_position.y - position[2]/100;
			new_y = y_start - position[2]/100;
		}
		
		curr_position.setLocation(new_x, new_y);
	}
	
	///Set the total traveled distance array
	/**
	 * Method to update the total traveled distance array.
	 * @param x 
	 * @param x Int value of distance traveled in the x direction
	 * @param x_negative Int value acts as a boolean to determine if x is negative (1 = negative, 0 = positive).
	 * @param y Int value of distance traveled in the y direction
	 * @param y_negative Int value acts as a boolean to determine if y is negative (1 = negative, 0 = positive).
	 */
	public void set_total_traveled(int x, int x_neg, int y, int y_neg){
		if(x_neg == 0){
			total_traveled[CardinalDirection.NORTH.index] += x/100;
		} else {
			total_traveled[CardinalDirection.SOUTH.index] += x/100;
		}
		
		if(y_neg == 0){
			total_traveled[CardinalDirection.EAST.index] += y/100;
		} else {
			total_traveled[CardinalDirection.WEST.index] += y/100;
		}
	}
	
	///Set the orientation variable
	/**
	 * Method to set the orientation of the roomba.
	 * @param orientation Int value of the orientation (0 - 360)
	 */
	public void set_orientation(int orientation){
		this.orientation = orientation;
	}
	
	///Set the status variable
	/**
	 * Set the status variable.  Status value is used to determine whether the program
	 * should listen for just position data, or the full suite (ir, object, position, bot sensors)
	 * of data.
	 * @return Int value, 0 = position data, 1 = full suite data.
	 */
	public void set_status(int status){
		this.status = status;
	}
	
	///Set the starting x position variable
	/**
	 * Used when you need to shift the canvas, changes the coordinate which everything is
	 * drawn from.
	 * @param x Int new x coordinate for start position
	 */
	public void set_start_x(int x){
		x_start = x;
	}
	
	///Set the starting y position variable
	/**
	 * Used when you need to shift the canvas, changes the coordinate which everything is
	 * drawn from.
	 * @param y Int new y coordinate for start position
	 */
	public void set_start_y(int y){
		y_start = y;
	}
}
