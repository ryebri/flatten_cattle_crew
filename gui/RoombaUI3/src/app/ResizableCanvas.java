/**
 * @file ResizableCanvas.java
 * @brief Class not used.  Original intent was to allow for flexibility of the UI so that
 * the user could change the size of the UI.  Unfortunately was not vital to core functionality
 * and time did not allow for full implementation to occur.  UI must remain at start size.
 * @author ryebri
 * @date 12/06/2016
 */
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
