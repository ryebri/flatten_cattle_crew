package app;

import javafx.scene.canvas.GraphicsContext;

///Super class Obstructions
/**
 * Super class that all of the obstruction objects extend.  Allows 1 obstruction array to
 * be used instead of multiple arrays.
 * @author ryebri
 *
 */
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
	
	///Default Constructor
	/**
	 * Creates a default object and initializes variables to 0/null.
	 */
	public Obstruction(){
		distance = 0;
		angle = 0;
		width = 0;
		type = null;
	}
	
	///Constructor
	/**
	 * Creates an object from the given parameters.
	 * @param distance Int calculated distance away
	 * @param angle Int angle of object with respect to the roomba
	 * @param width Int calculated width of the object
	 */
	public Obstruction(int distance, int angle, int width){
		this.distance = distance;
		this.angle = angle;
		this.width = width;
		
	}
	
	/* overriden in the children classes
	 * used to draw the shapes on the canvas
	 */
	///Draw Shape method
	/**
	 * Overriden method by all classes that implement an Obstruction.
	 * @param gc GraphicsContext object used to draw objects on the canvas
	 */
	public void drawShape(GraphicsContext gc){
		
	}
	
	/*
	 * getters
	 */
	///Get type of Obstruction
	/**
	 * Gets the type of the Obstruction object
	 * @return ObstructionType object.
	 */
	public ObstructionType get_type(){
		return type;
	}
	
	///Get the angle
	/**
	 * Gets the angle of the object with respect to the roomba
	 * @return Int angle of object with respect to the roomba
	 */
	public int get_angle(){
		return angle;
	}
	
	///Get the distance
	/**
	 * Gets the calculated distance from the object.
	 * @return Int distance
	 */
	public int get_distance(){
		return distance;
	}
	
	///Get the width
	/**
	 * Gets the calculated width of the object
	 * @return Int width of the object
	 */
	public int get_width(){
		return width;
	}
	
	/*
	 * setters
	 */
	///Sets the type of Obstruction
	/**
	 * Set the type of Obstruction this will be.
	 * @param type ObstructionType object
	 */
	public void set_type(ObstructionType type){
		this.type = type;
	}
	
	///Sets the angle of the Obstruction
	/**
	 * Set the angle of the object away from the roomba.
	 * @param angle Int angle of the object in reference to the roomba
	 */
	public void set_angle(int angle){
		this.angle = angle;
	}
	
	///Sets the distance away from the object
	/**
	 * Sets the distance way from the  object/roomba.
	 * @param distance Int value distance between roomba and object
	 */
	public void set_distance(int distance){
		this.distance = distance;
	}
	
	///Sets the width of the object
	/**
	 * Sets the calculated width of the object.
	 * @param width Int value of the width of the object
	 */
	public void set_width(int width){
		this.width = width;
	}
	

}
