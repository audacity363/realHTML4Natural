package realHTML.tomcat.connector;

import java.io.*;
import java.util.*;
import realHTML.tomcat.connector.Environ;

//TODO: Clean up this class
public class RH4NCallParms {
    public String natinfos[] = new String[2];
    public List<String> req_keys;
    public List<String> req_vals;
    public String req_type;
    public Boolean debug = false;
    public Boolean deleteFile = false;
    public String loglevel = "";
    public String username = "";
    public String natparms = "";
    public String natsrcpath = "";

    public Boolean request_successful = false;

    public Environ enviromentvars[] = {};

}

