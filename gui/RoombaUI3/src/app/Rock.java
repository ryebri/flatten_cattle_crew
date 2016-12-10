package app;

import java.awt.Point;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

///Class for rock objects
/**
 * Class which contains methods for drawing rocks
 * @author ryebri
 *
 */
public class Rock extends Obstruction {

	private Point point;
	
	///Default Constructor
	/**
	 * Creates a rock object with proper dimensions and a 0,0 location
	 */
	public Rock(){
		super(16, 0, 0);
		set_type(ObstructionType.ROCK);
		point = new Point();
	}
	
	///Constructor
	/**
	 * Creates a rock object with the distance, angle, and width.
	 * @param distance Int value of the distance away from the roomba
	 * @param angle Int angle of where the rock is (in relation to the roomba)
	 * @param width Int width of the rock
	 */
	public Rock(int distance, int angle, int width){
		super(distance, angle, width);
		set_type(ObstructionType.ROCK);
		point = new Point();
	}
	
	///Draw Rock
	/**
	 * Method to draw the rock object.
	 * @param gc GraphicContext object to be drawn upon
	 */
	@Override
	public void drawShape(GraphicsContext gc){
		gc.setFill(Color.BLUE);
		gc.setStroke(Color.BLUE);
		gc.fillOval(point.getX(), point.getY(), 13, 13);	//maybe set these to be a little larger?
		gc.strokeOval(point.getX(), point.getY(), 13, 13);
	}
	
	/*
	 * getters
	 */	
	///Gets the point object
	/**
	 * Method to get the point object.
	 * @return Point object where the rock is drawn on the canvas
	 */
	public Point get_point(){
		return point;
	}
	
	/*
	 * setters
	 */
	///Set the location
	/**
	 * Method to set the location of where the rock should be drawn on the canvas
	 * @param x Int x position on the canvas
	 * @param y Int y position on the canvas
	 */
	public void set_point(int x, int y){
		point.setLocation(x, y);
	}
	
	///Set the location
	/**
	 * Method to set the location of where the rock should be drawn on the canvas
	 * @param p Point where the rock should be drawn
	 */
	public void set_point(Point p){
		point.setLocation(p);
	}
	

}

