import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import realHTML.servlet.exceptions.EnvironmentException;
import realHTML.tomcat.environment.EnvironmentBuffer;
import realHTML.tomcat.xml.Export;

public class RealHTMLSaveConfig extends RealHTMLInit {
	private static final long serialVersionUID = 1L;
       
    public RealHTMLSaveConfig() {
        super();
    }
    
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		EnvironmentBuffer envs = EnvironmentBuffer.getEnvironmentsfromContext(getServletContext());
		if(envs == null) {
			throw(new ServletException(new EnvironmentException("No enviroment defined")));
		}
		
		try {
			Export export = new Export(envs);
			export.exportToFile(this.configurationfile);
		} catch(Exception e) {
			throw(new ServletException(e));
		}
		
		getServletContext().setAttribute("message", "Successfully saved configuration to " + this.configurationfile);
		getServletContext().removeAttribute("settingssaved");
		if(request.getHeader("referer") != null) {
			response.sendRedirect(request.getHeader("referer"));
		} else {
			response.sendRedirect("index.jsp");
		}
	}
}
