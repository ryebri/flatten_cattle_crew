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
	public void update_positions(int north, int south, int east, int west, int orientation){
		set_total_traveled(north, south, east, west);
		set_prev_position(curr_position.x, curr_position.y);
		set_curr_position((x_start - north + south), (y_start - west + east));
		this.orientation = orientation;
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
	
	/*
	 * setters
	 */
	public void set_prev_position(int x, int y){
		prev_position.setLocation(x, y);
	}
	
	public void set_curr_position(int x, int y){
		curr_position.setLocation(x, y);
	}
	
	public void set_total_traveled(int north, int south, int east, int west){
		total_traveled[CardinalDirection.NORTH.index] = north;
		total_traveled[CardinalDirection.SOUTH.index] = south;
		total_traveled[CardinalDirection.EAST.index] = east;
		total_traveled[CardinalDirection.WEST.index] = west;
	}
	
	public void set_orientation(int orientation){
		this.orientation = orientation;
	}
}
