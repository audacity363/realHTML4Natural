package realHTML.tomcat.JSONMatcher;

import java.util.LinkedList;
import com.eclipsesource.json.*;

public class JSONParser {
	
	String json;
	
	LLHandler varlist;
	LinkedList<String> parents;
	
	JSONUtils utils;
	
	public JSONParser(String json) {
		this.json = json;
		
		this.varlist = new LLHandler();
		this.utils = new JSONUtils();
	}
	
	public LLHandler run() throws JSONArrayException, TypeException, JSONObjectException {
		String name;
		JsonValue jsonObject = Json.parse(this.json);
        
        for (JsonObject.Member next : jsonObject.asObject()) {
            JsonValue value = next.getValue();
            name = next.getName();
	        parseValue(value, name);
        }
        return(this.varlist);
	}
	
	private void parseValue(JsonValue value, String name) throws JSONArrayException, TypeException, JSONObjectException {
		Types vartype;
		JSONArray arrayhandler;
		
		if(!value.isArray() && !value.isObject()) {
			vartype = this.utils.getJSONType(value);
			switch(vartype) {
				case STRING:
					if (this.parents != null) {
						this.varlist.addVar((String[])this.parents.toArray(new String[this.parents.size()]), 
								name, value.asString());
					} else {
						this.varlist.addVar(name, value.asString());
					}
					break;
				case FLOAT:
					if (this.parents != null) {
						this.varlist.addVar((String[])this.parents.toArray(new String[this.parents.size()]), 
								name, value.asFloat());
					} else {
						this.varlist.addVar(name, value.asFloat());
					}
					break;
				case INT:
					if (this.parents != null) {
						this.varlist.addVar((String[])this.parents.toArray(new String[this.parents.size()]), 
								name, value.asInt());
					} else {
						this.varlist.addVar(name, value.asInt());
					}
					break;
				case BOOLEAN:
					if (this.parents != null) {
						this.varlist.addVar((String[])this.parents.toArray(new String[this.parents.size()]), 
								name, value.asBoolean());
					} else {
						this.varlist.addVar(name, value.asBoolean());
					}
					break;
				case UNKNOWN:
					return;
				default:
					System.out.printf("Unkown vartype: [%s]\n", vartype.toString());
					return;
			}
			//System.out.printf("Value: %s\n",  value.toString());
		} else if(value.isArray()) {
			if(this.parents == null) {
				arrayhandler = new JSONArray(value.asArray(), name, this.varlist);
			} else {
				arrayhandler = new JSONArray(value.asArray(), name, this.parents, this.varlist);
			}
			arrayhandler.handleArray();
			this.varlist = arrayhandler.getVarlist();
		} else if(value.isObject()) {
			//System.out.println("Found Object");
			if (this.parents == null) {
				this.parents = new LinkedList<String>();
				this.varlist.addGrp(name);
				//System.out.println("Added Group "+ name);
			} else {
				this.varlist.addGrp((String[])this.parents.toArray(new String[this.parents.size()]), name);
			}
			this.parents.add(name);
			
			
			for(JsonObject.Member entry: value.asObject()) {
				name = entry.getName();
				parseValue(entry.getValue(), name);
			}
			this.parents.removeLast();
			if(this.parents.size() == 0) {
				this.parents = null;
			}
		}
	}
}
