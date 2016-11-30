package app;

public class IRdata {
	
	private String name;
	private int data[];
	private Obstruction obstr[];
	
	public IRdata(String json){
		str_to_obj(json);
	}
	
	public String getName(){
		return name;
	}
	
	public int[] getData()
	{
		return data;
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
		} else if(data != null && name.compareTo("sonar") == 0){
			obstr = new Obstruction[15];
			//{"sonar": {[18, 3.2, 20], [28, 6.4, 30]}}
		} else {
			data = null;
		}
	}
	
	public void add_sonar_data(String json){
		
	}
}
