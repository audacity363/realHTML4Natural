package realHTML.tomcat.connector;

import realHTML.tomcat.connector.Environ;
import realHTML.tomcat.JSONMatcher.LLHandler;

public class JNINatural{
    public native RH4NReturn jni_callNatural(RH4NParams parms, Environ envvars[]);
    public native int jni_printVersion();
    public native int jni_dumpVars(LLHandler varlist);
}
