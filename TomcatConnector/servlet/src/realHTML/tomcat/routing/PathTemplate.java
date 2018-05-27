package realHTML.tomcat.routing;

import java.util.ArrayList;
import java.util.HashMap;

public class PathTemplate {
	public String template;
	public Route route;
	PathEntry[] entries;
	
	public PathTemplate(String template, Route route) {
		this.template = template;
		this.route = route;
	}
	
	public void parseTemplate() {
		ArrayList<String> entries;
		
		entries = splitTemplate();
		this.entries = convertEntries(entries);
		
		for(int i = 0; i < this.entries.length; i++) {
			System.out.println(this.entries[i]);
		}
	}
	
	
	public Boolean matchPath(String url) {
		String[] urlentries_complete = url.split("/"),
				 urlentries;
		if(!this.route.active) {
			return(false);
		}
		
		urlentries = new String[urlentries_complete.length-1];
		System.arraycopy(urlentries_complete, 1, urlentries, 0, urlentries_complete.length-1);
//		System.out.println("----------------------------------");
//		for(int i = 0; i < urlentries.length; i++) {
//			System.out.println(urlentries[i]);
//		}
//		System.out.println("----------------------------------");
		
		if (urlentries.length != this.entries.length) {
			return(false);
		}
		
		for(int i = 0; i < this.entries.length; i++) {
			switch(this.entries[i].type) {
				case STATIC:
					if(!this.entries[i].getName().equals(urlentries[i])) { return(false); }
					break;
				case VARIABLE:
					this.entries[i].setValue(urlentries[i]);
					break;
				case SELECTION:
					if(!this.entries[i].containsOption(urlentries[i])) { return(false); }
					this.entries[i].setValue(urlentries[i]);
					break;
			}
//			if(!this.entries[i].isVariable() && 
//				!this.entries[i].getName().equals(urlentries[i])) {
//				return(false);
//			} else if(this.entries[i].isVariable()) {
//				this.entries[i].setValue(urlentries[i]);
//			}
		}
		
		
		return(true);
	}
	
	public HashMap<String, String> getParms() {
		HashMap<String, String> parms = new HashMap<String, String>();
		
		for(int i = 0; i < this.entries.length; i++) {
			if(this.entries[i].type == PathType.STATIC) { continue; }
			parms.put(this.entries[i].name, this.entries[i].value);
		}
		
		return(parms);
	}
	
	private PathEntry[] convertEntries(ArrayList<String> entries) {
		PathEntry[] peentries = new PathEntry[entries.size()];
		int i = 0;
		for(String entry: entries) {
			peentries[i++] = parseParmString(entry);
//			if(entry.charAt(0) == ':') {
//				peentries[i++] = new PathEntry(PathType.VARIABLE, entry.substring(1));
//			} else {
//				peentries[i++] = new PathEntry(PathType.STATIC, entry);
//			}
		}
		return(peentries);
	}
	
	private PathEntry parseParmString(String strEntry) {
		ArrayList<String> options = new ArrayList<String>(); 
		String name = "";
		String items[] = null;
		
		if (strEntry.charAt(0) != ':') {
			return(new PathEntry(PathType.STATIC, strEntry));
		}
		
		if (!strEntry.contains("=")) {
			return(new PathEntry(PathType.VARIABLE, strEntry.substring(1)));
		}
		
		strEntry = strEntry.substring(1);
		
		items = strEntry.split("=", 2);
		if (items[1].length() == 0) {
			//TODO: Throw Exception
		}
		
		if(!items[1].contains("|")) {
			options.add(items[1]);
			return(new PathEntry(PathType.SELECTION, items[0], 
					options.toArray(new String[options.size()])));
		}
		
		name = items[0];
		items = items[1].split("\\|");
		
		return(new PathEntry(PathType.SELECTION, name, items));
	}
	
	
	
	private ArrayList<String> splitTemplate() {
		int length = this.template.length();
		String entry = "";
		char target = ' ';
		ArrayList<String> entries = new ArrayList<String>();
		
		
		for(int i=0; i < length; i++) {
			target = this.template.charAt(i);
			if(target == '\\') {
				if(i+1 < length && this.template.charAt(i+1) == '/') {
					entry += "/";
					i++;
				} else {
					entry += '\\';
				}
			} else if(target == '/') {
				if(entry.length() != 0) {
					entries.add(entry);
				}
				entry = "";
			} else {
				entry += target;
			}
		}
		entries.add(entry);
		
		return(entries);
	}
	
	public String toString() {
		String ret = "PathTemplate[template=(" + this.template +"), parms="; //"]";
		for(int i = 0; i < this.entries.length; i++) {
			if(this.entries[i].type == PathType.STATIC) { continue; }
			ret += "(" + this.entries[i].name + "=" + this.entries[i].value + ")";
			if(i+1 < this.entries.length) {
				ret += ",";
			}
		}
		ret += "]";
		
		return(ret);
		
	}
}
