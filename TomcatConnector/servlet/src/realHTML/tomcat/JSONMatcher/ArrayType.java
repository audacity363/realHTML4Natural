package realHTML.tomcat.JSONMatcher;

public enum ArrayType {
	ARRAYEMPTY(10), DIMNOTSUPPORTED(11), 
	ONEDIM(1), TWODIM(2), THREEDIM(3);
	
	Integer dims;
	
	ArrayType(Integer number) {
		this.dims = number;
	}
	
	public Integer getDimension() {
		return(this.dims);
	}
}
