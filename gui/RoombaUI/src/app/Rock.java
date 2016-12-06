package app;

import java.awt.Point;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Rock extends Obstruction {

	private Point point;
	
	public Rock(){
		super(0, 0, 0);
		set_type(ObstructionType.ROCK);
		point = new Point();
	}
	
	public Rock(int distance, int angle, int width){
		super(distance, angle, width);
		set_type(ObstructionType.ROCK);
		point = new Point();
	}
	
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
	public Point get_point(){
		return point;
	}
	
	/*
	 * setters
	 */
	public void set_point(int x, int y){
		point.setLocation(x, y);
	}
	
	public void set_point(Point p){
		point.setLocation(p);
	}
	

}

