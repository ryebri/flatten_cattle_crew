package app.view;

import app.SensorData;
import app.MainApp;
import app.model.TextOutput;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.paint.Color;

public class OutputTextController {
	@FXML
	private TableView<TextOutput> outputList;
	
	@FXML
	private TableColumn<TextOutput, String> firstColumn;
	
	@FXML
	private TextField cmdLineBox;
	
	@FXML
	private Canvas sensor_map;
	
	@FXML
	private Canvas area_map;
	
	private MainApp mainApp;
	//light red		Color.rgb(255,94,94)
	//red			Color.rgb(255,0,0)
	//dark red		Color.rgb(180,0,0)
	//dark orange	Color.rgb(209,90,0)
	//orange		Color.rgb(255,110,0)
	//light orange	Color.rgb(255,150,0)
	//dark yellow	Color.rgb(211,211,0)
	//yellow		Color.rgb(255,255,0)
	//light yellow	Color.rgb(255,255,70)
	//light green	Color.rgb(80,255,0)
	//green			Color.rgb(0,255,0)
	//dark green	Color.rgb(0,170,0)
	//light indigo	Color.rgb(107,255,255)
	//indigo		Color.rgb(0,255,255)
	//dark indigo	Color.rgb(0,140,140)
	//dark blue		Color.rgb(0,25,140)
	//blue			Color.rgb(0,0,255)
	//light blue	Color.rgb(90,90,255)
	//dark violet	Color.rgb(140,0,140)
	//violet		Color.rgb(255,0,255)
	//light violet	Color.rgb(255,107,255)
	
	Color colors[] = {Color.rgb(180,0,0), Color.rgb(255,0,0), Color.rgb(255,94,94), 
			Color.rgb(209,90,0), Color.rgb(255,110,0), Color.rgb(255,150,0),
			Color.rgb(211,211,0), Color.rgb(255,255,0), Color.rgb(255,255,127), 
			Color.rgb(150,255,150), Color.rgb(0,255,0), Color.rgb(0,170,0), 
			Color.rgb(160,255,255), Color.rgb(0,255,255), Color.rgb(0,170,170), 
			Color.rgb(90,90,255), Color.rgb(0,0,255), Color.rgb(0,25,140), 
			Color.rgb(255,107,255), Color.rgb(255,0,255),Color.rgb(140,0,140)}; 
	
	public enum Colors {DARK_RED (0), RED (1), LIGHT_RED (2), DARK_ORANGE (3), ORANGE (4), LIGHT_ORANGE (5),
						DARK_YELLOW (6), YELLOW (7), LIGHT_YELLOW (8), LIGHT_GREEN (9), GREEN (10), DARK_GREEN (11),
						LIGHT_INDIGO (12), INDIGO (13), DARK_INDIGO (14), LIGHT_BLUE (15), BLUE (16), DARK_BLUE (17), 
						LIGHT_VIOLET (18), VIOLET (19), DARK_VIOLET (20);
		private int numVal;
		
		Colors(int numVal){
			this.numVal = numVal;
		}
	}
	int ir_reading[] = new int[180];
	
	public OutputTextController() {
		
	}
	
	//change from a list to a table view?
	@FXML
	private void initialize(){
		//Do the initialization, need internet for it
		//add listener to text box
		firstColumn.setCellValueFactory(cellData -> cellData.getValue().output());
	}
	
	public void setMainApp(MainApp mainApp){
		this.mainApp = mainApp;
		outputList.setItems(mainApp.getOutputData());
//		sensor_map.setWidth(value);
        this.mainApp.getPrimaryStage().getScene().widthProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth, Number newSceneWidth) {
                System.out.println("Width: " + newSceneWidth);
//                sensor_map.resize(mainApp.getPrimaryStage().getWidth(), 92);
//                sensor_map.setWidth(mainApp.getPrimaryStage().getWidth());
                System.out.println("Canvas Width: " + sensor_map.getWidth());
                sensor_map.getGraphicsContext2D().clearRect(0, 0, sensor_map.getWidth(), sensor_map.getHeight());
                drawShapes(sensor_map.getGraphicsContext2D());
                
                mainApp.getPrimaryStage().show();
                
            }
        });
        this.mainApp.getPrimaryStage().getScene().heightProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight, Number newSceneHeight) {
                System.out.println("Height: " + newSceneHeight);
            }
        });
		
		GraphicsContext gc = sensor_map.getGraphicsContext2D();
		gc.clearRect(0, 0, sensor_map.getWidth(), sensor_map.getHeight());	//use this to clear the canvas
		
		//drawShapes(gc);
		
		this.mainApp.getPrimaryStage().show();
	}
	
	//between 35 and 80 (distance) / use all of it 0 - 147 roygbiv
	//draw the ir sensor, then draw a box around the object based upon later data provided by robot
	private void drawShapes(GraphicsContext gc){
//		gc.setStroke(Color.BLUE);
//		gc.setLineWidth(1);
//		gc.strokeLine(20, 0, 20, sensor_map.getHeight());
		int line_width = (int)sensor_map.getWidth()/181;
		for(int i = 0; i < 180; i++)
		{
			gc.setStroke(colors[ir_reading[i]]);
			gc.setLineWidth(line_width);
			gc.strokeLine(i*line_width+line_width, 0, i*line_width+line_width, sensor_map.getHeight());
		}
	}
	
	//sets the sensor info and then draws it. eventually will interpret distances as well
	public void set_sensor(int[] ir_sensor)
	{
		ir_reading = ir_sensor;
		for(int i = 179; i > -1; i--)
		{
			ir_reading[i] = get_color(ir_sensor[i]);
		}
		GraphicsContext gc = sensor_map.getGraphicsContext2D();
		gc.clearRect(0, 0, sensor_map.getWidth(), sensor_map.getHeight());	//use this to clear the canvas
		
		drawShapes(gc);
	}
	
	// For the birds eye view map
	private void draw_map(GraphicsContext gc){
		
		//draw the border
		gc.setStroke(Color.BLACK);
		gc.setLineWidth(1.0);
		gc.strokeLine(0, 0, 0, area_map.getHeight());
		gc.strokeLine(0, 0, area_map.getWidth(), 0);
		gc.strokeLine(0, area_map.getHeight(), area_map.getWidth(), area_map.getHeight());
		gc.strokeLine(area_map.getWidth(), 0, area_map.getWidth(), area_map.getHeight());
		
		gc.setFill(Color.GREEN);
		gc.setStroke(Color.GREEN);
		gc.setLineWidth(5.0);
		gc.fillOval((area_map.getWidth()/2), (area_map.getHeight()/2), 10, 10);
		gc.strokeOval((area_map.getWidth()/2), (area_map.getHeight()/2), 10, 10);
		
		gc.setStroke(Color.RED);
		gc.setLineWidth(2.0);
		gc.strokeLine(0, 0, area_map.getWidth(), area_map.getHeight());
	}
	
	private int get_color(int ir)
	{
		int color = -1;
		
		if(ir < 10 && ir != -1){
			color = Colors.DARK_RED.numVal;
		} else if(ir < 15){
			color = Colors.RED.numVal;
		} else if(ir < 20){
			color = Colors.LIGHT_RED.numVal;
		} else if(ir < 25){
			color = Colors.DARK_ORANGE.numVal;
		} else if(ir < 30){
			color = Colors.ORANGE.numVal;
		} else if(ir < 35){
			color = Colors.LIGHT_ORANGE.numVal;
		} else if(ir < 40){
			color = Colors.DARK_YELLOW.numVal;
		} else if(ir < 45){
			color = Colors.YELLOW.numVal;
		} else if(ir < 50){
			color = Colors.LIGHT_YELLOW.numVal;
		} else if(ir < 55){
			color = Colors.LIGHT_GREEN.numVal;
		} else if(ir < 60){
			color = Colors.GREEN.numVal;
		} else if(ir < 65){
			color = Colors.DARK_GREEN.numVal;
		} else if(ir < 70){
			color = Colors.LIGHT_INDIGO.numVal;
		} else if(ir < 75){
			color = Colors.INDIGO.numVal;
		} else if(ir < 80){
			color = Colors.DARK_INDIGO.numVal;
		} else if(ir < 85){
			color = Colors.LIGHT_BLUE.numVal;
		} else if(ir < 90){
			color = Colors.BLUE.numVal;
		} else if(ir < 95){
			color = Colors.DARK_BLUE.numVal;
		} else if(ir < 100){
			color = Colors.LIGHT_VIOLET.numVal;
		} else if(ir < 105){
			color = Colors.VIOLET.numVal;
		} else {
			color = Colors.DARK_VIOLET.numVal;
		}
		
		return color;
	}
	
	@FXML
	public void onEnter(ActionEvent ae){
		String received;
//	   System.out.println("test") ;
	   mainApp.getOutputData().add(new TextOutput(">> " + cmdLineBox.getText()));
	   mainApp.rc.send_string(cmdLineBox.getText() + "\0", mainApp.test);
//	   mainApp.rc.test();
//	   sensor_map.getGraphicsContext2D().clearRect(0, 0, sensor_map.getWidth(), sensor_map.getHeight());
//	   drawShapes(sensor_map.getGraphicsContext2D());
	   this.mainApp.getPrimaryStage().show();
	   cmdLineBox.clear();
	   
	   draw_map(area_map.getGraphicsContext2D());
	   
//	   received = mainApp.rc.get_response(mainApp.test);
//	   mainApp.getOutputData().add(new TextOutput("<< " + received));
//	   SensorData data = new SensorData(received);
//	   set_sensor(data.getData());
	   

	}
	
	
}
