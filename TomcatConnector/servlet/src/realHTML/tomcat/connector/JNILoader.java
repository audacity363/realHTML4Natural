package realHTML.tomcat.connector;

import realHTML.tomcat.environment.EnvironmentVar;
import realHTML.tomcat.JSONMatcher.LLHandler;

public class JNILoader
{
    static 
    {
        System.out.println("Using java.library.path: [" + System.getProperty("java.library.path") + "]");
        //System.out.println("Loading sharedlib");
        System.loadLibrary("realHTMLconnector");
        //System.load("/u/it/a140734/C/realHTML_TomcatConnector/librealHTMLconnector.so");
        //System.out.println("Lib Loaded");
    }

    public RH4NReturn callNatural(RH4NParams parms, EnvironmentVar envvars[], LLHandler varlist)
    {
        return(new JNINatural()).jni_callNatural(parms, envvars, varlist);
    }
    public int printVersion()
    {
        return(new JNINatural()).jni_printVersion();
    }

    public int dumpVars(LLHandler varlist) 
    {
        return(new JNINatural()).jni_dumpVars(varlist);
    }

    public int passwd(String user, String passwd) {
        return(new JNINatural()).jni_passwd(user, passwd);
    }

}
