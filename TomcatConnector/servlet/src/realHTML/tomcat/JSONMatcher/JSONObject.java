package realHTML.tomcat.JSONMatcher;

import com.eclipsesource.json.*;

public class JSONObject {
	
	JsonObject target;
	JSONUtils utils;
	LLHandler signature;
	
	public JSONObject(JsonObject object) {
		this.target = object;
		this.utils = new JSONUtils();
	}
	
	public void createObjectSignature() throws JSONArrayException, JSONObjectException {
		this.signature = new LLHandler();
		Types type;
		String name;
		JSONArray arrayhandler;
		JsonValue value;
		Integer xlength = -1, ylength = -1, dimensions;
		
		for(JsonObject.Member member: this.target) {
			value = member.getValue();
			name = member.getName();
			if(value.isArray()) {
				arrayhandler = new JSONArray(value.asArray());
				type = arrayhandler.getArrayVarType();
				dimensions = type.getArrayDimension(type);
				if(dimensions == 1) {
					xlength = arrayhandler.getXLength();
				} else if (dimensions == 2) {
					xlength = arrayhandler.getXLength();
					ylength = arrayhandler.getYLength();
				}
				this.signature.addVar(name, type, xlength, ylength, -1);
				xlength = -1;
				ylength = -1;
			} else if(value.isObject()) {
				throw new JSONObjectException("Second level of Objects not supported in array of objects");
			} else {
				type = this.utils.getJSONType(value);
				this.signature.addVar(name, type, null);
			}
			
		}
		//System.out.println("Signature:");
		//this.signature.printList();
	}
	
	public Boolean compareSignatures(LLHandler target) {
		
		LLNode hptrl, hptrr;
		
		if(this.signature.size() != target.size()) {
			return(false);
		}
		
		hptrl = this.signature.getHead().next;
		hptrr = target.getHead().next;
		
		while(true) {
			if(hptrl == null && hptrr == null) {
				break;
			} else if(hptrl == null && hptrr != null) {
				return(false);
			} else if (hptrl != null && hptrr == null) {
				return (false);
			} else if(!hptrl.name.equals(hptrr.name)) {
				return (false);
			} else if(hptrl.type != hptrr.type) {
				return(false);
			}
			
			hptrl = hptrl.next;
			hptrr = hptrr.next;
		}
		return(true);
	}
	
	public LLHandler getSignature() {
		return(this.signature);
	}
}
