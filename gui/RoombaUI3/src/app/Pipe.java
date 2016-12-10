package app;

import java.awt.Point;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;
///Class of Pipe obstructions
/**
 * Class containing information about Pipe obstructions.
 * @author ryebri
 *
 */
public class Pipe extends Obstruction {
	
	private Point point;
	
	///Default Constructor
	/**
	 * Initializes everything to 0 and pipe
	 */
	public Pipe(){
		super(0, 0, 0);
		set_type(ObstructionType.LARGE_PIPE);
		point = new Point();
	}
	
	///Constructor
	/**
	 * Creates a Pipe object using the parameter values
	 * @param distance Int calculated distance away
	 * @param angle Int angle of object with respect to the roomba
	 * @param width Int calculated width of the object
	 */
	public Pipe(int distance, int angle, int width){
		super(distance, angle, width);
		set_type(ObstructionType.LARGE_PIPE);
		point = new Point();
	}
	
	///Draws the Pipe
	/**
	 * Draws the pipe object on the canvas.
	 * @param gc GraphicsContext object so that the pipe object can be drawn
	 */
	@Override
	public void drawShape(GraphicsContext gc){
		gc.setFill(Color.RED);
		gc.setStroke(Color.RED);
		gc.fillOval(point.getX(), point.getY(), 13, 13);
		gc.strokeOval(point.getX(), point.getY(), 13, 13);
	}
	
	/*
	 * getters
	 */
	///Get point Object
	/**
	 * Method to get the Point object.
	 * @return Point object 
	 */
	public Point get_point(){
		return point;
	}
	
	/*
	 * setters
	 */	
	///Set the point position where the Pipe will be drawn
	/**
	 * Sets the point position using x and y coordinates.
	 * @param x Int value x coordinate where it should be drawn on the canvas
	 * @param y Int value y coordinate where it should be drawn on the canvs
	 */
	public void set_point(int x, int y){
		point.setLocation(x, y);
	}
	
	///Set the point position  where the Pipe will be drawn
	/**
	 * Sets the point position using a Point object.
	 * @param p Point object storing the x and y coordinates where the Pipe should be drawn on the canvas
	 */
	public void set_point(Point p){
		point.setLocation(p);
	}
	

}
