<%@page import="realHTML.tomcat.environment.EnvironmentBuffer"%>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="realHTML.tomcat.routing.*" %>
<%@ page import="java.lang.Integer" %>
<%@ page import="realHTML.tomcat.environment.*" %>
<%@ page import="javax.servlet.ServletContext" %>    
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<jsp:include page="messagebar.jsp"></jsp:include>
	<%
		EnvironmentBuffer environments = EnvironmentBuffer.getEnvironmentsfromContext(application);
		String envname = request.getParameter("name");
		String routeid = request.getParameter("id");
		Environment environment = environments.getEnvironment(envname);
		Route route = environment.routing.getRouteById(Integer.parseInt(routeid));
		System.out.println(route.active);
	%>
	<form method="post" action="routes">
		<label for="library">Natural Library:</label>
		<input type="text" name="library" value=<%= route.natLibrary %>>
		<br>
		<label for="program">Natural Program:</label>
		<input type="text" name="program" value=<%= route.natProgram %>>
		<br>
		<label for="login">Login:</label>
		<input type="checkbox" name="login" <%if(route.login){%>checked<%}%>>
		<br>
		<label for="active">Active:</label>
		<input type="checkbox" name="active" <%if(route.active){%>checked<%}%>>
		<br>
		<label for="loglevel">Loglevel:</label>
		<select name="loglevel">
			<option value="DEVELOP" <%if(route.loglevel.equals("DEVELOP")) {%>selected<%}%>>Develop</option>
			<option value="DEBUG" <%if(route.loglevel.equals("DEBUG")) {%>selected<%}%>>Debug</option>
			<option value="INFO" <%if(route.loglevel.equals("INFO")) {%>selected<%}%>>Info</option>
			<option value="WARN" <%if(route.loglevel.equals("WARN")) {%>selected<%}%>>Warning</option>
			<option value="ERROR" <%if(route.loglevel.equals("ERROR")) {%>selected<%}%>>Error</option>
			<option value="FATAL" <%if(route.loglevel.equals("FATAL")) {%>selected<%}%>>Fatal</option>
		</select> 
		<br>
		<input value=<%= envname %> name="_envname" type="hidden">
		<input value=<%= routeid %> name="_routeid" type="hidden">
		<input value="post" name="_method" type="hidden">
		<button type="submit">Save</button>
	</form>
	<form method="post" action="routes">
		<input value=<%= envname %> name="_envname" type="hidden">
		<input value=<%= routeid %> name="_routeid" type="hidden">
		<input value="delete" name="_method" type="hidden">
		<button type="submit">Delete</button>
	</form>
</body>
</html>