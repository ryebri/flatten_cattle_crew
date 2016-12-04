package app;

import java.awt.Point;

import app.Obstruction.ObstructionType;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Hole extends Obstruction {
	private final int WIDTH = 60;
	private Point point;
	
	public Hole(){
		super(0, 0, 60);
		set_type(ObstructionType.HOLE);
		point = null;
	}
	
	public Hole(int x, int y){
		super(0, 0, 60);
		set_type(ObstructionType.HOLE);
		point = null;
	}
	
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
	public Point get_point(){
		return point;
	}
	
	/*
	 * setters
	 */
	public void set_point(int x, int y){
		point = new Point(x,y);
	}
}
