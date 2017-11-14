package realHTML.tomcat.connector;

import realHTML.tomcat.connector.Environ;

public class JNINatural{
    public native RH4NReturn jni_callNatural(RH4NParams parms, Environ envvars[]);
    public native int jni_printVersion();
}
