package realHTML.tomcat.routing;

import java.util.ArrayList;

public class Routing {
	public ArrayList<PathTemplate> templates;
	
	public Routing() {
		this.templates = new ArrayList<PathTemplate>();
	}
	
	public void addRoute(String template, Route route) {
		PathTemplate newroute = new PathTemplate(template, route);
		newroute.parseTemplate();
		this.templates.add(newroute);
	}
	
	public PathTemplate getRoute(String url) {
		for(PathTemplate entry: this.templates) {
			if(entry.matchPath(url)) {
				return(entry);
			}
		}
		return(null);
	}
	
	public String[] getRoutesTemplates() {
		String[] routes = new String[this.templates.size()];
		
		for(int i = 0; i < this.templates.size(); i++) {
			routes[i] = this.templates.get(i).template;
		}
		
		return(routes);
	}
	
	public Route getRouteById(int id) {
		if(id > this.templates.size()) {
			return(null);
		}
		
		return(this.templates.get(id).route);
	}
	
	public void deleteRoute(int id) {
		if(id > this.templates.size()) {
			return;
		}
		this.templates.remove(id);
	}
	
	public PathTemplate[] getRoutes() {
		return(this.templates.toArray(new PathTemplate[this.templates.size()]));
	}
}
