package realHTML.tomcat.xml;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;


import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import realHTML.servlet.exceptions.EnvironmentException;
import realHTML.servlet.exceptions.XMLException;
import realHTML.tomcat.environment.EnvironmentBuffer;
import realHTML.tomcat.routing.Route;

public class Import {
	public EnvironmentBuffer importFromFile(String path) throws ParserConfigurationException, SAXException, IOException, XMLException, EnvironmentException, FileNotFoundException {
		EnvironmentBuffer envs = new EnvironmentBuffer();
		
		DocumentBuilderFactory dbFactory; 
        DocumentBuilder dBuilder;
        Document doc;

        File inputFile = new File(path);
        
        if(!inputFile.exists()) {
        	throw(new FileNotFoundException());
        }

        dbFactory = DocumentBuilderFactory.newInstance();
        dBuilder = dbFactory.newDocumentBuilder();

        doc = dBuilder.parse(inputFile);
        doc.getDocumentElement().normalize();
        
        envs = this.readEnvironments(doc, envs);
		
		return(envs);
	}
	
	private EnvironmentBuffer readEnvironments(Document doc, EnvironmentBuffer envs) throws XMLException, EnvironmentException {
		NodeList environmentsElements, tmpList;
		Element targetElement, tmpElement;
		
		String envname, natsrc = "", natparms = "", encoding = "";
		
		environmentsElements = doc.getElementsByTagName("environment");
		for(int i = 0; i < environmentsElements.getLength(); i++) {
			targetElement = (Element)environmentsElements.item(i);
			envname = targetElement.getAttribute("name");
			if(envname.length() == 0) {
				throw(new XMLException("environment name is empty or not existent. Environment No." + i));
			}
			
			tmpList = targetElement.getElementsByTagName("natsrc");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				natsrc = tmpElement.getTextContent();
			}
			
			tmpList = targetElement.getElementsByTagName("natparms");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				natparms = tmpElement.getTextContent();
			}

            tmpList = targetElement.getElementsByTagName("charencoding");
            if(tmpList.getLength() != 0) {
                tmpElement = (Element)tmpList.item(0);
                encoding = tmpElement.getTextContent();
            } else {
                encoding = "ISO-8859-1";
            }
			
			envs.addEnviroment(envname, natparms, natsrc, encoding);
			envs = this.readRoutes(targetElement, envname, envs);
			envs = this.readEnvirons(targetElement, envname, envs);
			envname = natsrc = natparms = "";
		}
		
		return(envs);
	}
	
	private EnvironmentBuffer readRoutes(Element env, String envname, EnvironmentBuffer envs) throws XMLException, EnvironmentException {
		NodeList routesList, tmpList;
		Element targetElement, tmpElement;
		
		String template, natLibrary = "", natProgram = "", login_s = "", 
				loglevel = "", active_s = "";
		Boolean login, active;
		Route route;
		
		
		routesList = env.getElementsByTagName("route");
		for(int i = 0; i < routesList.getLength(); i++) {
			targetElement = (Element)routesList.item(i);
			template = targetElement.getAttribute("path");
			if(template.length() == 0) {
				throw(new XMLException("route in environment " + envname + " is empty"));
			}
			
			tmpList = targetElement.getElementsByTagName("natLibrary");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				natLibrary = tmpElement.getTextContent();
			}
			
			tmpList = targetElement.getElementsByTagName("natProgram");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				natProgram = tmpElement.getTextContent();
			}
			
			tmpList = targetElement.getElementsByTagName("login");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				login_s = tmpElement.getTextContent();
				if(login_s.equals("true")) { login = true; }
				else { login = false; }
			} else {
				login = false;
			}
			
			tmpList = targetElement.getElementsByTagName("loglevel");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				loglevel = tmpElement.getTextContent();
			} else {
				loglevel = "ERROR";
			}
			
			tmpList = targetElement.getElementsByTagName("active");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				active_s = tmpElement.getTextContent();
				if(active_s.equals("true")) { active = true; }
				else { active = false; }
			} else {
				active = false;
			}
			
			route = new Route(natLibrary, natProgram, login, loglevel, active);
			envs.addRoutetoEnv(envname, template, route);
			
			template = natLibrary = natProgram = login_s = loglevel = active_s = "";
			login = false;
		}
		
		return(envs);
	}
	
	private EnvironmentBuffer readEnvirons(Element env, String envname, EnvironmentBuffer envs) throws XMLException, EnvironmentException {
		NodeList environsList, tmpList;
		Element targetElement, tmpElement;
		
		String name = "", content = "", append_s = "";
		boolean append = false;
		
		environsList = env.getElementsByTagName("environ");
		for(int i = 0; i < environsList.getLength(); i++) {
			targetElement = (Element)environsList.item(i);
			
			tmpList = targetElement.getElementsByTagName("name");
			if(tmpList.getLength() == 0) {
				throw(new XMLException("Environ name in environment " + envname + " is empty"));
			}
			tmpElement = (Element)tmpList.item(0);
			name = tmpElement.getTextContent();
			
			tmpList = targetElement.getElementsByTagName("content");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				content = tmpElement.getTextContent();
			}
			
			tmpList = targetElement.getElementsByTagName("append");
			if(tmpList.getLength() != 0) {
				tmpElement = (Element)tmpList.item(0);
				append_s = tmpElement.getTextContent();
				if(append_s.equals("true")) {
					append = true;
				} else {
					append = false;
				}
			}
			
			envs.addEnvirontoEnv(envname, name, content, append);
			
			name = content = append_s = "";
		}
		
		return(envs);
	}
}
