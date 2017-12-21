import realHTML.tomcat.JSONMatcher.*;
import realHTML.tomcat.connector.JNILoader;

public class LLTest {

    public static void main(String args[]) {
        JNILoader bs = new JNILoader();
        
    	/*LLHandler ll = new LLHandler();
        String test[] = {"test2"};
        String valuetest[] = {"val11", "val22"};
        String valuetest2d[][] = {{"val11", "val21"}, {"val12", "val22"}, {"val13", "val23"}};
        String valuetest3d[][][] = {
        		{
        			{"val111", "val211"}, 
        			{"val121", "val221"}
        		}, 
        		{
        			{"val112", "val212"}, 
        			{"val122", "val222"}
        		}
        	};

        ll.addVar("test1", "Value1");
        ll.addGrp("test2");
        ll.addVar("test3", "Value3");
        ll.addVar(test, "test4", "Value4");

//        ll.printList();
        
        ll.addGrp(test, "test5");
//        ll.printList();
        
        String test1[] = {"test2", "test5"};
        ll.addVar(test1, "test6", "Value6");
        
        ll.addVar("test7", valuetest);
        ll.addVar("test7", valuetest2d);
        ll.addVar("test7", valuetest3d);
        ll.printList();*/
        
        //String input = "{\"key1\": \"val1\", \"key2\":{\"key11\": {\"key111\": \"val111\"}, \"key12\": \"val12\"}, \"key3\": [\"test\"]}";
        //String input = "{\"key1\": [[11, 12, 13], [21, 22, 23]]}";
        String input = "{\"key1\": [[[111, 112, 113], [121, 122, 123]], [[211, 212, 213], [221, 222, 223]]], \"key2\": 410.23,  \"key3\": ["
        		+ "{\"key1\": \"Hello World1\", \"key2\": [[11], [12]]}, {\"key1\": \"Hello World2\", \"key2\": [[21], [22]]}"
        		+ "],"
        		+ "\"key4\": {\"key1\": \"Hello World\", \"key5\": [\"str1\", \"str2\", \"str3\"]}, \"key5\": [[\"val11\", \"val12\"], [\"val21\", \"val22\"]], \"key6\": [false, false, true], \"key7\": [\"String Numero uno\", \"String Numero duo\"],"
                + "\"key8\": [[11, 12], [21, 22]], \"key9\": [[[\"val000\", \"val001\"], [\"val010\", \"val011\"]], [[\"val100\", \"val101\"], [\"val110\", \"val111\"]]],"
                + "\"key10\": 34.8, \"key11\": false}";
        System.out.println(input);
        //String input = "{\"key1\": [1, 2, 3, 4], \"key2\": [\"5\", \"6\", \"7\", \"8\"]}";
        JSONParser parser = new JSONParser(input);
        LLHandler varlist = null;
        try {
        	varlist = parser.run();
        	varlist.printList();
        } catch(TypeException e) {
        	System.out.println(e);
        } catch(JSONArrayException e) {
        	System.out.println(e);
        } catch (JSONObjectException e) {
        	System.out.println(e);
        }
        bs.dumpVars(varlist);

        
        //System.out.println("\n");
        //System.out.println(ll.searchVar(null, test, "test4"));
    }

}
