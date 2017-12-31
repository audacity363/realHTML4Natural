package realHTML.servlet.exceptions;

public class RouteException extends Exception {

    private static final long serialVersionUID = -4396169401893956613L;

    public RouteException() {}

    public RouteException(String message) {
        super(message);
    }
}