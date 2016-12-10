/**
 * @file Hole.java
 * @brief Class containing information about the hole object
 * @author ryebri
 * @date 12/06/2016
 */
package app;

import java.awt.Point;

import app.Obstruction.ObstructionType;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Hole extends Obstruction {
	private final int WIDTH = 60;
	private Point point;
	
	///Default Constructor
	/**
	 * Initializes everything to 0 or null
	 */
	public Hole(){
		super(0, 0, 60);
		set_type(ObstructionType.HOLE);
		point = new Point();
	}
	
	///Constructor
	/**
	 * Initializes everything with given values from the parameters
	 * @param x Int x coordinate where the hole was discovered
	 * @param y Int y coordinate where the hole was discovered
	 */
	public Hole(int x, int y){
		super(0, 0, 60);
		set_type(ObstructionType.HOLE);
		point = new Point();
	}
	///Draws the Pipe
	/**
	 * Draws the pipe object on the canvas.
	 * @param gc GraphicsContext object so that the pipe object can be drawn
	 */
	@Override
	public void drawShape(GraphicsContext gc){
		gc.setFill(Color.ORANGE);
		gc.setStroke(Color.ORANGE);
		gc.fillRect(point.getX(), point.getY(), 60, 60);
		gc.strokeRect(point.getX(), point.getY(), 60, 60);
	}
	
	/*
	 * getters
	 */
	///Get point
	/**
	 * Gets the point where the hole was first detected.
	 * @return Point object containing x,y coordinate where the hole was detected
	 */
	public Point get_point(){
		return point;
	}
	
	/*
	 * setters
	 */
	///Set the point
	/**
	 * Sets the original point where the hole was detected.
	 * @param x Int x coordinate where the hole was detected
	 * @param y Int y coordinate where the hole was detected
	 */
	public void set_point(int x, int y){
		point.setLocation(x, y);
	}
	
	///Set the point
	/**
	 * Sets the original point where the hole was detected.
	 * @param p Point containing the x,y coordinates where the hole was detected
	 */
	public void set_point(Point p){
		point.setLocation(p);
	}
}
