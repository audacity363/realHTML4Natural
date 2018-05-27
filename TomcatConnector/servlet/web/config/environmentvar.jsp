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
	String envname = request.getParameter("envname");
	String environname = request.getParameter("environname");
	
	Environment environment = environments.getEnvironment(envname);
	EnvironmentVar environvar  = environment.getEnvironmentVar(environname);
%>
<label for="name">Name:</label><span id="name"><% out.print(environvar.name); %></span><br>
<label for="value">Value:</label><input type="text" id="value" value=<%= environvar.value %>><br>
<label for="append">Append:</label><input id="append" type="checkbox" <% if(environvar.append) { %> checked <%} %>>

<form action="environmentvar" method="post">
	<input value=<%= envname %> name="_envname" type="hidden">
	<input value=<%= environvar.name %> name="varname" type="hidden">
	<input value="delete" name="_method" type="hidden">
	<button type="submit">Delete</button>
</form>
</body>
</html>