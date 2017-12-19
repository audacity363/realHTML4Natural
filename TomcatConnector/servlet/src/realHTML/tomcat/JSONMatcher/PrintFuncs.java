package realHTML.tomcat.JSONMatcher;

import java.util.Iterator;
import java.util.LinkedList;

public class PrintFuncs {
	
	public PrintFuncs() {
		
	}
	
	public void printEntry(LLNode target, int level) { 	
    	printTabs(level); System.out.printf("Name: %s\n", target.name);
    	printTabs(level); System.out.printf("Type: %s\n", target.type.toString());
    	if (target.xlength != -1) { printTabs(level); System.out.printf("X Length: %s\n", target.xlength); }
    	if (target.ylength != -1) { printTabs(level); System.out.printf("Y Length: %s\n", target.ylength); }
    	if (target.zlength != -1) { printTabs(level); System.out.printf("Z Length: %s\n", target.zlength); }
    	
    	printTabs(level);
    	if (target.value != null) {
    		System.out.print("Value:");
    		switch (target.type) {
    			case STRING:
    			case BOOLEAN:
    			case INT:
    			case FLOAT:
    				System.out.println(target.value);
    				break;
    			case STRING1D:
    			case BOOLEAN1D: 
    			case INT1D:
    			case FLOAT1D:
    				print1DArray(target.value);
    				break;
    			case STRING2D:
    			case BOOLEAN2D:
    			case INT2D:
    			case FLOAT2D:
    				print2DArray(target.value);
    				break;
    			case STRING3D:
    			case BOOLEAN3D:
    			case INT3D:
    			case FLOAT3D:
    				print3DArray(target.value);
    			case GROUP:
    				break;
    			case UNKNOWN:
    				break;
    		}
    	}
    	System.out.println();
    }
	
	private void printTabs(int level) {
        for(int i=0; i < level; i++) {
            System.out.print("\t");
        }
    }
    
    private void print1DArray(Object value) {
    	@SuppressWarnings("unchecked")
    	Iterator<Object> shptr = ((LinkedList<Object>)value).iterator();
    	System.out.print("[");
    	
    	while(shptr.hasNext()) {
    		System.out.print(shptr.next());
    		if(shptr.hasNext()) { System.out.print(", "); };
    	}
    	System.out.println("]");	
    }
    
    private void print2DArray(Object value) {
    	@SuppressWarnings("unchecked")
    	Iterator<Object> llhptr = ((LinkedList<Object>)value).iterator();
    	System.out.print("[");
    	while(llhptr.hasNext()) {
    		System.out.print("[");
    		
    		@SuppressWarnings("unchecked")
    		Iterator<Object> shptr = ((LinkedList<Object>)llhptr.next()).iterator();
    		while(shptr.hasNext()){
    			System.out.print(shptr.next());
    			
    			if(shptr.hasNext()) { System.out.print(", "); };
    		}
    		System.out.print("]");
    		
    		if(llhptr.hasNext()) { System.out.print(", "); };
    		
    	}
    	System.out.println("]");
    }
    
    private void print3DArray(Object value) {
    	@SuppressWarnings("unchecked")
    	Iterator<Object> lllhptr = ((LinkedList<Object>)value).iterator();
    	
    	System.out.print("[");
    	while(lllhptr.hasNext()) {
    		System.out.print("[");
    		
    		@SuppressWarnings("unchecked")
    		Iterator<Object> llhptr = ((LinkedList<Object>)lllhptr.next()).iterator();
    		
    		while(llhptr.hasNext()) {
    			System.out.print("[");
    			@SuppressWarnings("unchecked")
    			Iterator<Object> shptr = ((LinkedList<Object>)llhptr.next()).iterator();
    			
    			while(shptr.hasNext()) {
    				System.out.print(shptr.next());
    				if(shptr.hasNext()) { System.out.print(", "); };
    			}
    			System.out.print("]");
    			if(llhptr.hasNext()) { System.out.print(", "); };
    		}
    		
    		System.out.print("]");
    		if(lllhptr.hasNext()) { System.out.print(", "); };
    	}
    	System.out.println("]");
    }
}
