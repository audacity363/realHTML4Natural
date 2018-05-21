package realHTML.tomcat.connector;

import java.io.*;
import java.util.*;
import java.io.File;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

import realHTML.tomcat.connector.Environ;

public class ConfigurationLoader
{
    String filepath = null;
    String env = null;
    Map<String, String> results = new Map();
    String tags[] = {};

    Environ envvars[] = null;

    public ConfigurationLoader(String filepath, String searchtags[])
    {
        this.filepath = filepath;
        this.tags = searchtags;
    }

    public Boolean readConfiguration(String env) throws ParserConfigurationException, SAXException, IOException
    {
        DocumentBuilderFactory dbFactory; 
        DocumentBuilder dBuilder;
        Document doc;
        Element env_element;

        File inputFile = new File(this.filepath);

        dbFactory = DocumentBuilderFactory.newInstance();
        dBuilder = dbFactory.newDocumentBuilder();

        doc = dBuilder.parse(inputFile);
        doc.getDocumentElement().normalize();

        env_element = findEnv(env, doc);
        if(env_element == null)
        {
            return(false);
        }
        readSettings(env_element);
        readEnviromentVars(env_element);
        return(true);
        
    }

    private Element findEnv(String env, Document doc)
    {
        Element eElement;
        NodeList nList;
        Node nNode;

        nList = doc.getElementsByTagName("environment");
        for(int i=0; i < nList.getLength(); i++)
        {
            nNode = nList.item(i);
            eElement = (Element)nNode;

            if(eElement.getAttribute("type").equals(env))
            {
                return(eElement);
            }
        }
        return(null);
    }

    private void readSettings(Element target)
    {
        NodeList result;

        for(int i=0; i < this.tags.length; i++)
        {
            result = target.getElementsByTagName(this.tags[i]);
            if(result.getLength() == 0)
            {
                continue;
            }
            this.results.put(this.tags[i], result.item(0).getTextContent());
        }
    }

    private void readEnviromentVars(Element target) {
        NodeList envvars, tmp;
        Node environ_node;
        Element environ_entry;

        envvars = target.getElementsByTagName("environ");
        
        if(envvars.getLength() == 0) {
            return;
        }

        this.envvars = new Environ[envvars.getLength()];

        for(int i=0; i < envvars.getLength(); i++) {
            environ_node = envvars.item(i);
            environ_entry = (Element)environ_node;
            this.envvars[i] = new Environ();
          
            //Get the name of the enviroment variable
            tmp = environ_entry.getElementsByTagName("name");

            //When there are no or multiple entries the enviroment config is invalid
            //and gets ignored
            if(tmp.getLength() != 1) {
                continue;
            }

            this.envvars[i].name = tmp.item(0).getTextContent();

            //Get the value for the enviroment variable
            tmp = environ_entry.getElementsByTagName("value");
            if(tmp.getLength() != 1) {
                continue;
            }
            this.envvars[i].value = tmp.item(0).getTextContent();

            //Check if the variable should get appended
            tmp = environ_entry.getElementsByTagName("append");
            if(tmp.getLength() == 1) {
                if(tmp.item(0).getTextContent().equals("true")) {
                    this.envvars[i].append = true;
                } else {
                    this.envvars[i].append = false;
                }

            }
            else {
                this.envvars[i].append = false;
            }
            
        }
    }

    public Map<String, String> getResult() 
    {
        return(this.results);
    }

    public Environ[] getEnviromentVars() 
    {
        if(this.envvars != null) {
            return(this.envvars);
        }
        Environ[] nothing = {};
        return(nothing);

    }
}
