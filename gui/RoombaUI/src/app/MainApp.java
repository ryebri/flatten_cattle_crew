package app;

import java.io.IOException;

import app.model.TextOutput;
import app.view.OutputTextController;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

/**
 * Beginning of the JavaFX GUI
 * @author ryebri
 *
 */
public class MainApp extends Application {

	private Stage primaryStage;
    private BorderPane rootLayout;
    private ObservableList<TextOutput> outputData = FXCollections.observableArrayList();
    public RoombaComm rc;
    
    public MainApp(){
    	/*
    	Used for testing purposes
    	 */
//    	outputData.add(new TextOutput("Testing"));
//    	outputData.add(new TextOutput("Test 2"));
    }
	@Override
	public void start(Stage primaryStage) {
		this.primaryStage = primaryStage;
		this.primaryStage.setTitle("Cows Go Moo");
		
		initRootLayout();
		
		showMainPage();
	}
	
	/**
	 * Returns the data as an observable list of output data
	 * @return
	 */
	public ObservableList<TextOutput> getOutputData() {
		return outputData;
	}
	
	/**
	 * Launches the physical gui
	 * @param args
	 */
	public static void main(String[] args) {
		launch(args);
		
	}
	

    /**
     * Initializes the root layout.
     */
    public void initRootLayout() {
        try {
            // Load root layout from fxml file.
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("view/RootLayout.fxml"));
            rootLayout = (BorderPane) loader.load();

            // Show the scene containing the root layout.
            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);
            primaryStage.show();
            
    		try
    		{
    			rc = new RoombaComm();
    			
    			rc.connect("192.168.1.1", 42880);
    			rc.send_string("start\0");
    			
//    			rc.close();		//needs to be called eventually/on close of program
    		} catch (Exception e) {
    			e.printStackTrace();
    		}
    		
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    

    /**
     * Shows the main overview inside the root layout.
     */
    public void showMainPage() {
        try {
            // Load person overview.
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(MainApp.class.getResource("view/MainPage.fxml"));
            AnchorPane mainPage = (AnchorPane) loader.load();

            // Set person overview into the center of root layout.
            rootLayout.setCenter(mainPage);
            
            OutputTextController controller = loader.getController();
            controller.setMainApp(this);
            
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Returns the main stage.
     * @return
     */
    public Stage getPrimaryStage() {
        return primaryStage;
    }
}
