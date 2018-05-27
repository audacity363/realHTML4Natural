import java.io.FileNotFoundException;
import java.io.IOException;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import realHTML.servlet.exceptions.EnvironmentException;
import realHTML.servlet.exceptions.XMLException;
import realHTML.tomcat.environment.EnvironmentBuffer;
import realHTML.tomcat.xml.Import;
import realHTML.tomcat.connector.JNILoader;

public class RealHTMLInit extends HttpServlet {

	private static final long serialVersionUID = -6815603038617744076L;
	private final String version = "2.0";
	public String configurationfile;
    public String logpath;
    public JNILoader bs;
	
	public void init(ServletConfig config) throws ServletException {
		super.init(config);
		
		this.configurationfile = System.getenv("RH4NCONF");
		if(this.configurationfile == null) {
			throw(new ServletException("Enviroment variable RH4NCONF is missing"));
		}
		
		this.logpath = System.getenv("RH4NLOG");
		if(this.logpath == null) {
			throw(new ServletException("Enviroment variable RH4NLOG is missing"));
		}
		
		if(getServletContext().getAttribute("isinitialised") != null) {
			return;
		}
		System.out.print("Starting realHTML4Natural Tomcat Connector");
		System.out.println(" Version: " + this.version);
		
		if(EnvironmentBuffer.getEnvironmentsfromContext(getServletContext()) == null) {
			Import xmlimport = new Import();
			EnvironmentBuffer envs;
			
			try {
				envs = xmlimport.importFromFile(this.configurationfile);
			} catch(FileNotFoundException e) {
				System.out.println("Warning: File " + configurationfile + " does not exist.");
				System.out.println("Warning: There will be no environments definied.");
				envs = new EnvironmentBuffer();
			} catch (Exception e) {
				throw(new ServletException(e));
			}
			
			EnvironmentBuffer.setEnvironmentsforContext(getServletContext(), envs);
		}
    
        this.bs = new JNILoader();
        this.bs.printVersion();
		
		getServletContext().setAttribute("isinitialised", true);
		
		System.out.println("Successfully initialised realHTML4Natural Tomcat Connector");
	}
}
