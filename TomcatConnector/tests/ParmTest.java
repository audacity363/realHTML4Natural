import realHTML.tomcat.connector.JNILoader;
import realHTML.tomcat.connector.RH4NParams;
import realHTML.tomcat.connector.Environ;
import realHTML.tomcat.JSONMatcher.*;

public class ParmTest {
   public static void main(String[] args)  {
        JNILoader bs = new JNILoader();
        RH4NParams parms = new RH4NParams();
        Environ[] environs = {
                new Environ("var1", "value1", false),
                new Environ("var2", "value2", false),
                new Environ("var3", "value3", true),
                new Environ("var4", "value4", false),
                new Environ("var5", "value5", true)
            };

        parms.reqType = new String("GET");
        parms.natLibrary = "TGAP0734";
        parms.natProgram = "TESTPROG";
        parms.natparms = "etid=$$";
        parms.outputfile = "/tmp/output.html";
        //parms.loglevel = "DEVELOP";
        parms.loglevel = "DEBUG";
        parms.natsrcpath = "/VAW/nat";
        parms.logpath = "/tmp/";
        parms.errorRepresentation = "JSON";

        parms.urlVarsKey = new String[4];
        parms.urlVarsKey[0] = "URLVar1";
        parms.urlVarsKey[1] = "URLVar2";
        parms.urlVarsKey[2] = "URLVar3";
        parms.urlVarsKey[3] = "URLVar4";

        parms.urlVarsValue = new String[4];
        parms.urlVarsValue[0] = "URLValue1";
        parms.urlVarsValue[1] = "URLValue2";
        parms.urlVarsValue[2] = "URLValue3";
        parms.urlVarsValue[3] = "URLValue4";

        try {
            JSONParser jsonparser = new JSONParser("{\"var1\": 123,\"var2\":\"val2\"}");
            LLHandler varlist = jsonparser.run();
            

            
            bs.callNatural(parms, environs, varlist);
        } catch(Exception e) {
            System.out.println(e);
        }
   }
}
