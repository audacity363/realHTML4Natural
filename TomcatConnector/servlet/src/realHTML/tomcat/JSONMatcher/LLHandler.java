package realHTML.tomcat.JSONMatcher;

import java.util.Arrays;
import java.util.LinkedList;

public class LLHandler {
	
	private LLNode head;
	private int size;
	
	public LLHandler() {
		this.head = new LLNode("anker");
	}
	
	
	public int size() { return size; }
	public LLNode getHead() { return (this.head); }
    
    private LLNode searchGrp(LLNode head, String[] parents) {
    	LLNode hptr;

        String[] newparents;
        
        if (head == null) {
        	head = this.head.next;
        }

        for(hptr = head; hptr != null; hptr = hptr.next) 
        {
            if(hptr.name.equals(parents[0])) 
            {
            	if(parents.length > 1) {
                    newparents = Arrays.copyOfRange(parents, 1, parents.length);
                    return(searchGrp(hptr.nextlvl, newparents));

                } else {
                    return(hptr);
                }
            }
        }
        return(null);
    }
    
    private Object create1DArray(Object[] value) {
    	LinkedList<Object> llvalue;
    	
    	llvalue = new LinkedList<Object>();
    	for(int i=0; i < value.length; i++) {
    		llvalue.add(value[i]);
    	}
    	return((Object)llvalue);
    }
    
    private Object create2DArray(Object[][] value) {
    	LinkedList<LinkedList<Object>> llvalue;
    	LinkedList<Object> hptr;
    	
    	llvalue = new LinkedList<LinkedList<Object>>();
    	for(int i=0; i < value.length; i++) {
    		llvalue.add(new LinkedList<Object>());
    		hptr = llvalue.get(i);
    		for(int x = 0; x < value[i].length; x++) {
    			hptr.add(value[i][x]);
    		}
    	}
    	
    	return(llvalue);
    }
    
    private Object create3DArray(Object[][][] value) {
    	LinkedList<LinkedList<LinkedList<Object>>> llvalue;
    	LinkedList<LinkedList<Object>> lhptr;
    	LinkedList<Object> shptr;
    	
    	llvalue = new LinkedList<LinkedList<LinkedList<Object>>>();
    	
    	for(int i=0; i < value.length; i++) {
    		llvalue.add(new LinkedList<LinkedList<Object>>());
    		
    		lhptr = llvalue.get(i);
    		for(int x = 0; x < value[i].length; x++) {
    			lhptr.add(new LinkedList<Object>());
    			shptr = lhptr.get(x);
    			for(int y = 0; y < value[i][x].length; y++) {
    				shptr.add(value[i][x][y]);
    			}
    		}
    	}
    	
    	return((Object)llvalue);
    }

    private LLNode _addVar(String name) {
    	LLNode tmp = new LLNode(name), hptr;

        for(hptr = head; hptr.next != null; hptr = hptr.next);
        hptr.next = tmp;
        tmp.prev = hptr;
        this.size++;
        return (tmp);
    }
    
    private LLNode _addVar(String parents[], String name) {
    	LLNode hptr, tmp, grp;
        
        grp = searchGrp(this.head.next, parents);
        if(grp == null) {
        	return (null);
        }
        
        tmp = new LLNode(name);
        
        if(grp.nextlvl == null) {
        	grp.nextlvl = tmp;
        	tmp.prev = grp.nextlvl;
        	return(tmp);
        }
        
        for(hptr = grp.nextlvl; hptr.next != null; hptr = hptr.next);
        
        hptr.next = tmp;
        tmp.prev = tmp;
        
        this.size++;
        return(tmp);
    }
    
    public void addGrp(String name) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	
    	tmp.type = Types.GROUP;
    }
    
    public void addGrp(String[] parents, String name) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.GROUP;
    }
    
    public void addVar(String name, Types type, Integer xlength, Integer ylength, Integer zlength) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = type;
    	tmp.xlength = xlength;
    	tmp.ylength = ylength;
    	tmp.zlength = zlength;
    }
    
    public void addVar(String name, Types type, Object value) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = type;
    	tmp.value = value;
    }
    
    public void addVar(String name, Types type, Object[] value) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = type;
    	if (value == null) {
    		tmp.value = null;
    	} else {
    		tmp.value = create1DArray(value);
    	}
    }
    
    public void addVar(String name, Types type, Object[][] value) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = type;
    	if (value == null) {
    		tmp.value = null;
    	} else {
    		tmp.value = create2DArray(value);
    	}
    }
    
    public void addVar(String name, Types type, Object[][][] value) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = type;
    	if (value == null) {
    		tmp.value = null;
    	} else {
    		tmp.value = create3DArray(value);
    	}
    }
    
    public void addVar(String[] parents, String name, Types type, Object value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = type;
    	tmp.value = value;
    }
    
    public void addVar(String[] parents, String name, Types type, Object[] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = type;
    	if (value == null) {
    		tmp.value = null;
    	} else {
    		tmp.value = create1DArray(value);
    	}
    }
    
    public void addVar(String[] parents, String name, Types type, Object[][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = type;
    	if (value == null) {
    		tmp.value = null;
    	} else {
    		tmp.value = create2DArray(value);
    	}
    }
    
    public void addVar(String[] parents, String name, Types type, Object[][][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = type;
    	if (value == null) {
    		tmp.value = null;
    	} else {
    		tmp.value = create3DArray(value);
    	}
    }
    
    public void addVar(String name, String value) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = Types.STRING;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String name, String[] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.STRING1D; 	
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String name, String[][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.STRING2D;    	
    	tmp.value = create2DArray((Object[][])value);
    }
    
    public void addVar(String name, String[][][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.STRING3D;
    	tmp.value = create3DArray((Object[][][])value);
    }
    
    public void addVar(String name, Integer value) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = Types.INT;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String name, Integer[] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.INT1D; 	
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String name, Integer[][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.INT2D;    	
    	tmp.value = create2DArray((Object[][])value);
    }
    
    public void addVar(String name, Integer[][][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.INT3D;
    	tmp.value = create3DArray((Object[][][])value);
    }
    
    public void addVar(String name, Boolean value) {
    	LLNode tmp;
    	tmp = _addVar(name);
    	tmp.type = Types.BOOLEAN;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String name, Boolean[] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.BOOLEAN1D; 	
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String name, Boolean[][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.BOOLEAN2D;    	
    	tmp.value = create2DArray((Object[][])value);
    }
    
    public void addVar(String name, Boolean[][][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.BOOLEAN3D;
    	tmp.value = create3DArray((Object[][][])value);
    }
    
    public void addVar(String name, Float value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.FLOAT;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String name, Float [] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.FLOAT1D; 	
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String name, Float[][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.FLOAT2D;    	
    	tmp.value = create2DArray((Object[][])value);
    }
    
    public void addVar(String name, Float[][][] value) {
    	LLNode tmp = _addVar(name);
    	tmp.type = Types.FLOAT3D;
    	tmp.value = create3DArray((Object[][][])value);
    }
    
    public void addVar(String parents[], String name, String value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.STRING;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String parents[], String name, String[] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.STRING1D;
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String parents[], String name, String[][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.STRING2D;
    	tmp.value = create1DArray((Object[][])value);
    }
    
    public void addVar(String parents[], String name, String[][][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.STRING3D;
    	tmp.value = create1DArray((Object[][][])value);
    }
    
    public void addVar(String parents[], String name, Integer value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.INT;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String parents[], String name, Integer[] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.INT1D;
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String parents[], String name, Integer[][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.INT2D;
    	tmp.value = create1DArray((Object[][])value);
    }
    
    public void addVar(String parents[], String name, Integer[][][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.INT3D;
    	tmp.value = create1DArray((Object[][][])value);
    }
    
    public void addVar(String parents[], String name, Boolean value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.BOOLEAN;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String parents[], String name, Boolean[] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.BOOLEAN1D;
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String parents[], String name, Boolean[][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.BOOLEAN2D;
    	tmp.value = create1DArray((Object[][])value);
    }
    
    public void addVar(String parents[], String name, Boolean[][][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.BOOLEAN3D;
    	tmp.value = create1DArray((Object[][][])value);
    }
    
    public void addVar(String parents[], String name, Float value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.FLOAT;
    	tmp.value = (Object)value;
    }
    
    public void addVar(String parents[], String name, Float[] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.FLOAT1D;
    	tmp.value = create1DArray((Object[])value);
    }
    
    public void addVar(String parents[], String name, Float[][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.FLOAT2D;
    	tmp.value = create1DArray((Object[][])value);
    }
    
    public void addVar(String parents[], String name, Float[][][] value) {
    	LLNode tmp;
    	tmp = _addVar(parents, name);
    	tmp.type = Types.FLOAT3D;
    	tmp.value = create1DArray((Object[][][])value);
    }


    public LLNode searchVar(LLNode head, String[] parents, String name) {
        LLNode hptr;
        String searchparm;

        String[] newparents;
        
        if (head == null) {
        	head = this.head.next;
        }

        if(parents != null) {
            searchparm = parents[0];
        } else {
            searchparm = name;
        }

        for(hptr = head; hptr != null; hptr = hptr.next) 
        {
            if(hptr.name.equals(searchparm)) 
            {
            	if(parents == null) {
            		return(hptr);
            	} else if(parents.length > 1) {
                    newparents = Arrays.copyOfRange(parents, 1, parents.length);
                    return(searchVar(hptr.nextlvl, newparents, name));

                } else {
                    return(searchVar(hptr.nextlvl, null, name));
                }
            }
        }


        return(null);

    }
    
    @SuppressWarnings("unchecked")
	public void editVar(String parents[], String name, Object value, Integer xindex) {
    	LLNode tmp;
    	
    	tmp = this.searchVar(this.head.next, parents, name);
    	if (tmp == null) {
    		System.out.println("Variable " + parents.toString() + " " + name + "not found");
    		return;
    	}
    	
    	((LinkedList<Object>)tmp.value).set(xindex, value);
    }
    
    @SuppressWarnings("unchecked")
	public void editVar(String parents[], String name, Object value, Integer xindex, Integer yindex) {
    	LLNode tmp;
    	LinkedList<Object> firstdim;
    	
    	tmp = this.searchVar(this.head.next, parents, name);
    	
    	firstdim = ((LinkedList<Object>)tmp.value);
    	
    	((LinkedList<Object>)firstdim.get(xindex)).set(yindex, value);
    }
    
    @SuppressWarnings("unchecked")
	public void editVar(String parents[], String name, Object value, Integer xindex, Integer yindex, Integer zindex) {
    	LLNode tmp;
    	LinkedList<Object> firstdim, seconddim;
    	
    	tmp = this.searchVar(this.head.next, parents, name);
    	
    	firstdim = ((LinkedList<Object>)tmp.value);
    	
    	seconddim = ((LinkedList<Object>)firstdim.get(xindex));
    	((LinkedList<Object>)seconddim.get(yindex)).set(zindex, value);
    }
   
    private void printGrp(LLNode grphead, int level) {
        LLNode hptr = grphead;
        PrintFuncs varprinter = new PrintFuncs();

        for(;hptr != null; hptr = hptr.next) {
        	varprinter.printEntry(hptr, level);

            if(hptr.nextlvl != null) {
                printGrp(hptr.nextlvl, level+1);
            }
        }
    }

    public void printList() {
        LLNode tmp = head.next;
        PrintFuncs varprinter = new PrintFuncs();

        System.out.println("-------------------------------------------------------");
        for(tmp = head.next; tmp != null; tmp = tmp.next) {
        	varprinter.printEntry(tmp, 0);
            if(tmp.nextlvl != null) {
                printGrp(tmp.nextlvl, 1);
            }
        }
        System.out.println("-------------------------------------------------------");
    }
	 
}
