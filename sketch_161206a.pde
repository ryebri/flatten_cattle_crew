import processing.serial.*;
import java.awt.AWTException;
import java.awt.Robot;

Serial arduinoPort;
int inByte;
KeystrokeSimulator keySim;

void setup(){
  printArray(Serial.list());
  arduinoPort = new Serial(this,Serial.list()[1], 9600);
  keySim = new KeystrokeSimulator(); 
}

void draw(){
  while(arduinoPort.available() > 0){
    try{
      keySim.simulate((char)arduinoPort.read()); 
      delay(500);
    }
    catch(AWTException e){
      println(e);
    }
  }
}


public class KeystrokeSimulator {

private Robot robot;
  
  KeystrokeSimulator(){
    try{
      robot = new Robot();  
    }
    catch(AWTException e){
      println(e);
    }
  }
  
  void simulate(char c) throws AWTException {
    robot.keyPress(c);
  }
}