import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

import realHTML.tomcat.connector.*;

public class realHTMLLogin extends HttpServlet {
    private JNILoader bs;

    public void init() {
        this.bs = new JNILoader();
    }


    public void doPost(HttpServletRequest request, HttpServletResponse response) 
        throws ServletException {
            String user, passwd;
            PrintWriter output = null;
            int authOK = 0;
            HttpSession session = null;

            //System.out.println("Login: [" + request.getRequestURI() + "]");
            
            try {
                output = response.getWriter();

                session = request.getSession();
                if(!session.isNew()) {
                    output.print("{\"success\":false,\"msg\":\"Client is already authenticated\"}");
                    output.flush();
                    return;
                }


                user = request.getParameter("username");
                passwd = request.getParameter("password");

                if(user == null || passwd == null) {
                    output.print("{\"success\":false,\"msg\":\"Username or password missing\"}");
                    output.flush();
                    session.invalidate();
                    return;
                }

                authOK = this.bs.passwd(user, passwd);

                if(authOK == 1 || authOK == -4) {
                    output.print("{\"success\":false,\"msg\":\"Username or password wrong\"}");
                    output.flush();
                    session.invalidate();
                    return;
                } else if (authOK != 0) {
                    output.print("{\"success\":false,\"msg\":\"Internal Error\"}");
                    output.flush();
                    session.invalidate();
                    return;
                }

                session.setAttribute("username", user);

                output.print("{\"success\":true}");
                output.flush();
                return;
            } catch (Exception e) {
                throw(new ServletException("", e));
            }
     }
}
