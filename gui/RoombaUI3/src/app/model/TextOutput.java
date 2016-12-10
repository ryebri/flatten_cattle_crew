/**
 * @file TextOutput.java
 * @brief Custom Class for displaying messages in the GUI
 * @author ryebri
 * @date 12/06/2016
 */
package app.model;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

///Class for message boxes
public class TextOutput {
	private StringProperty output;
	
	///Default Constructor
	/**
	 * Creates a new messagebox with no string.
	 */
	public TextOutput() {
		this(null);
	}
	
	///Constructor
	/**
	 * Creates a new messagebox with a string.
	 * @param text
	 */
	public TextOutput(String text) {
		this.output = new SimpleStringProperty(text);
	}
	
	///Gets the string value
	/**
	 * Method to get the string value from a messagebox.
	 * @return StringProperty object containing message string.
	 */
	public StringProperty output() {
		return output;
	}
}
