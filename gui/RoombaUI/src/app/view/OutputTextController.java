package app.view;

import app.MainApp;
import app.model.TextOutput;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;

public class OutputTextController {
	@FXML
	private TableView<TextOutput> outputList;
	
	@FXML
	private TableColumn<TextOutput, String> firstColumn;
	
	@FXML
	private TextField cmdLineBox;
	
	
	
	private MainApp mainApp;
	
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
	}
	
	@FXML
	public void onEnter(ActionEvent ae){
//	   System.out.println("test") ;
	   mainApp.getOutputData().add(new TextOutput(">> " + cmdLineBox.getText()));
	   mainApp.rc.send_string(cmdLineBox.getText() + "\0");
	   cmdLineBox.clear();
	}
}
