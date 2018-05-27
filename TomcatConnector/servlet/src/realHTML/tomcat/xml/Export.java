package realHTML.tomcat.xml;

import java.io.File;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import realHTML.servlet.exceptions.EnvironmentException;
import realHTML.tomcat.environment.Environment;
import realHTML.tomcat.environment.EnvironmentBuffer;
import realHTML.tomcat.environment.EnvironmentVar;
import realHTML.tomcat.routing.PathTemplate;
import realHTML.tomcat.routing.Routing;

public class Export {
	
	EnvironmentBuffer envs;
	
	public Export(EnvironmentBuffer envs) {
		this.envs = envs;
	}
	
	public void exportToFile(String path) throws ParserConfigurationException, TransformerException, EnvironmentException {
		DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder docBuilder = docFactory.newDocumentBuilder();

		Document doc = docBuilder.newDocument();
		Element rootElement = doc.createElement("realHTML4Natural");
		doc.appendChild(rootElement);
		
		String[] envkeys = this.envs.getEnviromentNames();
		for(int i = 0; i < envkeys.length; i++) {
			rootElement.appendChild(this.envtoString(doc, envkeys[i]));
		}
		
		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer = transformerFactory.newTransformer();
		DOMSource source = new DOMSource(doc);
		StreamResult result = new StreamResult(new File(path));
		
		transformer.transform(source, result);
	}
	
	private Element envtoString(Document doc, String envname) throws EnvironmentException {
		Element envElement, natsrcElement, natparmElement; 
		Environment env = this.envs.getEnvironment(envname);
		
		envElement = doc.createElement("environment");
		envElement.setAttribute("name", envname);
		
		natsrcElement = doc.createElement("natsrc");
		natsrcElement.setTextContent(env.natsourcepath);
		
		natparmElement = doc.createElement("natparms");
		natparmElement.setTextContent(env.natparms);
		
		envElement.appendChild(natsrcElement);
		envElement.appendChild(natparmElement);
		
		envElement.appendChild(this.routestoString(doc, env.routing));
		envElement.appendChild(this.environtoString(doc, env.environvars));
		
		return(envElement);
	}	
	
	private Element environtoString(Document doc, ArrayList<EnvironmentVar> environs) {
		Element environsElement, environElement, nameElement, contentElement, appendElement;
		EnvironmentVar envvar;
		
		environsElement = doc.createElement("environs");
		
		for(int i = 0; i < environs.size(); i++) {
			envvar = environs.get(i);
			environElement = doc.createElement("environ");
			
			nameElement = doc.createElement("name");
			nameElement.setTextContent(envvar.name);
			
			contentElement = doc.createElement("content");
			contentElement.setTextContent(envvar.value);
			
			appendElement = doc.createElement("append");
			if(envvar.append) {
				appendElement.setTextContent("true");
			} else {
				appendElement.setTextContent("false");
			}
			
			environElement.appendChild(nameElement);
			environElement.appendChild(contentElement);
			environElement.appendChild(appendElement);
			
			environsElement.appendChild(environElement);
		}
		
		return(environsElement);
	}
	
	private Element routestoString(Document doc, Routing routing) {
		Element routesElement, routeElement, natlibrary, natprogram,
			loginElement, loglevelElement;
		
		PathTemplate[] routes = routing.getRoutes();
		
		routesElement = doc.createElement("routes");
		
		for(int i = 0; i < routes.length; i++) {
			routeElement = doc.createElement("route");
			routeElement.setAttribute("path", routes[i].template);
			
			natlibrary = doc.createElement("natLibrary");
			natlibrary.setTextContent(routes[i].route.natLibrary);
			
			natprogram = doc.createElement("natProgram");
			natprogram.setTextContent(routes[i].route.natProgram);
			
			loginElement = doc.createElement("login");
			if(routes[i].route.login) {
				loginElement.setTextContent("true");
			} else {
				loginElement.setTextContent("false");
			}
			
			loglevelElement = doc.createElement("loglevel");
			loglevelElement.setTextContent(routes[i].route.loglevel);
			
			routeElement.appendChild(natlibrary);
			routeElement.appendChild(natprogram);
			routeElement.appendChild(loginElement);
			routeElement.appendChild(loglevelElement);
			
			routesElement.appendChild(routeElement);
		}
		
		return(routesElement);
	}
}
