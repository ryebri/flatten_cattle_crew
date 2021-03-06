package app;

import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;

public class ResizableCanvas extends Canvas {
	public ResizableCanvas(){
		widthProperty().addListener(evt -> draw());
		heightProperty().addListener(evt -> draw());
	}
	
	private void draw() {
		double width = getWidth();
		double height = getHeight();
		
		GraphicsContext gc = getGraphicsContext2D();
		gc.clearRect(0, 0, width, height);
		
	}
	
	@Override
	public boolean isResizable(){
		return true;
	}
	
	@Override
	public double prefWidth(double width){
		return getWidth();
	}
	
	@Override
	public double prefHeight(double height){
		return getHeight();
	}
}
