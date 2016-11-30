package app;

public class Obstruction {
	
	private int width;
	private double distance;
	private int angle;
	//add in xy coordinates?
	
	public Obstruction(int width, double distance, int angle){
		this.width = width;
		this.distance = distance;
		this.angle = angle;
	}
	
	public int get_width(){
		return width;
	}
	
	public double get_distance(){
		return distance;
	}
	
	public int get_angle(){
		return angle;
	}

}
