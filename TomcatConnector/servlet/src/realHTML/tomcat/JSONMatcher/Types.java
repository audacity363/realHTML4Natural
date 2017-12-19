package realHTML.tomcat.JSONMatcher;

public enum Types {
	GROUP(0), 
	STRING(1), STRING1D(2), STRING2D(3), STRING3D(4),
	BOOLEAN(5), BOOLEAN1D(6), BOOLEAN2D(7), BOOLEAN3D(8),
	INT(9), INT1D(10), INT2D(11), INT3D(12),
	FLOAT(13), FLOAT1D(14), FLOAT2D(15), FLOAT3D(16),
	UNKNOWN(17);
	
	Integer number_rep;
	
	private Types(Integer number) {
		this.number_rep = number;
	}
	
	public int getNumberRep() {
		return (this.number_rep.intValue());
	}
	
	public Types convertType(Types base, Integer dimension) {
		if(base == STRING && dimension == 0) {
			return(STRING);
		} else if(base == STRING && dimension == 1) {
			return(STRING1D);
		} else if(base == STRING && dimension == 2) {
			return(STRING2D);
		} else if(base == STRING && dimension == 3) {
			return(STRING3D);
		}
		
		if(base == BOOLEAN && dimension == 0) {
			return(BOOLEAN);
		} else if(base == BOOLEAN && dimension == 1) {
			return(BOOLEAN1D);
		} else if(base == BOOLEAN && dimension == 2) {
			return(BOOLEAN2D);
		} else if(base == BOOLEAN && dimension == 3) {
			return(BOOLEAN3D);
		}
		
		if(base == INT && dimension == 0) {
			return(INT);
		} else if(base == INT && dimension == 1) {
			return(INT1D);
		} else if(base == INT && dimension == 2) {
			return(INT2D);
		} else if(base == INT && dimension == 3) {
			return(INT3D);
		}
		
		if(base == FLOAT && dimension == 0) {
			return(FLOAT);
		} else if(base == FLOAT && dimension == 1) {
			return(FLOAT1D);
		} else if(base == FLOAT && dimension == 2) {
			return(FLOAT2D);
		} else if(base == FLOAT && dimension == 3) {
			return(FLOAT3D);
		}
		
		if(base == STRING1D && dimension == 1) {
			return(STRING2D);
		} else if(base == STRING1D && dimension == 2) {
			return(STRING3D);
		}
		
		if(base == FLOAT1D && dimension == 1) {
			return(FLOAT2D);
		} else if(base == FLOAT1D && dimension == 2) {
			return(FLOAT3D);
		}
		
		if(base == INT1D && dimension == 1) {
			return(INT2D);
		} else if(base == FLOAT1D && dimension == 2) {
			return(INT3D);
		}
		
		if(base == BOOLEAN1D && dimension == 1) {
			return(BOOLEAN2D);
		} else if(base == BOOLEAN1D && dimension == 2) {
			return(BOOLEAN3D);
		}
		
		if(base == BOOLEAN2D && dimension == 1) {
			return(BOOLEAN3D);
		}
		
		if(base == FLOAT2D && dimension == 1) {
			return(FLOAT3D);
		}
		
		if(base == INT2D && dimension == 1) {
			return(INT3D);
		}
		
		if(base == STRING2D && dimension == 1) {
			return(STRING3D);
		}
		
		return(UNKNOWN);
	}
	
	public Integer getArrayDimension(Types target) {
		switch(target) {
			case INT:
			case STRING:
			case FLOAT:
			case BOOLEAN:
				return(0);
			case INT1D:
			case STRING1D:
			case FLOAT1D:
			case BOOLEAN1D:
				return(1);
			case INT2D:
			case STRING2D:
			case FLOAT2D:
			case BOOLEAN2D:
				return(2);
			case INT3D:
			case STRING3D:
			case FLOAT3D:
			case BOOLEAN3D:
				return(3);
			default:
				return(-1);
		}
	}
}
