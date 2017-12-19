package realHTML.tomcat.JSONMatcher;

import java.util.LinkedList;

import com.eclipsesource.json.*;

public class JSONArray {
	
	JsonArray target;
	String name;
	
	JSONUtils utils;
	LLHandler varlist;
	LinkedList<String> parents;
	ArrayType dimensions = null;
	
	public JSONArray(JsonArray array) {
		this.target = array;
		this.utils = new JSONUtils();
	}
	
	public JSONArray(JsonArray array, String name, LLHandler varlist) {
		this.target = array;
		this.name = name;
		
		this.utils = new JSONUtils();
		this.varlist = varlist;
	}
	
	public JSONArray(JsonArray array, String name, LinkedList<String> parents, LLHandler varlist) {
		this.target = array;
		this.name = name;
		
		this.utils = new JSONUtils();
		this.varlist = varlist;
		this.parents = parents;
	}
	
	public LLHandler getVarlist() {
		return(this.varlist);
	}
	
	public void handleArray() throws JSONArrayException, TypeException, JSONObjectException {
		ArrayType arraytype = getArrayDimensions();
		
		switch(arraytype) {
			case ARRAYEMPTY:
				throw new JSONArrayException("Array " + name + " is empty");
			case DIMNOTSUPPORTED:
				throw new JSONArrayException("More than three dimensions are not supported");
			case ONEDIM:
				this.handle1DArray(-1);
				break;
			case TWODIM:
				this.handle2DArray(-1);
				break;
			case THREEDIM:
				this.handle3DArray();
				break;
		}
	}
	
	public void handleArray(Integer xindex) throws JSONArrayException, TypeException, JSONObjectException {
		ArrayType arraytype = getArrayDimensions();
		
		switch(arraytype) {
			case ARRAYEMPTY:
				throw new JSONArrayException("Array " + name + " is empty");
			case DIMNOTSUPPORTED:
				throw new JSONArrayException("More than three dimensions are not supported");
			case ONEDIM:
				this.handle1DArray(xindex);
				break;
			case TWODIM:
				this.handle2DArray(xindex);
				break;
			case THREEDIM:
				System.out.println("Something is not right.");
				break;
		}
	}
	
	public ArrayType getArrayDimensions () throws JSONArrayException {
		JsonArray seconddim, thirddim;
		JsonValue tmp;
		
		// First check what array type it is
		if (this.target.isEmpty()) {
			throw new JSONArrayException("Array is empty");
		}
		
		tmp = this.target.get(0);
		if(tmp.isArray()) {
			//At least a 2D Array
			seconddim = tmp.asArray();
			if(seconddim.isEmpty()) {
				throw new JSONArrayException("Array is empty");
			}
			tmp = seconddim.get(0);
			if(tmp.isArray()) {
				//There are 3 Dims
				thirddim = tmp.asArray();
				if(thirddim.isEmpty()) {
					throw new JSONArrayException("Array is empty");
				}
				tmp = thirddim.get(0);
				if(tmp.isArray()) {
					throw new JSONArrayException("More than three dimensions are not supported");
				}
				this.dimensions = ArrayType.THREEDIM;
				return(ArrayType.THREEDIM);
			}
			this.dimensions = ArrayType.TWODIM;
			return(ArrayType.TWODIM);
		}
		this.dimensions = ArrayType.ONEDIM;
		return(ArrayType.ONEDIM);
	}
	
	public Types getArrayVarType() throws JSONArrayException {
		Types vartype = null;
		
		if (this.dimensions == null) {
			this.getArrayDimensions();
		}
		
		switch(this.dimensions) {
			case ONEDIM:
				vartype = this.getType1DArray();
				break;
			case TWODIM:
				vartype = this.getType2DArray();
				break;
			case THREEDIM:
				vartype = this.getType3DArray();
				break;
		}
		
		return(vartype.convertType(vartype, this.dimensions.getDimension()));
		
	}
	
	private Types getType1DArray() throws JSONArrayException {
		Types vartype = null, curtype;
		
		for(JsonValue value: this.target) {
			curtype = this.utils.getJSONType(value);
			if (vartype == null)  {
				vartype = curtype;
			} else if(vartype != curtype) {
				throw new JSONArrayException("Types in array are not uniform. Expected: ["+vartype.toString()+"] got ["+curtype.toString()+"]");
			}
		}
		return(vartype);
	}
	
	private Types getType2DArray() throws JSONArrayException {
		Types vartype = null, curtype;
		
		for(JsonValue value: this.target) {
			for(JsonValue value2d: value.asArray()) {
				curtype = this.utils.getJSONType(value2d);
				if (vartype == null)  {
					vartype = curtype;
				} else if(vartype != curtype) {
					throw new JSONArrayException("Types in array are not uniform. Expected: ["+vartype.toString()+"] got ["+curtype.toString()+"]");
				}
			}
		}
		return(vartype);
	}
	
	private Types getType3DArray() throws JSONArrayException {
		Types vartype = null, curtype;
		
		for(JsonValue value: this.target) {
			for(JsonValue value2d: value.asArray()) {
				for(JsonValue value3d: value2d.asArray()) {
					curtype = this.utils.getJSONType(value3d);
					if (vartype == null)  {
						vartype = curtype;
					} else if(vartype != curtype) {
						throw new JSONArrayException("Types in array are not uniform. Expected: ["+vartype.toString()+"] got ["+curtype.toString()+"]");
					}
				}
			}
		}
		return(vartype);
	}
	
	private void saveValue(String parents[], String name, JsonValue value, Types vartype, Integer xindex) throws TypeException{
		this.varlist.editVar(parents, name, getValue(value, vartype), xindex);
	}
	
	private void saveValue(String parents[], String name, JsonValue value, Types vartype, Integer xindex, Integer yindex) throws TypeException {
		this.varlist.editVar(parents, name, getValue(value, vartype), xindex, yindex);
	}
	private void saveValue(String parents[], String name, JsonValue value, Types vartype, Integer xindex, Integer yindex, Integer zindex) throws TypeException {
		this.varlist.editVar(parents, name, getValue(value, vartype), xindex, yindex, zindex);
	}
	
	private void handle1DArray(Integer xindex) throws JSONArrayException, TypeException, JSONObjectException {
		Types vartype = null;
		int array_length = 0, i = 0;
		Object[] val = null;
		
		vartype = this.getType1DArray();
		array_length = this.getXLength();
		
		if(array_length == 0) {
			throw new JSONArrayException("Array is empty");
		}
		
		if(vartype == Types.GROUP) {
			handle1DObjectArray(array_length);
			return;
		}
		
		if(xindex == -1) {
			val = (Object[])this.utils.createDummyVariable(vartype.convertType(vartype, 1), array_length, -1, -1);
			if (this.parents != null) {
				this.varlist.addVar(this.parents.toArray(new String[this.parents.size()]), name, vartype.convertType(vartype, 1), val);
			} else {
				this.varlist.addVar(name, vartype.convertType(vartype, 1), val);
			}
		}
		
		i = 0;
		for(JsonValue value: this.target) {
			if (xindex == -1) {
				if (this.parents != null) {
					this.saveValue(this.parents.toArray(new String[this.parents.size()]), name, value, vartype, i);
				} else {
					this.saveValue(null, name, value, vartype, i);
				}
			} else {
				if (this.parents != null) {
					this.saveValue(this.parents.toArray(new String[this.parents.size()]), name, value, vartype, xindex, i);
				} else {
					this.saveValue(null, name, value, vartype, xindex, i);
				}
			}
			i++;
		}
	}
	
	
	private void handle1DObjectArray(Integer xlength) throws JSONArrayException, TypeException, JSONObjectException {
		LLHandler signature = null;
		JSONObject objecthandler;
		LLNode hptr;
		Types shiftedtype;
		Object dummyvar = null;
		Integer dimensions, ylength = 0, zlength = 0;
		JSONArray arrayhandler;
		LinkedList<String> newparents;
		
		for(JsonValue value: this.target) {
			objecthandler = new JSONObject(value.asObject());
			objecthandler.createObjectSignature();
			if (signature == null) {
				signature = objecthandler.getSignature();
			} else {
				if(!objecthandler.compareSignatures(signature)) {
					throw new JSONArrayException("ObjectArray: Object signatures does not match.");
				}
			
			}
		}
		
		String[] grparray = new String[1];
		grparray[0] = this.name;
		newparents = new LinkedList<String>();
		newparents.add(this.name);
		
		hptr = signature.getHead().next;
		this.varlist.addGrp(this.name);
		
		// Safe the signature vars in the real varlist
		for(; hptr != null; hptr = hptr.next) {
			//Convert the type to the next dimension
			shiftedtype = hptr.type.convertType(hptr.type, 1);
			if(shiftedtype == Types.UNKNOWN) {
				throw new TypeException("Could not shift type ["+hptr.type.toString()+"]");
			}
			
			dimensions = shiftedtype.getArrayDimension(hptr.type);
			if (dimensions == 1) {
				ylength = hptr.xlength;
			} else if (dimensions == 2) {
				ylength = hptr.xlength;
				zlength = hptr.ylength;
			}
			dummyvar = this.utils.createDummyVariable(shiftedtype, xlength, ylength, zlength);
			ylength = 0;
			zlength = 0;
			
			dimensions = shiftedtype.getArrayDimension(shiftedtype);
			if (dimensions == 1) {
				this.varlist.addVar(grparray, hptr.name, shiftedtype, (Object[])dummyvar);
			} else if (dimensions == 2) {
				this.varlist.addVar(grparray, hptr.name, shiftedtype, (Object[][])dummyvar);
			} else if (dimensions == 3) {
				this.varlist.addVar(grparray, hptr.name, shiftedtype, (Object[][][])dummyvar);
			}
		}
		
		JsonValue tmp;
		String varname;
		Types vartype;
		Integer x = 0;
		
		for(JsonValue value: this.target) {
			for(JsonObject.Member member: value.asObject()) {
				tmp = member.getValue();
				varname = member.getName();
				if(!tmp.isArray()) {
					vartype = this.utils.getJSONType(tmp);
					saveValue(grparray, varname, tmp, vartype, x);
				} else if(tmp.isArray()) {
					arrayhandler = new JSONArray(tmp.asArray(), varname, newparents, this.varlist);
					arrayhandler.handleArray(x);
					this.varlist = arrayhandler.getVarlist();
				}
			}
			x++;
		}
		return;
	}
	
	private void handle2DArray(Integer xindex) throws JSONArrayException, TypeException {
		Types vartype = null;
		int array_length = -1, x = 0, y = 0;
		Object[][] val = null;
		
		vartype = getType2DArray();
		
		x = getXLength();
		if (x == 0) {
			throw new JSONArrayException("Array is empty");
		}
		array_length = getYLength();
		if (array_length == 0) {
			throw new JSONArrayException("Array is empty");
		}
		
		if(xindex == -1) {
			val = (Object[][])this.utils.createDummyVariable(vartype.convertType(vartype, 2), x, array_length, -1);
			if (this.parents != null) {
				this.varlist.addVar(this.parents.toArray(new String[this.parents.size()]), name, vartype.convertType(vartype, 2), val);
			} else {
				this.varlist.addVar(name, vartype.convertType(vartype, 2), val);
			}
		}
		
		x = 0; y = 0;
		
		for(JsonValue value: this.target) {
			y = 0;
			if (value.asArray().size() == 0) {
				throw new JSONArrayException("index ["+x+"]["+y+"] in ["+name+"] is empty");
			}
			for(JsonValue value2d: value.asArray()) {
				if (xindex == -1) {
					if (this.parents != null) {
						this.saveValue(this.parents.toArray(new String[this.parents.size()]), name, value2d, vartype, x, y);
					} else {
						this.saveValue(null, name, value2d, vartype, x, y);
					}
				} else {
					if (this.parents != null) {
						this.saveValue(this.parents.toArray(new String[this.parents.size()]), name, value2d, vartype, xindex, x, y);
					} else {
						this.saveValue(null, name, value2d, vartype, xindex, x, y);
					}
				}
				y++;
			}
			x++;
		}
		
	}
	
	private void handle3DArray() throws JSONArrayException, TypeException {
		Types vartype = null;
		int y_array_length = -1, z_array_length = -1, x = 0, y = 0, z = 0;
		Object[][][] val = null;
		
		vartype = getType3DArray();
		x = getXLength();
		if(x == 0) {
			throw new JSONArrayException("Array is empty");
		}
		y_array_length = getYLength();
		if(y_array_length == 0) {
			throw new JSONArrayException("Array is empty");
		}
		z_array_length = getZLength();
		if(z_array_length == 0) {
			throw new JSONArrayException("Array is empty");
		}
		
		val = (Object[][][])this.utils.createDummyVariable(vartype.convertType(vartype, 3), x, y_array_length, z_array_length);
		if (this.parents != null) {
			this.varlist.addVar(this.parents.toArray(new String[this.parents.size()]), name, vartype.convertType(vartype, 3), val);
		} else {
			this.varlist.addVar(name, vartype.convertType(vartype, 3), val);
		}
		
		x = 0; y = 0;
		
		for(JsonValue value: this.target) {
			y = 0;
			for(JsonValue value2d: value.asArray()) {
				z = 0;
				for(JsonValue value3d: value2d.asArray()) {
					if (this.parents != null) {
						this.saveValue(this.parents.toArray(new String[this.parents.size()]), name, value3d, vartype, x, y, z);
					} else {
						this.saveValue(null, name, value3d, vartype, x, y, z);
					}
					z++;
				}
				y++;
			}
			x++;
		}
	}
	
	private Object getValue(JsonValue value, Types vartype) throws TypeException {
		switch(vartype) {
		case STRING:
			return(value.asString());
		case FLOAT:
			return(value.asFloat());
		case INT:
			return(value.asInt());
		case BOOLEAN:
			return(value.asBoolean());
		default:
			throw new TypeException("Got unkown type: ["+vartype.toString()+"]");
		}
	}
	
	public Integer getXLength() {
		return (this.target.size());
	}
	
	public Integer getYLength() {
		return (this.target.get(0).asArray().size());
	}

	public Integer getZLength() {
		return (this.target.get(0).asArray().get(0).asArray().size());
	}
}
