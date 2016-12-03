package app;

public class SensorData {
	
	public enum Direction {
	    NORTH(0), SOUTH(1), EAST(2), WEST(3);

	    private final int id;
	    Direction(int id) { this.id = id; }
	    public int getValue() { return id; }
	}
	
	public enum Sensor {
		BUMP_LEFT(0), BUMP_RIGHT(1), CLIFF_SENSOR(2);
		
		private final int index;
		Sensor(int index) {this.index = index;}
		public int getValue() { return index; }
	}
	
	private String name;
	private int data[];
	private Obstruction obstr[];
	private int sensors[];
	
	public int flag_data_done;
	
	
	/**
	 * Constructor for the SensorData class
	 * @param json
	 */
	public SensorData(String json){
		flag_data_done = 0;
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
	
	
	private void str_to_obj(String json){
		int i = 0, temp = 0;
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
		} else if(name.compareTo("obstr") == 0){
			obstr = new Obstruction[15];
			//{"obstr": {[18, 3.2, 20], [28, 6.4, 30]}}
		} else if(name.compareTo("position") == 0){
			//{"position": {[NORTH, SOUTH, EAST, WEST]}
		} else if(name.compareTo("sensors") == 0){
			//{"sensors": {[BUMP_LEFT, BUMP_RIGHT, CLIFF_SENSOR]}
			flag_data_done = 1;
		} else {
			data = null;
		}
	}
	
	public void add_data(String json){
		str_to_obj(json);
	}
}
