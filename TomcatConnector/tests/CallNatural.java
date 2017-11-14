import realHTML.tomcat.connector.JNILoader;
import realHTML.tomcat.connector.RH4NParams;
import realHTML.tomcat.connector.RH4NReturn;
import realHTML.tomcat.connector.Environ;

public class CallNatural
{
    public static void main(String args[])
    {
        RH4NParams parms = new RH4NParams();
        RH4NReturn returncode;
        Environ envvars[];


        parms.keys = new String[10];
        parms.vals = new String[10];

        for(int i=0; i < 10; i++) {
           parms.keys[i] = String.format("Key %d", i); 
           parms.vals[i] = String.format("Value %d", i);
        }

        parms.reg_type = "GET";
        parms.nat_library = "TGAP0734";
        parms.nat_program = "PARMTEST";

        parms.tmp_file = "/u/it/a140734/C/rh4n/realHTML_TomcatConnector/tests/test.out";
        parms.settings = "templates=/u/it/a140734/C/realHtml4Natural/web_server/templates/;lib=TGAP0734;natsrc=/VAW/natural/abve/fuser63/";

        parms.natparams = "etid=$$ parm=lohnparm";

        parms.debug = "false";

        envvars = new Environ[1];
        envvars[0] = new Environ();
        envvars[0].name = "PROJECT";
        envvars[0].value = "bali";
        envvars[0].append = false;



        JNILoader bs = new JNILoader(); 

        returncode = bs.callNatural(parms, envvars);
        System.out.printf("Natprocess: [%d]\n", returncode.natprocess_ret);
        if(returncode.natprocess_ret != 0) {
            System.out.printf("Error: [%s]\n", returncode.error_msg);
        }

    }
}
