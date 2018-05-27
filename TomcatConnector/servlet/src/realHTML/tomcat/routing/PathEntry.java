package realHTML.tomcat.routing;

import java.util.Arrays;

public class PathEntry {
	PathType type;
	String name;
	String value;
	
	//is only set when type == SELECTION
	String[] options;
	
	public PathEntry(PathType type, String name) {
		this.type = type;
		this.name = name;
		this.options = null;
	}
	
	public PathEntry(PathType type, String name, String[] options) {
		this.type = type;
		this.name = name;
		this.options = options;
	}
	
	public Boolean isVariable() {
		if (this.type == PathType.VARIABLE) {
			return(true);
		}
		return(false);
	}
	
	public String getName() {
		return(this.name);
	}
	
	public String toString() {
		String representation = "PathEntry[Type=";
		switch(this.type) {
			case VARIABLE:
				representation += "Variable"; 
				break;
			case STATIC:
				representation += "Static";
				break;
			case SELECTION:
				representation += "Options(";
				for(int i = 0; i < this.options.length; i++) {
					representation += this.options[i];
					if(i+1 < this.options.length) { representation += "|"; } 
				}
				representation += ")";
				break;
		}
		return(representation += ", Name=" + this.name + "]");
	}
	
	public void setValue(String value) {
		this.value = value;
	}
	
	public String getValue() {
		return(this.value);
	}
	
	public PathType getType() {
		return(this.type);
	}
	
	public Boolean containsOption(String option) {
		return(Arrays.asList(this.options).contains(option));
	}
}
