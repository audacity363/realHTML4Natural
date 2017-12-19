package realHTML.tomcat.JSONMatcher;

public class LLNode {
	String name;
    Types type;
    Object value;
    
    Integer xlength;
    Integer ylength;
    Integer zlength;

    LLNode next;
    LLNode nextlvl;
    LLNode prev;
    
    public LLNode(String name) {
    	this.name = name;
    	this.xlength = -1;
    	this.ylength = -1;
    	this.zlength = -1;
    }
    
    public LLNode(String name, Types type) {
    	this.name = name;
        this.type = type;
    }

    public LLNode(String name, Types type, String value) {
        this.name = name;
        this.type = type;
        this.value = (Object)value;
    }
    
    public LLNode(String name, Types type, Integer xlength, Integer ylength, Integer zlength) {
    	this.name = name;
    	this.type = type;
    	this.xlength = xlength;
    	this.ylength = ylength;
    	this.zlength = zlength;
    }

    public String toString() {
        return this.name;
    }
}
