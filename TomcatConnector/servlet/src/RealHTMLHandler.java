import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

import java.util.*;
import java.util.Map.Entry;
import java.util.logging.FileHandler;
import java.nio.charset.Charset;

import realHTML.servlet.exceptions.*;
import realHTML.tomcat.connector.RH4NParams;
import realHTML.tomcat.connector.RH4NReturn;
import realHTML.tomcat.connector.Environ;
import realHTML.tomcat.connector.JNILoader;

import realHTML.tomcat.JSONMatcher.*;
import realHTML.tomcat.routing.PathTemplate;
import realHTML.tomcat.environment.Environment;
import realHTML.tomcat.environment.EnvironmentBuffer;
import realHTML.tomcat.environment.EnvironmentVar;

import org.apache.commons.io.*;

public class RealHTMLHandler extends RealHTMLInit {
    private JNILoader bs;

    public void init() throws ServletException {
        super.init();

        this.bs = new JNILoader();
        this.bs.printVersion();
    }

    public void doGet(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException {
        try {
            handleRequest(request, response);
        } catch(ServletException e) {
            throw(e);
        } catch(Exception e) {
            throw(new ServletException("", e));
        }
    }

    public void doPost(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException {
        try {
            handleRequest(request, response);
        } catch(ServletException e) {
            throw(e);
        } catch(Exception e) {
            throw(new ServletException("", e));
        }
    }

    public void doPut(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException {
        try {
            handleRequest(request, response);
        } catch(ServletException e) {
            throw(e);
        } catch(Exception e) {
            throw(new ServletException("", e));
        }
    }

    public void doDelete(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException {
        try {
            handleRequest(request, response);;
        } catch(ServletException e) {
            throw(e);
        } catch(Exception e) {
            throw(new ServletException("", e));
        }
    }

    private void handleRequest(HttpServletRequest request, HttpServletResponse response) throws ServletException {
        RH4NParams parms = new RH4NParams();
        LLHandler bodyvars = null;
        String contentType;
        Environ envvars[] = null;
        RH4NReturn natret = null;

        try {
            parms = getRoute(request, response);
            parms.outputfile = createTmpFile();
        } catch(Exception e) {
            throw(new ServletException(e));
        }

        parms.reqType = request.getMethod();
        parms.logpath = this.logpath;
        parms.errorRepresentation = "JSON";
        //envvars = call_parms.enviromentvars;

        parms = getQueryParms(request, parms);

        try {
            contentType = request.getContentType();
            if(contentType != null && contentType.equals("application/json")) {
                bodyvars = getBodyParms(request);
            }
        } catch(Exception e) {
            throw(new ServletException(e));
        }
            
        try {
            natret = bs.callNatural(parms, parms.environs, bodyvars);
            if(natret.natprocess_ret < 0) {
                sendErrorMessage(response, natret.error_msg);
                return;
            }
            deliverFile(response, parms.outputfile, true);

        } catch(Exception e) {
            throw(new ServletException(e));
        }
    }

    private void sendErrorMessage(HttpServletResponse response, String errormessage) 
        throws Exception {
        try {
            response.sendError(500, errormessage);
        } catch (Exception e) {
            throw(e);
        }
    }

    private String createTmpFile() 
        throws Exception {
        try  {
            File tmp = File.createTempFile("rh4n", "", new File("/tmp/"));
            return(tmp.getAbsolutePath());
        } catch (Exception e) {
            throw(e);
        }
    }

    private RH4NParams getRoute(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException {
        String environment, path;
        int index = 0;
        RH4NParams parms = new RH4NParams();
        HttpSession session = null;
        EnvironmentBuffer envs;
        Environment env;
        PathTemplate route;

        path = request.getRequestURI().substring((request.getContextPath() + "/nat").length());
        if(path.length() == 1) {
            throw(new ServletException(new RouteException("Enviroment and Route is missing")));
        }

        index = path.indexOf("/", 1);
        if(index == -1) {
            throw(new ServletException(new RouteException("No Route was given")));
        }

        environment = path.substring(1, index);
        path = path.replace("/" + environment, "");

        envs = EnvironmentBuffer.getEnvironmentsfromContext(getServletContext());
        try {
            env = envs.getEnvironment(environment);
        } catch(Exception e) {
            throw(new ServletException(e));
        }

        route = env.routing.getRoute(path);
        if(route == null) {
            throw(new ServletException("Unkown Route"));
        }

        if(route.route.login) {
            if(!checkLogin(request, response)) { return(null); }
            session = request.getSession();
            parms.username = (String)session.getAttribute("username");
        } else {
            parms.username = "";
        }

        parms.natLibrary = route.route.natLibrary;
        parms.natProgram = route.route.natProgram;
        parms.loglevel = route.route.loglevel;
        parms.natparms = env.natparms;
        parms.natsrcpath = env.natsourcepath;
        parms.environs = env.environvars.toArray(new EnvironmentVar[env.environvars.size()]);

        HashMap<String, String> routeparms = route.getParms();
        if(routeparms.size() != 0) {
            parms.urlVarsKey = new String[routeparms.size()];
            parms.urlVarsValue = new String[routeparms.size()];

            int i = 0;
            for(String key: routeparms.keySet()) {
                parms.urlVarsKey[i] = key;
                parms.urlVarsValue[i++] = routeparms.get(key);
            }

        } else {
            parms.urlVarsKey = null;
            parms.urlVarsValue = null;
        }

        return(parms);
    }

    private Boolean checkLogin(HttpServletRequest request, HttpServletResponse response) {
       HttpSession session = null;

       session = request.getSession();

       if(!session.isNew()) {
           return(true);
       }

       response.setStatus(HttpServletResponse.SC_FORBIDDEN);
       session.invalidate();
       return(false);
    }

    private RH4NParams getQueryParms(HttpServletRequest request, RH4NParams parms) {
        Map urlparms;
        int offset = 0, parmsize = 0;
        String tmpkeys[], tmpvalues[];

        urlparms = request.getParameterMap();
        if(urlparms.size() == 0) {
            if(parms.urlVarsKey == null) {
                parms.urlVarsKey = new String[0];
                parms.urlVarsValue = new String[0];
            }
            return(parms);
        }

        // Bullshit!!! offset it die länge des Arrays in parms
        parmsize = urlparms.size();
        if(parms.urlVarsKey != null) {
            offset = parms.urlVarsKey.length;
            tmpkeys = new String[offset+parmsize];
            tmpvalues = new String[offset+parmsize];
            System.arraycopy(parms.urlVarsKey, 0, tmpkeys, 0, offset);
            System.arraycopy(parms.urlVarsValue, 0, tmpvalues, 0, offset);
        } else {
            tmpkeys = new String[parmsize];
            tmpvalues = new String[parmsize];
        }

        for(String key: (Set<String>)urlparms.keySet()) {
            tmpkeys[offset] = key;
            tmpvalues[offset++] = ((String[])urlparms.get(key))[0];
        }

        parms.urlVarsKey = tmpkeys;
        parms.urlVarsValue = tmpvalues;

        return(parms);
    }

    private LLHandler getBodyParms(HttpServletRequest request) 
        throws IOException, Exception {
        String jsonString = "";
        int jsonChar = 0;
        BufferedReader br = null;
        LLHandler varlist = null;
        JSONParser jsonparser = null;

        try {
            br = new BufferedReader(new InputStreamReader(request.getInputStream()));
            if(br == null) {
                return(null);
            }

            for(jsonChar = br.read(); jsonChar != -1; jsonChar = br.read()) {
                jsonString += ((char)jsonChar);
            }
        } catch(IOException e) {
            throw(e);
        }

        if(jsonString.length() == 0) {
            return(null);
        }

        jsonparser = new JSONParser(jsonString);
        try {
            varlist = jsonparser.run();
            //varlist.printList();
        } catch(Exception e) {
            throw(e);
        }

        return(varlist);
    }

    private void deliverFile(HttpServletResponse response, String filepath, Boolean deletefile) 
        throws Exception {
        File returnfile = null;
        FileReader filereader = null;

        response.setContentType("text/html");
        try {
            returnfile = new File(filepath);
            filereader =  new FileReader(filepath);
            IOUtils.copy(filereader, response.getOutputStream());
        } catch(Exception e) {
            throw(e);
        }

        if(deletefile) {
            returnfile.delete();
        }
    }
}
