import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class RealHTMLLogout extends HttpServlet {

    public void doPost(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException {
        HttpSession session = null;
        PrintWriter output = null;
        String user = null;

        try {
            output = response.getWriter();

            session = request.getSession();
            if(session.isNew()) {
                output.print("{\"success\":false,\"msg\":\"Client is not authenticated\"}");
                output.flush();
                session.invalidate();
                return;
            }
            
            user = (String)session.getAttribute("username");
            if(user == null) {
                output.print("{\"success\":false,\"msg\":\"Username not found in session\"}");
                output.flush();
                session.invalidate();
                return;
            }

            session.invalidate();
            output.print("{\"success\":true,\"msg\":\"User "+user+" was logged out\"}");
            output.flush();
            return;

        } catch(Exception e) {
            throw(new ServletException("", e));
        }
    }
}
