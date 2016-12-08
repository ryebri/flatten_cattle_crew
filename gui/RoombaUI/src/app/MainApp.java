package app;

import java.io.IOException;

import app.model.TextOutput;
import app.view.OutputTextController;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

///Main Class which starts everything
/**
 * Beginning of the JavaFX GUI.  Contains a variable called test which when assigned 0x00 creates
 * the roomba communication object, otherwise it enables various test methods throughout the code.
 * @author ryebri
 *
 */
public class MainApp extends Application {

	private Stage primaryStage;
    private BorderPane rootLayout;
    private ObservableList<TextOutput> outputData = FXCollections.observableArrayList();
    public RoombaComm rc;
    private OutputTextController controller;
//    public ResizableCanvas sensor_map;
	public final char test = (char)0x01;	//change to 0x00 when in actual use, 0x01 for testing
    
	///Constructor for the MainApp
	/**
	 * Constructor for the MainApp Object
	 */
    public MainApp(){
    	/*
    	Used for testing purposes
    	 */
//    	outputData.add(new TextOutput("Testing"));
//    	outputData.add(new TextOutput("Test 2"));
    }
    
    ///Starts the program
    /**
     * Overriden method to start the JavaFX GUI
     * @param primaryStage A Stage object is required to start the application
     */
	@Override
	public void start(Stage primaryStage) {
		this.primaryStage = primaryStage;
		this.primaryStage.setTitle("Cows Go Moo");
		
		initRootLayout();
		
		showMainPage();
	}
	
	///Returns the Displayed Text responses
	/**
	 * Returns the data as an observable list of output data so more messages can be added
	 * @return ObservableList<TextOutput> List of the messages in the GUI
	 */
	public ObservableList<TextOutput> getOutputData() {
		return outputData;
	}
	
	///Main Function
	/**
	 * Launches the physical gui.
	 * @param args String array of arguments for the program
	 */
	public static void main(String[] args) {
		
		launch(args);
		
	}
	
	///Initializes the root layout
    /**
     * Initializes the root layout by setting all of the GUI objects.  Initializes the object
     * for communication between the GUI and roomba.
     */
    public void initRootLayout() {
        try {
            // Load root layout from fxml file.
//            primaryStage.setMaximized(true);
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("view/RootLayout.fxml"));
            rootLayout = (BorderPane) loader.load();
//            sensor_map = new ResizableCanvas();
            // Show the scene containing the root layout.
            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);
            primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>(){
            	public void handle(WindowEvent we) {
            		try {
            			if(test == 0x00)
            			{
            				rc.close();
            			}
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
            	}
            });
            primaryStage.show();

            
    		try
    		{
    			rc = new RoombaComm();
    			if(test == 0x00)
    			{	
					rc.connect("192.168.1.1", 42880);
//					rc.send_string("start\0", test);
    			
//					rc.close();		//needs to be called eventually/on close of program
    			}
    		} catch (Exception e) {
    			e.printStackTrace();
    		}
    		
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    ///Loads the main page into the program
    /**
     * Shows the main overview inside the root layout.
     */
    public void showMainPage() {
        try {
            // Load person overview.
        	primaryStage.setMaximized(true);
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("view/MainPage.fxml"));
            AnchorPane mainPage = (AnchorPane) loader.load();

            // Set person overview into the center of root layout.
            rootLayout.setCenter(mainPage);
            rootLayout.setTop(new Canvas());
            
            controller = loader.getController();
//            rc.test();
            controller.setMainApp(this);
            rc.set_controller(controller);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    ///Returns the main stage.
    /**
     * Returns the main stage. This is used to alter what is viewed on the screen.
     * @return Stage Object
     */
    public Stage getPrimaryStage() {
        return primaryStage;
    }
}
