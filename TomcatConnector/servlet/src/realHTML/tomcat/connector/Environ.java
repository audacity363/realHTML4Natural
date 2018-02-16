package realHTML.tomcat.connector;

public class Environ {
    public String name;
    public String value;
    public boolean append;

    public Environ() {
        this.append = false;
    }

    public Environ(String name, String value, boolean append) {
        this.name = name;
        this.value = value;
        this.append = append;
    }
}
