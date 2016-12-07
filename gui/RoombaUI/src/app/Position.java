package app;

import java.awt.Point;

public class Position {
	
	public enum CardinalDirection {
		NORTH(0), SOUTH(1), EAST(2), WEST(3);
		
		private final int index;
		CardinalDirection(int index) {this.index = index;}
		public int getValue() { return index; }
	}
	private final int x_start = 430;
	private final int y_start = 275;
	private int[] total_traveled;	//in each direction
	private Point prev_position;	// xy position
	private Point curr_position;	// xy position
	private int orientation;
	
	public Position(){
		total_traveled = new int[]{0,0,0,0};
		prev_position = new Point(x_start, y_start);	//middle of the canvas
		curr_position = new Point(x_start, y_start);
		orientation = 0;
	}
	
	//orientation has north at 0
	public void update_positions(int x, int x_negative, int y, int y_negative, int orientation){
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
	}
	public void update_positions(int[] pos){
		//may need to have more logic here if the robot does not take care of things
		set_total_traveled(pos[0], pos[1], pos[2], pos[3]);
		set_prev_position(curr_position.x, curr_position.y);
		set_curr_position(pos);
		this.orientation = pos[4];
	}
	
	/*
	 * getters
	 */
	public Point get_prev_position(){
		return prev_position;
	}
	
	public Point get_curr_position(){
		return curr_position;
	}
	
	public int[] get_total_traveled(){
		return total_traveled;
	}
	
	public int get_orientation(){
		return orientation;
	}
	
	public int get_start_x(){
		return x_start;
	}
	
	public int get_start_y(){
		return y_start;
	}
	
	/*
	 * setters
	 */
	public void set_prev_position(int x, int y){
		prev_position.setLocation(x, y);
	}
	
	public void set_curr_position(int x, int y){
		curr_position.setLocation(x, y);
	}
	
	public void set_curr_position(int position[]){
		int new_x, new_y;
		if(position[1] == 0){
			new_x = curr_position.x + position[0];
		} else {
			new_x = curr_position.x - position[0];
		}
		
		if(position[3] == 0){
			new_y = curr_position.y + position[2];
		} else {
			new_y = curr_position.y - position[2];
		}
		
		curr_position.setLocation(new_x, new_y);
	}
	
	public void set_total_traveled(int x, int x_neg, int y, int y_neg){
		if(x_neg == 0){
			total_traveled[CardinalDirection.NORTH.index] += x;
		} else {
			total_traveled[CardinalDirection.SOUTH.index] += x;
		}
		
		if(y_neg == 0){
			total_traveled[CardinalDirection.EAST.index] += y;
		} else {
			total_traveled[CardinalDirection.WEST.index] += y;
		}
	}
	
	public void set_orientation(int orientation){
		this.orientation = orientation;
	}
}
