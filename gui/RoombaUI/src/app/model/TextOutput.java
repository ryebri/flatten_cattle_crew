package app.model;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class TextOutput {
	private StringProperty output;
	
	public TextOutput() {
		this(null);
	}
	
	public TextOutput(String text) {
		this.output = new SimpleStringProperty(text);
	}
	
	public StringProperty output() {
		return output;
	}
}
