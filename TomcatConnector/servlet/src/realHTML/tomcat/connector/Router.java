package realHTML.tomcat.connector; 

import java.io.*;
import java.io.File;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

import realHTML.servlet.exceptions.*;

public class Router
{
    String filepath = null;
    String program = null;
    String library = null;
    Boolean debug = false;
    Boolean deletefile = true;
    String error_msg; 

    Route route;

    String[] supportedTags = {
        "library",
        "program",
        "debug", 
        "deleteFile",
        "login",
        "logging"
        };

    public Router(String filepath)
    {
        this.filepath = filepath;
        this.route = new Route();
    }

    public Boolean searchRoute(String route) 
        throws ParserConfigurationException, SAXException, IOException, RouteException
    {
        File inputFile = new File(this.filepath);

        DocumentBuilderFactory dbFactory; 
        DocumentBuilder dBuilder;
        Document doc;
        NodeList nList, target;
        Node nNode;
        Element eElement;
        
        dbFactory = DocumentBuilderFactory.newInstance();
        dBuilder = dbFactory.newDocumentBuilder();

        doc = dBuilder.parse(inputFile);
        doc.getDocumentElement().normalize();

        nList = doc.getElementsByTagName("route");


        for(int i=0; i < nList.getLength(); i++)
        {
            nNode = nList.item(i);
            eElement = (Element)nNode;

            if(eElement.getAttribute("path").equals(route))
            {
                for(int x = 0; x < this.supportedTags.length; x++) {
                    target = eElement.getElementsByTagName(this.supportedTags[x]);
                    handleTag(this.supportedTags[x], target);
                }

                return(true);
            }
        }
        this.error_msg = "no route found";
        return(false);
    }

    //TODO: Find a nicer way to distribute the handler call (function pointers?!)
    private void handleTag(String tag, NodeList target) throws RouteException {
        if(tag.equals("library")) {
            handleLibrary(target);
        } else if(tag.equals("program")) {
                handleProgram(target);
        } else if(tag.equals("debug")) {
                handleDebug(target);
        } else if(tag.equals("deleteFile")) {
                handleDeleteFile(target);
        } else if(tag.equals("login")) {
                handleLogin(target);
        } else if(tag.equals("logging")) {
            handleLogging(target);
        }
    }

    private void handleLibrary(NodeList target) throws RouteException {
        if(target.getLength() == 0) {
            throw(new RouteException("library tag is not set"));
        }
        this.route.library = target.item(0).getTextContent();
        if(this.route.library.length() == 0) {
            throw(new RouteException("library tag is empty"));
        }
    }

    private void handleProgram(NodeList target) throws RouteException {
        if(target.getLength() == 0) {
            throw(new RouteException("program tag is not set"));
        }
        this.route.program = target.item(0).getTextContent();
        if(this.route.program.length() == 0) {
            throw(new RouteException("program tag is empty"));
        }
    }

    private void handleDebug(NodeList target) {
        String bool_str = null;

        if(target.getLength() == 0) {
            this.route.debug = false;
            return;
        }

        bool_str = target.item(0).getTextContent();
        if(bool_str.equals("true")) {
            this.route.debug = true;
        }
    }

    private void handleDeleteFile(NodeList target) {
        String bool_str = null;

        if(target.getLength() == 0) {
            this.route.deletefile = false;
            return;
        }

        bool_str = target.item(0).getTextContent();
        if(bool_str.equals("true")) {
            this.route.deletefile = true;
        }
    }

    private void handleLogin(NodeList target) {
        String bool_str = null;

        if(target.getLength() == 0) {
            this.route.login = false;
            return;
        }

        bool_str = target.item(0).getTextContent();
        if(bool_str.equals("true")) {
            this.route.login = true;
        }
    }

    private void handleLogging(NodeList target) {
        if(target.getLength() == 0) {
            this.route.loglevel = "ERROR";
            return;
        }
        this.route.loglevel = target.item(0).getTextContent();
    }

    public Route getRoute() {
        return(this.route);
    }
}
