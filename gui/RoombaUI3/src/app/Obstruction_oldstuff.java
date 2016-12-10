/**
 * @file Obstruction_oldstuff.java
 * @brief This class was created as the original Obstruction class, but was not used in the
 * final project.  Initially planned on having all obstructions be obstructions instead of
 * just extending a super class.  It separates out the functionality a bit.
 * @author ryebri
 * @date 12/06/2016
 */
package app;

import java.awt.Point;

public class Obstruction_oldstuff {
	
	private int width;			//calculated width of object
	private double distance;	//distance from bot
	private int angle;			//angle from bot
	private Point point;		//point on the canvas
	private ObstructionType type;
	
	//might not be able to tell the difference between a large pipe and a small pipe
	public enum ObstructionType {
		LARGE_PIPE(0), SMALL_PIPE(1), ROCK(2), LINE(3), HOLE(4);
		
		private final int index;
		ObstructionType(int index) {this.index = index;}
		public int getValue() { return index; }
	}
	
	public Obstruction_oldstuff(){
		this.width = -1;
		this.distance = -1;
		this.angle = -1;
		this.point = null;
		this.type = null;
	}
	
	public Obstruction_oldstuff(int width, double distance, int angle){
		this.width = width;
		this.distance = distance;
		this.angle = angle;
		this.point = null;
		this.type = null;
	}
	
	/*
	 * get the width, distance, angle, point, type
	 */
	public int get_width(){
		return width;
	}
	
	public double get_distance(){
		return distance;
	}
	
	public int get_angle(){
		return angle;
	}
	
	public Point get_point(){
		return point;
	}
	
	public ObstructionType get_type(){
		return type;
	}
	
	/*
	 * Set the width, distance, angle, point, type
	 */
	public void set_width(int width){
		this.width = width;
	}
	
	public void set_distance(double distance){
		this.distance = distance;
	}
	
	public void set_angle(int angle){
		this.angle = angle;
	}
	
	public void set_point(int x, int y){
		this.point = new Point(x, y);
	}
	
	public void set_type(ObstructionType type){
		this.type = type;
	}
}
