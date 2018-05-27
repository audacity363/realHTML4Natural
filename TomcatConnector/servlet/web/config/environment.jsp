<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="javax.servlet.ServletContext" %>    
<%@ page import="realHTML.tomcat.routing.*" %>
<%@ page import="realHTML.tomcat.environment.*" %>    
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
	
	Environment environment = environments.getEnvironment(envname);
	String[] routes = environment.routing.getRoutesTemplates();
	EnvironmentVar[] environs = environment.getEnvirons();
%>
<h1>Environment: <% out.print(envname); %></h1>
<form method="post" action="environment">
	<label for="natparm">Natural Parms:</label>
	<input type="text" name="natparm" value=<%= environment.natparms %>>
	<br>
	<label for="natsrc">Natural RSC PATH:</label>
	<input type="text" name="natsrc" value=<%= environment.natsourcepath %>>
	<br>
	<input value=<%= envname %> name="envname" type="hidden">
	<input value="post" name="_method" type="hidden">
	<button type="submit">Save</button>
</form>
<form method="post" action="environment">
	<input value=<%= envname %> name="envname" type="hidden">
	<input value="delete" name="_method" type="hidden">
	<button type="submit">Delete</button>
</form>
<h2>Routes</h2>
<ul>
<%
	for(int i = 0; i < routes.length; i++) {
		out.print("<li><a href='route.jsp?id=" + i + "&name=" + envname + "'>" + routes[i] + "</a></li>");
	}
%>
</ul>
	<form method="post" action="routes">
		<label for="routelink">Route Template:</label>
		<input type="text" name="routelink">
		<br>
		<label for="routelink">Natural Library:</label>
		<input type="text" name="library">
		<br>
		<label for="routelink">Natural Program:</label>
		<input type="text" name="program">
		<br>
		<label for="login">Login:</label>
		<input type="checkbox" name="login">
		<br>
		<label for="login">Active:</label>
		<input type="checkbox" name="active" checked>
		<br>
		<label for="loglevel">Loglevel:</label>
		<select name="loglevel">
			<option value="DEVELOP">Develop</option>
			<option value="DEBUG">Debug</option>
			<option value="INFO">Info</option>
			<option value="WARN">Warning</option>
			<option value="ERROR" selected>Error</option>
			<option value="FATAL">Fatal</option>
		</select> 
		<br>
		<input value=<%= envname %> name="_envname" type="hidden">
		<input value="put" name="_method" type="hidden">
		<button type="submit">Add Route</button>
	</form>
<hr>
<h2>Environment Variablen</h2>
<ul>
<%
	for(int i = 0; i < environs.length; i++) {
		out.print("<li><a href='environmentvar.jsp?envname=" + envname + "&environname=" + environs[i].name + "'>" + environs[i].name + "</a></li>");
	}
%>
</ul>
<form method="post" action="environmentvar">
		<label for="varname">Variablename:</label>
		<input type="text" name="varname">
		<br>
		<label for="content">Content:</label>
		<input type="text" name="content">
		<br>
		<label for="append">Append?:</label>
		<input type="checkbox" name="append">
		<br>
		<input value=<%= envname %> name="_envname" type="hidden">
		<input value="put" name="_method" type="hidden">
		<button type="submit">Add Environ</button>
	</form>
<hr>
</body>
</html>