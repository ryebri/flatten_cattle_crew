package app;

import javafx.scene.canvas.GraphicsContext;

public class Obstruction {
	
	//might not be able to tell the difference between a large pipe and a small pipe
	public enum ObstructionType {
		LARGE_PIPE(0), SMALL_PIPE(1), ROCK(2), LINE(3), HOLE(4);
		
		private final int index;
		ObstructionType(int index) {this.index = index;}
		public int getValue() { return index; }
	}
	
	private ObstructionType type;
	private int distance;
	private int angle;
	private int width;
	
	public Obstruction(){
		distance = 0;
		angle = 0;
		width = 0;
		type = null;
	}
	
	public Obstruction(int distance, int angle, int width){
		this.distance = distance;
		this.angle = angle;
		this.width = width;
		
	}
	
	/* overriden in the children classes
	 * used to draw the shapes on the canvas
	 */
	public void drawShape(GraphicsContext gc){
		
	}
	
	/*
	 * getters
	 */
	public ObstructionType get_type(){
		return type;
	}
	
	public int get_angle(){
		return angle;
	}
	
	public int get_distance(){
		return distance;
	}
	
	public int get_width(){
		return width;
	}
	
	/*
	 * setters
	 */
	public void set_type(ObstructionType type){
		this.type = type;
	}
	
	public void set_angle(int angle){
		this.angle = angle;
	}
	
	public void set_distance(int distance){
		this.distance = distance;
	}
	
	public void set_width(int width){
		this.width = width;
	}
	

}
