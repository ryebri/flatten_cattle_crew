/**
 * @file Line.java
 * @brief This is a class containing information on Lines being drawn on the canvas
 * @author ryebri
 * @date 12/06/2016
 */
package app;

import java.awt.Point;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;


public class Line extends Obstruction {

	private Point p1;
	private Point p2;
	
	///Default Constructor
	/**
	 * Creates an initialized object, everything is initialized to either 0 or null.
	 */
	public Line(){
		super(0,0,0);
		set_type(ObstructionType.LINE);
		p1 = new Point();
		p2 = new Point();
	}
	
	///Constructor
	/**
	 * Creates a Line object based upon the passed in parameters
	 * @param x1 Int x coordinate on the canvas for first point
	 * @param y1 Int y coordinate on the canvas for first point
	 * @param x2 Int x coordinate on the canvas for second point
	 * @param y2 Int y coordinate on the canvas for second point
	 */
	public Line(int x1, int y1, int x2, int y2){
		super(0,0,0);
		set_type(ObstructionType.LINE);
		p1 = new Point(x1, y1);
		p2 = new Point(x2, y2);
	}
	
	@Override
	public void drawShape(GraphicsContext gc){
		gc.setStroke(Color.WHITE);
		gc.setLineWidth(3.0);
		gc.strokeLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
	}
	
	/*
	 * getters
	 */
	///Get Point 1
	/**
	 * Get point 1 of the line.
	 * @return Point 1 of the line
	 */
	public Point get_p1(){
		return p1;
	}
	
	///Get Point 2
	/**
	 * Get Point 1 of the line.
	 * @return Point 1 of the line
	 */
	public Point get_p2(){
		return p2;
	}
	
	
	/*
	 * setters
	 */
	///Set Point 1
	/**
	 * Set point 1 using integer values.
	 * @param x Int for the x coordinate
	 * @param y Int for the y coordinate
	 */
	public void set_p1(int x, int y){
		p1.setLocation(x, y);
	}
	
	///Set Point 1
	/**
	 * Set point 1 using a Point object.
	 * @param p Point object containing the x,y coordinates for the object
	 */
	public void set_p1(Point p){
		p1.setLocation(p);
	}
	
	///Set Point 2
	/**
	 * Set Point 2 using int values for x and y coordinates.
	 * @param x Int x coordinate
	 * @param y Int y coordinate
	 */
	public void set_p2(int x, int y){
		p2.setLocation(x, y);
	}
	
	///Set Point 2
	/**
	 * Set Point 2 using a Point object.
	 * @param p Point object containing the x,y coordinates for the object
	 */
	public void set_p2(Point p){
		p2.setLocation(p);
	}
	
	
}
