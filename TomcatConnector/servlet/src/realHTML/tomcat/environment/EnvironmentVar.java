package realHTML.tomcat.environment;

public class EnvironmentVar {
	public String name;
    public String value;
    public boolean append;

    public EnvironmentVar() {
        this.append = false;
    }

    public EnvironmentVar(String name, String value, boolean append) {
        this.name = name;
        this.value = value;
        this.append = append;
}
}
