package app.view;

import java.awt.Point;
import java.util.ArrayList;

import app.Hole;
import app.Line;
import app.MainApp;
import app.Obstruction;
import app.Obstruction.ObstructionType;
import app.Pipe;
import app.Position;
import app.Rock;
import app.SensorData;
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
	
	private SensorData data;						//parses and reads in all data from bot
	private Position position;						//stores position data 
	private ArrayList<Obstruction> obstructions;	//array list containing all known obstructions
	
	
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
		obstructions = new ArrayList<Obstruction>();
		position = new Position();
		this.mainApp.getPrimaryStage().show();
	}
	
	//between 35 and 80 (distance) / use all of it 0 - 147 roygbiv
	//draw the ir sensor, then draw a box around the object based upon later data provided by robot
	private void drawShapes(GraphicsContext gc){
//		gc.setStroke(Color.BLUE);
//		gc.setLineWidth(1);
//		gc.strokeLine(20, 0, 20, sensor_map.getHeight());
		int line_width = (int)sensor_map.getWidth()/181;
		for(int i = 179; i > -1; i--)
		{
			gc.setStroke(colors[ir_reading[i]]);
			gc.setLineWidth(line_width);
			gc.strokeLine((180 - i)*line_width+line_width, 0, (180 - i)*line_width+line_width, sensor_map.getHeight());
		}
	}
	
	//sets the sensor info and then draws it. eventually will interpret distances as well
	public void set_sensor(int[] ir_sensor)
	{
		ir_reading = ir_sensor;
		for(int i = 0; i < 180; i++)
		{
			ir_reading[i] = get_color(ir_sensor[i]);
		}
		GraphicsContext gc = sensor_map.getGraphicsContext2D();
		gc.clearRect(0, 0, sensor_map.getWidth(), sensor_map.getHeight());	//use this to clear the canvas
		
		drawShapes(gc);
	}
	
	// For the birds eye view map
	private void draw_map(GraphicsContext gc){
		gc.clearRect(0, 0, area_map.getWidth(), area_map.getHeight());
		//draw the border
		gc.setStroke(Color.BLACK);
		gc.setLineWidth(1.0);
		gc.strokeLine(0, 0, 0, area_map.getHeight());
		gc.strokeLine(0, 0, area_map.getWidth(), 0);
		gc.strokeLine(0, area_map.getHeight(), area_map.getWidth(), area_map.getHeight());
		gc.strokeLine(area_map.getWidth(), 0, area_map.getWidth(), area_map.getHeight());
		
		//draw the starting position of the robot
		gc.setFill(Color.GREEN);
		gc.setStroke(Color.GREEN);
		gc.setLineWidth(5.0);
		gc.fillOval((position.get_curr_position().x - 17), (position.get_curr_position().y - 17), 34, 34);
		gc.strokeOval((position.get_curr_position().x - 17), (position.get_curr_position().y - 17), 34, 34);
		
		//draws a cross hair on the screen for ease of testing
		gc.setStroke(Color.BLACK);
		gc.setLineWidth(3.0);
		gc.strokeLine(position.get_start_x(), position.get_start_y() + 50, position.get_start_x(), position.get_start_y() - 50);
		gc.strokeLine(position.get_start_x() + 50, position.get_start_y(), position.get_start_x() - 50, position.get_start_y());

		
		//call method to draw obstructions
		
//		gc.setStroke(Color.RED);
//		gc.setLineWidth(2.0);
//		gc.strokeLine(0, 0, area_map.getWidth(), area_map.getHeight());
	}
	
	private void draw_obstructions(GraphicsContext gc){
		//depending upon what type of object it is, it creates a different object
		//pipes are both the same, red circle 
		//line draws a white line (might need to add in some other data about it
		//rock draws a blue circle
		//hole draws a rectangle
		int adj_angle = position.get_orientation();
		int cos_coeff = 1;
		int sin_coeff = 1;
		Double radian;
		
		for(int i = 0; i < obstructions.size(); i++){
			obstructions.get(i).drawShape(gc);
		}
		
		//Draw Orientation
		if (adj_angle >= 360){
			cos_coeff = 1;
			sin_coeff = -1;
			adj_angle = 90 - adj_angle % 360;
			
		//object is north and west, --
		}else if(adj_angle >= 270){
			cos_coeff = -1;
			sin_coeff = -1;
			adj_angle = adj_angle - 270;
			
		} else if(adj_angle <= -1){
			cos_coeff = -1;
			sin_coeff = -1;
			adj_angle = 90 + adj_angle;
			
		//object is south and west, +-
		} else if(adj_angle >= 180){
			cos_coeff = -1;
			sin_coeff = 1;
			adj_angle = 270 - adj_angle;
			
		//object is south and east, ++
		} else if(adj_angle >= 90){
			cos_coeff = 1;
			sin_coeff = 1;
			adj_angle = adj_angle - 90;
			
		//object is north and east, -+
		} else {
			cos_coeff = 1;
			sin_coeff = -1;
			adj_angle = 90 - adj_angle;
		}
		radian = Math.toRadians(adj_angle);
		
		//background for the compass
		gc.setStroke(Color.WHITE);
		gc.setFill(Color.WHITE);
		gc.setLineWidth(1.0);
		gc.strokeOval(650, 425, 100, 100);
		gc.fillOval(650, 425, 100, 100);
		
		//draw compass
		gc.setStroke(Color.RED);
		gc.setLineWidth(3.0);
		gc.strokeLine(700, 475, 50*cos_coeff*Math.cos(radian) + 700, 50*sin_coeff*Math.sin(radian) + 475);
		
		//draw middle of compass
		gc.setStroke(Color.BLACK);
		gc.setFill(Color.GREEN);
		gc.strokeRect(697, 472, 5, 5);
		gc.fillRect(698, 473, 3, 3);
	}
	
	private int get_color(int ir)
	{
		int color = -1;
		if(ir == -1){
			color = Colors.DARK_VIOLET.numVal;
		}else if(ir < 10){
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
	   receive();
	   
//	   draw_map(area_map.getGraphicsContext2D());

	   //ALL MOVED TO METHOD receive()
//	   /*
//	    * Need to receive 4 sets of data
//	    */
//	   //receive ir
//	   received = mainApp.rc.get_response(mainApp.test);
//	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: ir info"));
//	   data = new SensorData(received);
//	   set_sensor(data.getData());
//	   
//	   //receive objects
//	   received = mainApp.rc.get_response(mainApp.test);
//	   data.add_data(received);
//	   Obstruction[] temp = data.getObstruction();
//	   mainApp.getOutputData().add(new TextOutput("<< " + received));
//	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: object info"));
//	   
//	   /* 	receive position and direction 
//	    *  	use this method to update the positions
//	    */
//	   //position.update_positions(north, south, east, west, orientation);
//	   received = mainApp.rc.get_response(mainApp.test);
//	   data.add_data(received);
//	   position.update_positions(data.get_position());
//	   mainApp.getOutputData().add(new TextOutput("<< " + received));
//	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: position info"));
//	   
//	   //receive other sensor data
//	   received = mainApp.rc.get_response(mainApp.test);
//	   data.add_data(received);
//	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: bot sensors"));
//	   //interpret obstructions based upon sensor data
//	   //testing
//	   if(mainApp.test == 0x01){
//		   
//		   position.set_orientation(0);
//		   Pipe ob1 = new Pipe(40, 65, 6);
//		   ob1.set_point(calculate_location(ob1));
//		   ob1.drawShape(area_map.getGraphicsContext2D());
//		   
//		   Pipe ob2 = new Pipe(43, 120, 2);
//		   ob2.set_point(calculate_location(ob2));
//		   ob2.drawShape(area_map.getGraphicsContext2D());
//	   }
//	  
//	   
//	   
//
//	   interpret_obstructions(temp);
//	   draw_map(area_map.getGraphicsContext2D());
//	   draw_obstructions(area_map.getGraphicsContext2D());
	}
	/*
	 *used for the controller 
	 */
	public void receive(){
		String received;
	   this.mainApp.getPrimaryStage().show();
	   cmdLineBox.clear();
	   
//		   draw_map(area_map.getGraphicsContext2D());
	   
	   /*
	    * Need to receive 4 sets of data
	    */
	   //receive ir
	   received = mainApp.rc.get_response(mainApp.test);
	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: ir info"));
	   data = new SensorData(received);
	   set_sensor(data.getData());
	   
	   //receive objects
	   received = mainApp.rc.get_response(mainApp.test);
	   data.add_data(received);
	   Obstruction[] temp = data.getObstruction();
	   mainApp.getOutputData().add(new TextOutput("<< " + received));
	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: object info"));
	   
	   /* 	receive position and direction 
	    *  	use this method to update the positions
	    */
	   //position.update_positions(north, south, east, west, orientation);
	   received = mainApp.rc.get_response(mainApp.test);
	   data.add_data(received);
	   position.update_positions(data.get_position());
	   mainApp.getOutputData().add(new TextOutput("<< " + received));
	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: position info"));
	   
	   //receive other sensor data
	   received = mainApp.rc.get_response(mainApp.test);
	   data.add_data(received);
	   mainApp.getOutputData().add(new TextOutput("<< " + "Received: bot sensors"));
	   //interpret obstructions based upon sensor data
	   //testing
	   if(mainApp.test == 0x01){
		   
		   position.set_orientation(90);
		   Pipe ob1 = new Pipe(40, 65, 6);
		   ob1.set_point(calculate_location(ob1));
		   ob1.drawShape(area_map.getGraphicsContext2D());
		   
		   Pipe ob2 = new Pipe(43, 120, 2);
		   ob2.set_point(calculate_location(ob2));
		   ob2.drawShape(area_map.getGraphicsContext2D());
	   }
	  
	   
	   

	   interpret_obstructions(temp);
	   draw_map(area_map.getGraphicsContext2D());
	   draw_obstructions(area_map.getGraphicsContext2D());
		
	}
	
	//based upon other sensor data, inputs data to the obstruction array
	// will have another argument eventually containing sensor data
	private void interpret_obstructions(Obstruction[] obstr){
		int already_exists = 0;
		
		//check to see if the pipe is already in the obstructions, add it if it isn't
		for(int i = 0; i < data.get_obstr_size(); i++){
			already_exists = 0;
			Pipe temp = new Pipe(obstr[i].get_distance(), obstr[i].get_angle(), obstr[i].get_width());
			Point location = calculate_location(temp);
			temp.set_point(location.x, location.y);
			//do a check to see if object exists in/near the same location
			for(int j = 0; j < obstructions.size(); j++){
				if(obstructions.get(j).get_type().getValue() == ObstructionType.LARGE_PIPE.getValue()){
					Pipe pipe = ((Pipe)obstructions.get(j));
					
					//may need to adjust these values, not sure if +- 3 is big enough for margin of error
					if(pipe.get_point().x+3 > location.x && pipe.get_point().x-3 < location.x && pipe.get_point().y+3 > location.y && pipe.get_point().y-3 < location.y){
						already_exists = 1;
						if(pipe.get_distance() < temp.get_distance()){
							pipe.set_distance(temp.get_distance());
						}
						break;
					}
				}
			}
			if(already_exists != 1){
				temp.set_point(calculate_location(temp));
				obstructions.add(temp);
			}
		}
		
		//add way to interpret rocks/lines/holes
		if(mainApp.test == 0x01){
			data.add_data("{\"sensors\": [001,001,001,002,003]}\n\0");
		}
		interpret_bot_sensors();
		
	}
	
	/*
	 * Calculates the location for where an object should be drawn on the canvas
	 */
	private Point calculate_location(Obstruction obstr){
		Point p = new Point();
		int sin_coeff = 1;
		int cos_coeff = 1;
		int adj_angle = 0;
		Double radian;
		
		//converts the angle from the ir sensor to the orientation angle system
		if(obstr.get_angle() <= 90){
			adj_angle = (position.get_orientation() + (90 - obstr.get_angle()));
		} else {
			adj_angle = (position.get_orientation() - (obstr.get_angle() - 90));
		}
		//object is north and east, -+
		if (adj_angle >= 360){
			cos_coeff = 1;
			sin_coeff = -1;
			adj_angle = 90 - adj_angle % 360;
			
		//object is north and west, --
		}else if(adj_angle >= 270){
			cos_coeff = -1;
			sin_coeff = -1;
			adj_angle = adj_angle - 270;
			
		} else if(adj_angle <= -1){
			cos_coeff = -1;
			sin_coeff = -1;
			adj_angle = 90 + adj_angle;
			
		//object is south and west, +-
		} else if(adj_angle >= 180){
			cos_coeff = -1;
			sin_coeff = 1;
			adj_angle = 270 - adj_angle;
			
		//object is south and east, ++
		} else if(adj_angle >= 90){
			cos_coeff = 1;
			sin_coeff = 1;
			adj_angle = adj_angle - 90;
			
		//object is north and east, -+
		} else {
			cos_coeff = 1;
			sin_coeff = -1;
			adj_angle = 90 - adj_angle;
		}
		radian = Math.toRadians(adj_angle);
		
		//use distance measure and angle measure in a trig function to decompose into x and y
		p.setLocation((obstr.get_distance()*cos_coeff*Math.cos(radian)), (obstr.get_distance()*sin_coeff*Math.sin(radian)));
		
		//give it the proper point in the canvas
		p.setLocation((p.x + position.get_curr_position().x), (p.y + position.get_curr_position().y));
		
		return p;
	}
	
	private void interpret_bot_sensors(){
		int sensors[] = data.getSensors();
		int adj_angle = position.get_orientation();
		int cos_coeff = 1, sin_coeff = 1;
		Double radian;
		
		//interpret rocks
		Rock temp = new Rock();
		switch(sensors[4]){
		case 0:
			//make rock @ 45 degree angle to the left
			adj_angle -= 45;
			break;
			
		case 1:
			//make rock @ 45 degree angle to the right
			adj_angle += 45;
			break;
			
		default:
			break;
				
		}
		if(sensors[4] != 3){
			if (adj_angle >= 360){
				cos_coeff = 1;
				sin_coeff = -1;
				adj_angle = 90 - adj_angle % 360;
				
			//object is north and west, --
			}else if(adj_angle >= 270){
				cos_coeff = -1;
				sin_coeff = -1;
				adj_angle = adj_angle - 270;
				
			} else if(adj_angle <= -1){
				cos_coeff = -1;
				sin_coeff = -1;
				adj_angle = 90 + adj_angle;
				
			//object is south and west, +-
			} else if(adj_angle >= 180){
				cos_coeff = -1;
				sin_coeff = 1;
				adj_angle = 270 - adj_angle;
				
			//object is south and east, ++
			} else if(adj_angle >= 90){
				cos_coeff = 1;
				sin_coeff = 1;
				adj_angle = adj_angle - 90;
				
			//object is north and east, -+
			} else {
				cos_coeff = 1;
				sin_coeff = -1;
				adj_angle = 90 - adj_angle;
			}
			radian = Math.toRadians(adj_angle);
			
			temp.set_point((int)(17*cos_coeff*Math.cos(radian) + position.get_curr_position().x), (int)(17*sin_coeff*Math.sin(radian) + position.get_curr_position().y));
		}
		
		if(temp.get_point().x != 0 && temp.get_point().y != 0){
			obstructions.add(temp);
		}
		
		//interpret line sensors
		
		//just interpret the values, if any have something add either a line or a hole, or the final thing
		/*
		 * 0 - white tape/boundary, 1 - normal floor, 2 - extraction, 3 - hole, 4 - error
		 */
		int flag = 1;
		for(int i = 0; i < 4; i++){
			if(sensors[i] != 1){
				flag = sensors[i];
				break;
			}
		}
		adj_angle = position.get_orientation();
		if(sensors[0] == flag || sensors[3] == flag){
			if(sensors[0] == flag){
				adj_angle -= 45;
			} else {
				adj_angle +=45;
			}
			
			if (adj_angle >= 360){
				cos_coeff = 1;
				sin_coeff = -1;
				adj_angle = 90 - adj_angle % 360;
				
			//object is north and west, --
			}else if(adj_angle >= 270){
				cos_coeff = -1;
				sin_coeff = -1;
				adj_angle = adj_angle - 270;
				
			} else if(adj_angle <= -1){
				cos_coeff = -1;
				sin_coeff = -1;
				adj_angle = 90 + adj_angle;
				
			//object is south and west, +-
			} else if(adj_angle >= 180){
				cos_coeff = -1;
				sin_coeff = 1;
				adj_angle = 270 - adj_angle;
				
			//object is south and east, ++
			} else if(adj_angle >= 90){
				cos_coeff = 1;
				sin_coeff = 1;
				adj_angle = adj_angle - 90;
				
			//object is north and east, -+
			} else {
				cos_coeff = 1;
				sin_coeff = -1;
				adj_angle = 90 - adj_angle;
			}
		}
		radian = Math.toRadians(adj_angle);
		
		int delta_x = position.get_start_x() - position.get_curr_position().x;
		int delta_y = position.get_start_y() - position.get_curr_position().y;
		adj_angle = position.get_orientation();
		switch(flag){
		case 0:
			Line l = new Line();
			//West
			if(sensors[0] == 0 && (adj_angle < 5 || adj_angle > 355)){
				l.set_p1(position.get_curr_position().x - 17, 0);
				l.set_p2(position.get_curr_position().x - 17, (int)area_map.getHeight());
				//add the other side as well?
				
			//East
			} else if(sensors[0] == 0 && (adj_angle > 175 && adj_angle < 185)){
				l.set_p1(position.get_curr_position().x + 17, 0);
				l.set_p2(position.get_curr_position().x + 17, (int)area_map.getHeight());
			
			//North
			} else if(sensors[0] == 0 && (adj_angle > 85 && adj_angle < 95 )){
				l.set_p1(0, position.get_curr_position().y - 17);
				l.set_p1((int)area_map.getWidth(), position.get_curr_position().y - 17);
				
			//South
			} else if(sensors[0] == 0 && (adj_angle > 265 && adj_angle < 275)){
				l.set_p1(0, position.get_curr_position().y + 17);
				l.set_p2((int)area_map.getWidth(), position.get_curr_position().y + 17);
			
			//East
			} else if(sensors[3] == 0 && (adj_angle < 5 || adj_angle > 355)){
				l.set_p1(position.get_curr_position().x + 17, 0);
				l.set_p2(position.get_curr_position().x + 17, (int)area_map.getHeight());
			
			//West
			} else if(sensors[3] == 0 && (adj_angle > 175 && adj_angle > 185)){
				l.set_p1(position.get_curr_position().x - 17, 0);
				l.set_p2(position.get_curr_position().x - 17, (int)area_map.getHeight());
			
			//South
			} else if(sensors[3] == 0 && (adj_angle > 85 && adj_angle < 95)){
				l.set_p1(0, position.get_curr_position().y + 17);
				l.set_p2((int)area_map.getWidth(), position.get_curr_position().y + 17);
				
			//North
			} else if(sensors[3] == 0 && (adj_angle > 265 && adj_angle < 275)){
				l.set_p1(0, position.get_curr_position().y - 17);
				l.set_p1((int)area_map.getWidth(), position.get_curr_position().y - 17);
			
			//North
			} else if(adj_angle > 355 || adj_angle < 5){
				l.set_p1(0, position.get_curr_position().y - 17);
				l.set_p1((int)area_map.getWidth(), position.get_curr_position().y - 17);
				
			//South
			} else if(adj_angle > 175 && adj_angle < 185){
				l.set_p1(0, position.get_curr_position().y + 17);
				l.set_p2((int)area_map.getWidth(), position.get_curr_position().y + 17);
				
			//East
			} else if(adj_angle > 75 && adj_angle < 85){
				l.set_p1(position.get_curr_position().x + 17, 0);
				l.set_p2(position.get_curr_position().x + 17, (int)area_map.getHeight());
				
			//West
			} else if(adj_angle > 265 && adj_angle < 285){
				l.set_p1(position.get_curr_position().x - 17, 0);
				l.set_p2(position.get_curr_position().x - 17, (int)area_map.getHeight());
			}

			obstructions.add(l);
			
			break;
		case 3:
			mainApp.getOutputData().add(new TextOutput("<< " + "EXTRACTION POINT"));
			break;
		case 2:
			Hole h = new Hole();
			//calculate where to place the hole
			if(adj_angle < 45 || adj_angle > 325){
				//add @ location of bot y-60
				h.set_point(position.get_curr_position().x - 30, (int)position.get_curr_position().y - 60);
			} else if(adj_angle > 45 && adj_angle < 135){
				//add @ location y + 30
				h.set_point(position.get_curr_position().x, position.get_curr_position().y - 30);
			} else if(adj_angle > 135 && adj_angle < 225){
				//add @ location
				h.set_point(position.get_curr_position().x - 30, position.get_curr_position().y);
			} else if(adj_angle > 225 && adj_angle < 325){
				//add @ location x - 60, y - 30
				h.set_point(position.get_curr_position().x - 60, position.get_curr_position().y - 30);
			}
			obstructions.add(h);
			break;
		case 4:
			mainApp.getOutputData().add(new TextOutput("<< " + "FloorSensor: Error"));
			break;
		default:
			break;
		} 
	}
	
	
}
