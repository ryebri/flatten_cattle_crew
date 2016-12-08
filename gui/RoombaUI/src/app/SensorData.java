package app;

///Class to interpret json data
/**
 * Interprets the json data sent from the roomba and creates proper objects based upon which object
 * type is contained within the json data.
 * @author ryebri
 *
 */
public class SensorData {
	
//	public enum Sensor {
//		BUMP_LEFT(0), BUMP_RIGHT(1), CLIFF_SENSOR(2);
//		
//		private final int index;
//		Sensor(int index) {this.index = index;}
//		public int getValue() { return index; }
//	}
	
	
	private String name;
	private int data[];
	private Obstruction obstr[];
	private int obstr_size;
	private int position[];	//[x,y,orientation] --new [North, South, East, West, orientation] --old
	private int sensors[];	//[cliff_left, cliff_leftfront, cliff_rightfront, cliff_right, bumper]
	
	public int flag_data_done;
	
	
	/**
	 * Constructor for the SensorData class
	 * @param json
	 */
	public SensorData(String json){
		flag_data_done = 0;
		obstr_size = 0;
		position = new int[6];
		sensors = new int[5];
		str_to_obj(json);
	}
	
	public String getName(){
		return name;
	}
	
	public int[] getData()
	{
		return data;
	}
	
	public Obstruction[] getObstruction(){
		return obstr;
	}
	
	public int[] getSensors(){
		return sensors;
	}
	
	public int get_obstr_size(){
		return obstr_size;
	}
	
	public int[] get_position(){
		return position;
	}
	
	
	private void str_to_obj(String json){
		flag_data_done = 0;
		int i = 0, index = -1, temp = 0;
		StringBuilder builder = new StringBuilder();
		while(json.charAt(i) != '\"')
		{
			i++;
		}
		i++;
		while(json.charAt(i) != '\"'){
			builder.append(json.charAt(i));
			i++;
		}
		
		name = builder.toString();
		if(name.compareTo("ir") == 0){
			data = new int[180];
			while(json.charAt(i)!= '['){
				i++;
			}
			i++;
			for(int j = 0; j < 180; j++){
				if(json.charAt(i)==']'){
					break;
				} else if(json.charAt(i) == ','){
					i++;
				}
				temp = Character.getNumericValue(json.charAt(i))*100 + Character.getNumericValue(json.charAt(i+1))*10 + Character.getNumericValue(json.charAt(i+2));
				data[j] = temp;
				i+=3;
				
			}
		} else if(name.compareTo("objects") == 0){
			int obstr_data_index = 0;
			obstr = new Obstruction[15];
			while(json.charAt(i)!= '['){
				i++;
			}

			while(json.charAt(i)!= '}'){
				if(json.charAt(i) == '[') {
					index+=1;
					obstr_data_index = 0;
					obstr[index] = new Obstruction();
					
				} else if(json.charAt(i) == ']' && json.charAt(i+1) != '}'){
					i+=2;
				} else if(json.charAt(i) == ','){
					i++;
				} else {
					if(obstr_data_index == 0){
						obstr[index].set_distance(Character.getNumericValue(json.charAt(i))*100 + Character.getNumericValue(json.charAt(i+1))*10 + Character.getNumericValue(json.charAt(i+2)));
						i+=3;
						obstr_data_index++;
					} else if(obstr_data_index == 1){
						obstr[index].set_angle(Character.getNumericValue(json.charAt(i))*100 + Character.getNumericValue(json.charAt(i+1))*10 + Character.getNumericValue(json.charAt(i+2)));
						i+=3;
						obstr_data_index++;
					} else if(obstr_data_index == 2){
						obstr[index].set_width(Character.getNumericValue(json.charAt(i))*100 + Character.getNumericValue(json.charAt(i+1))*10 + Character.getNumericValue(json.charAt(i+2)));
						i+=3;
						obstr_data_index++;
					}	
				}			
				i++;
			}
			
			obstr_size = index + 1;
			//{"object": {[distance, angle, width], [...]}}
			//{"object": {[018, 032, 020], [028, 006, 030]}}
		} else if(name.compareTo("position") == 0){
			//{"position": {[NORTH,SOUTH,EAST,WEST,orientation]}
			//new version {"position": [x,y,orientation]}
			position = new int[6];
			while(json.charAt(i)!= '['){
				i++;
			}
			i++;
			for(int j = 0; j < 6; j++){
				if(json.charAt(i)==']' || json.charAt(i) == '}'){
					break;
				} else if(json.charAt(i) == ','){
					i++;
				}
				temp = Character.getNumericValue(json.charAt(i))*100 + Character.getNumericValue(json.charAt(i+1))*10 + Character.getNumericValue(json.charAt(i+2));
				position[j] = temp;
				i+=3;
			}

			
		} else if(name.compareTo("sensors") == 0){
			//{"sensors": [cliff_left, cliff_leftfront, cliff_rightfront, cliff_right, bumper]}
			
			sensors = new int[5];
			while(json.charAt(i)!= '['){
				i++;
			}
			i++;
			for(int j = 0; j < 5; j++){
				if(json.charAt(i)==']' || json.charAt(i) == '}'){
					break;
				} else if(json.charAt(i) == ','){
					i++;
				}
				temp = Character.getNumericValue(json.charAt(i))*100 + Character.getNumericValue(json.charAt(i+1))*10 + Character.getNumericValue(json.charAt(i+2));
				sensors[j] = temp;
				i+=3;
			}
			flag_data_done = 1;
		} else {
			data = null;
		}
	}
	
	//method to add data to the object
	public void add_data(String json){
		str_to_obj(json);
	}
	
	//calculates the position of the object based upon position of bot and object info
	//1cm is 1 x or y unit
	private void calc_location(int index){
		
	}
}
