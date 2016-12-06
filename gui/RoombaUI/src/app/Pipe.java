package app;

import java.awt.Point;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Pipe extends Obstruction {
	
	private Point point;
	
	public Pipe(){
		super(0, 0, 0);
		set_type(ObstructionType.LARGE_PIPE);
		point = new Point();
	}
	
	public Pipe(int distance, int angle, int width){
		super(distance, angle, width);
		set_type(ObstructionType.LARGE_PIPE);
		point = new Point();
	}
	
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
