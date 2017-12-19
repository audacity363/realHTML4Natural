package realHTML.tomcat.JSONMatcher;

public class TypeException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4396169401899956613L;

	// Parameterless Constructor
    public TypeException() {}

    // Constructor that accepts a message
    public TypeException(String message)
    {
       super(message);
    }
}
