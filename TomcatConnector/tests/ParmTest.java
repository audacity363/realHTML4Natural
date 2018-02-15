import realHTML.tomcat.connector.JNILoader;
import realHTML.tomcat.connector.JNIParms;
import realHTML.tomcat.connector.RH4NParams;

public class ParmTest {
   public static void main(String[] args)  {
        JNILoader bs = new JNILoader();
        RH4NParams parms = new RH4NParams();

        parms.reqType = new String("GET");
        parms.nat_library = "TGAP0734";
        parms.nat_program = "TESTPROG";
        parms.natparms = "etid=$$";
        parms.outputfile = "/tmp/output.html";
        parms.loglevel = "DEBUG";
        parms.natsrcpath = "/VAW/nat";

        
        bs.callNatural(parms, null, null);
   }
}
