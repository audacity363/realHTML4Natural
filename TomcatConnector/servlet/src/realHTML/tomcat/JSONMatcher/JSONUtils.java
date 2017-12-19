package realHTML.tomcat.JSONMatcher;

import com.eclipsesource.json.*;

public class JSONUtils {
	public Types getJSONType(JsonValue value) {
		String stringval;
		
		if(value.isBoolean()) {
			return(Types.BOOLEAN);
		} else if(value.isNumber()) {
			stringval = value.toString();
			if(stringval.indexOf('.') >= 0)
			{
				return(Types.FLOAT);
			}
			return(Types.INT);
		} else if(value.isString()) {
			return(Types.STRING);
		} else if(value.isObject()) {
			return(Types.GROUP);
		}
		return(Types.UNKNOWN);
	}
	
	public Integer getXArrayLength(JsonArray array) {
		return (array.size());
	}
	
	public Integer getYArrayLength(JsonArray array) {
		return(array.get(0).asArray().size());
	}
	
	public Object createDummyVariable(Types type, Integer xlength, Integer ylength, Integer zlength) {
		Object dummyvar = null;
		
		switch(type) {
			case INT:
				dummyvar = (Object) new Integer(0);
				break;
			case INT1D:
				dummyvar = (Object) new Integer[xlength];
				break;
			case INT2D:
				dummyvar = (Object) new Integer[xlength][ylength];
				break;
			case INT3D:
				dummyvar = (Object) new Integer[xlength][ylength][zlength];
				break;
			case STRING:
				dummyvar = (Object) new String();
				break;
			case STRING1D: 
				dummyvar = (Object) new String[xlength];
				break;
			case STRING2D: 
				dummyvar = (Object) new String[xlength][ylength];
				break;
			case STRING3D: 
				dummyvar = (Object) new String[xlength][ylength][zlength];
				break;
			case BOOLEAN:
				dummyvar = (Object) new Boolean(false);
				break;
			case BOOLEAN1D:
				dummyvar = (Object) new Boolean[xlength];
				break;
			case BOOLEAN2D:
				dummyvar = (Object) new Boolean[xlength][ylength];
				break;
			case BOOLEAN3D:
				dummyvar = (Object) new Boolean[xlength][ylength][zlength];
				break;
			case FLOAT:
				dummyvar = (Object) new Float(0);
				break;
			case FLOAT1D:
				dummyvar = (Object) new Float[xlength];
				break;
			case FLOAT2D:
				dummyvar = (Object) new Float[xlength][ylength];
				break;
			case FLOAT3D:
				dummyvar = (Object) new Float[xlength][ylength][zlength];
				break;
			default:
				dummyvar = null;
		}
		
		return(dummyvar);
	}
}
