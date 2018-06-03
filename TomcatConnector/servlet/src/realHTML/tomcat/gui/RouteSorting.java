package realHTML.tomcat.gui;


import realHTML.tomcat.routing.PathTemplate;
import realHTML.tomcat.routing.Routing;

public class RouteSorting {

	public static RouteTree sortRoutes(Routing routing) {
		PathTemplate tmp;
		RouteTree treeentry, root;
		String targetpath;
		
		root = new RouteTree();
		
		for(int i=0; i < routing.templates.size(); i++) {
			tmp = routing.templates.get(i);
			treeentry = root;
			for(int x = 0; x < tmp.entries.length; x++) {
				targetpath = tmp.entries[x].getName();
				if(!treeentry.checkPath(targetpath)) {
					if(x == tmp.entries.length-1) {
						treeentry.addChild(targetpath, i, tmp.entries[x], tmp.route);
					} else {
						treeentry.addChild(targetpath);
					}
					treeentry = treeentry.getChildPath(targetpath);
				} else {
					treeentry = treeentry.getChildPath(targetpath);
				}
			}
		}
		
		return(root);
	}
}
