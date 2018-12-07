package realHTML.tomcat.environment;

import java.util.ArrayList;

import realHTML.tomcat.routing.Routing;

public class Environment {
	public String natparms;
	public String natsourcepath;
    public String charEncoding;
	public ArrayList<EnvironmentVar> environvars;
	public Routing routing;
	
	public Environment(String natparms, String natsrc, String charencoding) {
		this.natparms = natparms;
		this.natsourcepath = natsrc;
        this.charEncoding = charencoding;
		this.routing = new Routing();
		this.environvars = new ArrayList<EnvironmentVar>();
	}
	
	public EnvironmentVar getEnvironmentVar(String name) {
		for(int i = 0; i < this.environvars.size(); i++) {
			if(this.environvars.get(i).name.equals(name)) {
				return(this.environvars.get(i));
			}
		}
		return(null);
	}
	
	public void addEnvironmentVar(String name, String value, boolean append) {
		this.environvars.add(new EnvironmentVar(name, value, append));
	}
	
	public void deleteEnvironmentVar(String name) {
		for(int i = 0; i < this.environvars.size(); i++) {
			if(this.environvars.get(i).name.equals(name)) {
				this.environvars.remove(i);
				break;
			}
		}
	}
	
	public EnvironmentVar[] getEnvirons() {
		return(this.environvars.toArray(new EnvironmentVar[this.environvars.size()]));
	}
}
