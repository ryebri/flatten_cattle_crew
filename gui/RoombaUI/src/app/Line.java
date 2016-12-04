package app;

import java.awt.Point;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Line extends Obstruction {

	private Point p1;
	private Point p2;
	
	public Line(){
		super(0,0,0);
		set_type(ObstructionType.LINE);
		p1 = null;
		p2 = null;
	}
	
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
	public Point get_p1(){
		return p1;
	}
	
	public Point get_p2(){
		return p2;
	}
	
	
	/*
	 * setters
	 */
	public void set_p1(int x, int y){
		p1 = new Point(x,y);
	}
	
	public void set_p2(int x, int y){
		p2 = new Point(x,y);
	}
	
}
