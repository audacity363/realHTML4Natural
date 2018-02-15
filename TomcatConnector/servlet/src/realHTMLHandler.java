import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

import java.util.*;
import java.util.logging.FileHandler;
import java.nio.charset.Charset;

import realHTML.servlet.exceptions.*;
import realHTML.tomcat.connector.*;
import realHTML.tomcat.JSONMatcher.*;

import org.apache.commons.io.*;

public class realHTMLHandler extends HttpServlet {

    String configurationfile = "";
    static String default_tags[] = {"routes", "templates", "debug", "naterror", "ldaerror", "natparms", "natsourcepath"};
    private JNILoader bs;

    public void init() throws ServletException {
        this.configurationfile = System.getenv("realHTMLconfiguration");
        if(this.configurationfile == null) {
            throw(new ServletException("Enviroment variable \"realHTMLconfiguration\" is missing"));
        }

        bs = new JNILoader();
        bs.printVersion();
        System.out.println("Loaded jni Library");
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
            handleRequest(request, response);
        } catch(ServletException e) {
            throw(e);
        } catch(Exception e) {
            throw(new ServletException("", e));
        }
    }

    private void handleRequest(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException, IOException, RouteException, Exception {
        RH4NCallParms call_parms;
        RH4NParams parms = new RH4NParams();
        LLHandler bodyvars = null;
        String contentType;
        Boolean deleteFile = false;
        Environ envvars[] = null;
        RH4NReturn natret = null;

        try {
            call_parms = getRoute(request, response);
            if(call_parms == null) {
                return;
            }

            parms.tmp_file = createTmpFile();

            if(call_parms.debug) {
                parms.loglevel = "DEBUG";
            } else {
                parms.loglevel = "ERROR";
            }

            parms.reg_type = request.getMethod();

            parms.nat_library = call_parms.natinfos[0];
            parms.nat_program = call_parms.natinfos[1];

            parms.natparams = call_parms.settings.get("natparms");

            deleteFile = call_parms.deleteFile;
            envvars = call_parms.enviromentvars;

            //parms.settings = createSettingsString(call_parms.settings.get("templates"), call_parms.natinfos[0],
            //call_parms.settings.get("natsourcepath"), call_parms.debug);
            parms.natsrcpath = call_parms.settings.get("natsourcepath");

            call_parms = getQueryParms(request);
            parms.urlVarsKey = call_parms.req_keys.toArray(new String[0]);
            parms.urlVarsValue = call_parms.req_vals.toArray(new String[0]);

            contentType = request.getContentType();
            if(contentType != null && contentType.equals("application/json")) {
                bodyvars = getBodyParms(request);
            }

            natret = bs.callNatural(parms, envvars, bodyvars);
            if(natret.natprocess_ret < 0) {
                sendErrorMessage(response, natret.error_msg);
                return;
            }
            deliverFile(response, parms.tmp_file, deleteFile);

        } catch(ServletException e) {
            throw(e);
        } catch(RouteException e) {
            throw(e);
        } catch(Exception e) {
            throw(e);
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

    private String createSettingsString(String templatepath, String natlib, String fuserpath, Boolean debug) {
        String settings;

        settings = String.format("templates=%s;lib=%s;natsrc=%s;debug=%b",
            templatepath, natlib, fuserpath, debug);

        return(settings);
    }
    
    private RH4NCallParms getRoute(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException, RouteException {
        String enviroment, path;
        int index = 0;
        ConfigurationLoader cl;
        RH4NCallParms parms = new RH4NCallParms();
        Router r;
        Route route = null;

        //System.out.println("RequestURI: " + request.getRequestURI());
        //System.out.println("ContextPath: " + request.getContextPath());
        path = request.getRequestURI().substring((request.getContextPath() + "/nat").length());
        if(path.length() == 1) {
            throw(new RouteException("Enviroment and Route is missing"));
        }

        index = path.indexOf("/", 1);
        if(index == -1) {
            throw(new RouteException("No Route was given"));
        }

        enviroment = path.substring(1, index);
        path = path.replace("/" + enviroment, "");

        cl = new ConfigurationLoader(this.configurationfile, default_tags);

        try {
            if(!cl.readConfiguration(enviroment)) {
                throw(new RouteException("Enviroment " + enviroment + " was not found"));
            }
        } catch(Exception e) {
            throw(new RouteException(e.getMessage()));
        }

        parms.enviromentvars = cl.getEnviromentVars();
        parms.settings = cl.getResult();

        r = new Router(parms.settings.get("routes"));
        try {
            if(!r.searchRoute(path)) {
                throw(new RouteException("Route " + path + " was not found"));
            }
        } catch (Exception e) {
            throw(new RouteException(e.getMessage()));
        }

        route = r.getRoute();
        if(route.login) {
            if(!checkLogin(request, response)) {
                return(null);
            }
        }

        parms.natinfos[0] = route.library; //r.getLibrary();
        parms.natinfos[1] = route.program; //r.getProgram();
        parms.debug = route.debug; //r.getDebug();
        parms.deleteFile = route.deletefile; //r.getDeleteFile();

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

    private RH4NCallParms getQueryParms(HttpServletRequest request) {
        RH4NCallParms parms = new RH4NCallParms();
        Iterator parmiterator;
        Map.Entry<String, String[]> entry;

        parms.req_keys = new ArrayList<String>();
        parms.req_vals = new ArrayList<String>();

        parmiterator = request.getParameterMap().entrySet().iterator();

        while(parmiterator.hasNext()) {
            entry = (Map.Entry<String, String[]>)parmiterator.next();
            parms.req_keys.add(entry.getKey());
            parms.req_vals.add(entry.getValue()[0]);
        }

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
            varlist.printList();
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
