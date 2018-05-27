

import java.io.FileNotFoundException;
import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import realHTML.servlet.exceptions.EnvironmentException;
import realHTML.servlet.exceptions.XMLException;
import realHTML.tomcat.environment.EnvironmentBuffer;
import realHTML.tomcat.xml.Import;

public class RealHTMLLoadConfig extends RealHTMLInit {
	private static final long serialVersionUID = 1L;
       
    public RealHTMLLoadConfig() {
        super();
    }

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		Import xmlimport = new Import();
		
			EnvironmentBuffer envs;
			try {
				envs = xmlimport.importFromFile(this.configurationfile);
			} catch(FileNotFoundException e) {
				envs = new EnvironmentBuffer();
			} catch (Exception e) {
				throw(new ServletException(e));
			}
			EnvironmentBuffer.setEnvironmentsforContext(getServletContext(), envs);
		
		getServletContext().setAttribute("message", "Successfully loaded configuration from " + this.configurationfile);
		getServletContext().removeAttribute("settingssaved");
		response.sendRedirect("index.jsp");
	}

}
