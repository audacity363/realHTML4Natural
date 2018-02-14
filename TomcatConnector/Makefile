#JAVACLASSPATH = ${CURDIR}/servlet/web/WEB-INF/lib/realHTMLconnector.jar:/srv/apache-tomcat-8.5.9/lib/servlet-api.jar
CLASSPATH = "/u/it/a140734/rh4n/realHTML4Natural/TomcatConnector/servlet/web/WEB-INF/lib/realHTMLconnector.jar:/opt/tomcat/tomcat-rh4n/lib/servlet-api.jar:./servlet/bin/:./servlet/web/WEB-INF/lib/minimal-json-0.9.4.jar:./servlet/web/WEB-INF/lib/commons-io-2.6.jar"


#INCLUDES = -I/usr/java/jdk1.8.0_111/include/ \
#		   -I/usr/java/jdk1.8.0_111/include/linux/ \
#		   -I./jniLibrary/

INCLUDES = -I/SAG/cjp/v16/include/ \
		   -I./jniLibrary/include/ \
		   -I../include/ \
		   -I../libs/varhandle_lib/src/ \
		   -I../var2names/include/

LIBS = -L../libs/varhandle_lib/ -lvars \
	   -L../libs/nat-lda-parser/ -lldaparser \
	   -L../var2names/ -lvar2name \
	   -L../libs/utils/ -lutils \
	   -lcrypt


#CC = gcc
CC = xlc

JSON_JAVA_PARSER =  ./servlet/src/realHTML/tomcat/JSONMatcher/JSONArrayException.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/JSONObjectException.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/TypeException.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/Types.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/ArrayType.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/JSONUtils.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/LLNode.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/LLHandler.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/JSONObject.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/JSONArray.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/PrintFuncs.java \
					./servlet/src/realHTML/tomcat/JSONMatcher/JSONParser.java


JAVA_UTILS_PACKAGE = ./servlet/src/realHTML/tomcat/connector/Environ.java \
					 ./servlet/src/realHTML/tomcat/connector/RH4NReturn.java \
					 ./servlet/src/realHTML/tomcat/connector/RH4NParams.java \
					 ./servlet/src/realHTML/tomcat/connector/JNINatural.java \
					 ./servlet/src/realHTML/tomcat/connector/JNILoader.java \
					 ./servlet/src/realHTML/tomcat/connector/Route.java \
					 ./servlet/src/realHTML/tomcat/connector/Router.java \
					 ./servlet/src/realHTML/tomcat/connector/ConfigurationLoader.java \
					 ./servlet/src/realHTML/tomcat/connector/RH4NCallParms.java 

JAVA_UTILS_EXCEPTIONS = ./servlet/src/realHTML/servlet/exceptions/RouteException.java

JNI_JSON_HANDLERS = json_main.o json_utils.o plain_handlers.o 1d_handlers.o \
					2d_handlers.o 3d_handlers.o handlers.o callNatural.o

JNI_MEMBER = main.o natural.o passwd.o

all:
	@make warfile

warfile: cleanwarfile servletclass
	@printf "Copy package\n"
	@#cp ./servlet/bin/realHTMLServlet.class ./servlet/web/WEB-INF/classes/
	@cp ./servlet/bin/realHTMLHandler.class ./servlet/web/WEB-INF/classes/
	@cp ./servlet/bin/realHTMLLogin.class ./servlet/web/WEB-INF/classes/
	@cp ./servlet/bin/realHTMLLogout.class ./servlet/web/WEB-INF/classes/
	@printf "Copy servlet class\n"
	@cp ./servlet/bin/realHTMLconnector.jar ./servlet/web/WEB-INF/lib/
	@printf "Creating war file\n"
	@cd ./servlet/web; jar cvf ../realHTML4Natural.war .

cleanwarfile:
	@printf "Cleaning warfile\n"
	@rm -f ./realHTML4Natural.war
	@printf "Cleaning skeleton\n"
	@rm -f ./servlet/web/WEB-INF/classes/*.class
	@rm -f ./servlet/web/WEB-INF/lib/realHTMLconnector.jar

servletclass: cleanservletclass package
	@printf "Compiling Servlet\n"
	@#javac -d ./servlet/bin -cp $(CLASSPATH) servlet/src/realHTMLServlet.java
	@javac -d ./servlet/bin -cp $(CLASSPATH) servlet/src/realHTMLHandler.java
	@javac -d ./servlet/bin -cp $(CLASSPATH) servlet/src/realHTMLLogin.java
	@javac -d ./servlet/bin -cp $(CLASSPATH) servlet/src/realHTMLLogout.java

cleanservletclass:
	@printf "Cleaning servlet class\n"
	@rm -f ./servlet/bin/*.class

package: cleanpackage
	@printf "Compiling realHTML.tomcat.JSONMatcher\n"
	@javac -d ./servlet/bin -cp $(CLASSPATH) $(JSON_JAVA_PARSER)
	@printf "Compiling realHTML.tomcat.connector\n"
	@rm -rf ./bin/realHTML/tomcat/connector/*.class
	@javac -d ./servlet/bin -cp $(CLASSPATH) $(JAVA_UTILS_PACKAGE)
	@printf "Compiling realHTML.servlet.exceptions\n"
	@rm -rf ./bin/realHTML/servlet/exceptions/*.class
	@javac -d ./servlet/bin -cp $(CLASSPATH) $(JAVA_UTILS_EXCEPTIONS)
	@printf "Creating realHTMLconnector.jar\n"
	@cd ./servlet/bin && jar cf ./realHTMLconnector.jar ./realHTML

cleanpackage:
	@printf "Cleaning realHTML.tomcat.JSONMatcher\n"
	@rm -rf ./servlet/bin/realHTMLtomcat/JSONMatcher/*.class
	@printf "Cleaning realHTML.tomcat.connector\n"
	@rm -rf ./servlet/bin/realHTMLtomcat/connector/*.class
	@rm -f ./servlet/bin/realHTMLconnector.jar


jnilib: jniclean jniheader $(JNI_MEMBER) $(JNI_JSON_HANDLERS)
	@printf "Linking librealHTMLconnector.so\n"
	@$(CC) -G -o librealHTMLconnector.so $(LIBS) ./jniLibrary/bin/*.o

jniheader: package
	@printf "Generating JNI header\n"
	@cd ./servlet/bin; javah -jni -o ../../jniLibrary/include/realHTML_tomcat_connector_JNINatural.h \
			realHTML.tomcat.connector.JNINatural

$(JNI_MEMBER):
	@printf "CC ./jniLibrary/src/$*.c => ./jniLibrary/bin/$*.o\n"
	@$(CC) -g -c -fpic $(INCLUDES) -o ./jniLibrary/bin/$*.o ./jniLibrary/src/$*.c

$(JNI_JSON_HANDLERS):
	@printf "CC ./jniLibrary/src/json/$*.c => ./jniLibrary/bin/$*.o\n"
	@$(CC) -g -c -fpic $(INCLUDES) -o ./jniLibrary/bin/$*.o ./jniLibrary/src/json/$*.c

jniclean:
	@printf "Cleaning JNI objects\n"
	@rm -f ./jniLibrary/bin/*.o
	@printf "Cleaning shared object"
	@rm -f ./librealHTMLconnector.so

lltest:
	@printf "Cleaning LLTest object\n"
	@rm -f ./servlet/bin/LLTest.class
	@printf "Compiling LLTest.java\n"
	@javac -cp $(CLASSPATH) -d ./servlet/bin/ ./tests/LLTest.java
	@make runlltest

runlltest:
	@printf "Run LLTest\n"
	@export NATUSER=/u/it/a140734/rh4n/realHTML4Natural/bin/librealHTML4Natural.so && \
	java -cp "./servlet/bin:./servlet/web/WEB-INF/lib/minimal-json-0.9.4.jar" -Djava.library.path="/u/it/a140734/rh4n/realHTML4Natural/TomcatConnector/" LLTest


	
test:
	javac -cp ./servlet/src/ ./tests/CallNatural.java 
	java -cp "./servlet/src/:./tests/" -Djava.library.path="/u/it/a140734/rh4n/realHTML4Natural/TomcatConnector/" CallNatural

clean:
	rm -f ./servlet/realHTMLServlet.class
	rm -f ./servlet/realHTML/tomcat/connector/*.class
	rm -f ./servlet/realHTML4Natural.war
	rm -f ./servlet/web/WEBINF/classes/realHTMLServlet.class
	rm -f ./servlet/web/WEB-INF/lib/realHTMLconnector.jar
	rm -f ./librealHTMLconnector.so
	rm -f ./tests/*.class
	rm -f ./jniLibrary/*.o
