package realHTML.tomcat.connector;

import realHTML.tomcat.connector.Environ;
import realHTML.tomcat.JSONMatcher.LLHandler;

public class JNINatural{
    public native RH4NReturn jni_callNatural(RH4NParams parms, Environ envvars[], LLHandler varlist);
    public native int jni_printVersion();
    public native int jni_dumpVars(LLHandler varlist);
    public native int jni_passwd(String username, String passwd);
}
