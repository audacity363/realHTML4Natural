package realHTML.tomcat.gui;

import java.io.IOException;
import java.util.ArrayList;

import javax.servlet.jsp.JspWriter;

import realHTML.tomcat.routing.PathEntry;
import realHTML.tomcat.routing.PathType;
import realHTML.tomcat.routing.Route;

public class RouteTree {
	public ArrayList<RouteTree> childs;
	public String path;
	public int id;
	public PathEntry pathentry;
	public Route route;
	
	public RouteTree() {
		this.childs = new ArrayList<RouteTree>();
		this.id = -1;
		this.path = "/";
	}
	
	public void addChild(String path) {
		RouteTree tmp = new RouteTree();
		tmp.path = path;
		tmp.id = -1;
		
		this.childs.add(tmp);
	}
	
	public void addChild(String path, int id, PathEntry entry, Route route) {
		RouteTree tmp = new RouteTree();
		tmp.path = path;
		tmp.id = id;
		tmp.pathentry = entry;
		tmp.route = route;
		
		this.childs.add(tmp);
	}
	
	public Boolean checkPath(String path) {
		for(int i = 0; i < this.childs.size(); i++) {
			if(this.childs.get(i).path.equals(path)) {
				return(true);
			}
		}
		return(false);
	}
	
	public RouteTree getChildPath(String path) {
		for(int i = 0; i < this.childs.size(); i++) {
			if(this.childs.get(i).path.equals(path)) {
				return(this.childs.get(i));
			}
		}
		return(null);
	}
	
	public void printHTML(JspWriter out, String environmentname, int level) throws IOException {
		if(level == 0) { out.print("<ul class=\"tree\">"); }
		//else if(level > 1) { out.print("<ul style=\"display: none\""); }
		else { out.print("<ul>"); }
		if(level > 0) { out.print("<span class=\"arrow\">&#10148;</span>"); }
		if(this.id != -1) { out.print("<a href=\"route.jsp?id=" + this.id +"&name=" + environmentname + "\">"); }
		else { out.print("<a>"); }
		if(level > 1) { out.print("/"); }
		if(this.pathentry != null) {
			if(this.pathentry.getType() == PathType.VARIABLE || this.pathentry.getType() == PathType.SELECTION) { 
				out.print("&lt;"); 
			}
		}
		out.print(this.path);
		if(this.pathentry != null) {
			
			if(this.pathentry.getType() == PathType.SELECTION) {
				out.print("=");
				String[] options = this.pathentry.getOptions();
				for(int i = 0; i < options.length; i++) {
					out.print(options[i]);
					if(i+1 < options.length) {
						out.print("|");
					}
				}
				out.print("&gt;");
			}
			
			if(this.pathentry.getType() == PathType.VARIABLE) { out.print("&gt;"); }
		}
		out.print("</a>");
		if(this.route != null) {
			out.print("<span>&rarr; (" + this.route.natLibrary +"/" + this.route.natProgram + ")</span>");
		}
		for(int i = 0; i < this.childs.size(); i++) {
			if(level > 0) { out.print("<li style=\"display: none\">"); }
			else { out.print("<li>"); }
			this.childs.get(i).printHTML(out, environmentname, level+1);
			out.print("</li>");
		}
		out.print("</ul>");
	}
	
	public void print(int level) {
		this.printtabs(level);
		System.out.println("Name: " + this.path);
		if(this.id != -1) {
			this.printtabs(level);
			System.out.println("ID: " + this.id);
		}
		for(int i = 0; i < this.childs.size(); i++) {
			this.childs.get(i).print(level+1);
		}
	}
	
	private void printtabs(int level) {
		for(int i = 0; i < level; i++) { System.out.print("\t"); }
	}
}
