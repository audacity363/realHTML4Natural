#JAVACLASSPATH = ${CURDIR}/servlet/web/WEB-INF/lib/realHTMLconnector.jar:/srv/apache-tomcat-8.5.9/lib/servlet-api.jar
CLASSPATH = "/u/it/a140734/rh4n/realHTML4Natural/TomcatConnector/servlet/web/WEB-INF/lib/realHTMLconnector.jar:/opt/tomcat/apache-tomcat-8.0.41/lib/servlet-api.jar:./servlet/bin/:./servlet/web/WEB-INF/lib/minimal-json-0.9.4.jar"


#INCLUDES = -I/usr/java/jdk1.8.0_111/include/ \
#		   -I/usr/java/jdk1.8.0_111/include/linux/ \
#		   -I./jniLibrary/

INCLUDES = -I/SAG/cjp/v16/include/ \
		   -I./jniLibrary/include/ \
		   -I../libs/varhandle_lib/src/


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
					 ./servlet/src/realHTML/tomcat/connector/Router.java \
					 ./servlet/src/realHTML/tomcat/connector/ConfigurationLoader.java \
					 ./servlet/src/realHTML/tomcat/connector/RH4NCallParms.java 

JNI_JSON_HANDLERS = json_utils.o plain_handlers.o 1d_handlers.o \
					2d_handlers.o 3d_handlers.o handlers.o callNatural.o

JNI_MEMBER = main.o natural.o

all:
	@printf "targets:\n"
	@printf "\tcomplete      : Build the complete connector\n"
	@printf "\twarfile       : Pack the .war file for deployment\n"
	@printf "\tjni_connector : Compile the utility library\n"
	@printf "\ttomcat_servlet: Compile the tomcat servlet\n"
	@printf "\tjni_so        : Compile the JNI so object\n"

complete:
	make clean
	make jni_connector
	make tomcat_servlet
	make warfile
	make jni_so

cleanjava:
	@make cleanwarfile
	@make cleanservletclass
	@make cleanpackage

warfile: cleanwarfile servletclass
	@printf "Copy package\n"
	@cp ./servlet/bin/realHTMLServlet.class ./servlet/web/WEB-INF/classes/
	@printf "Copy servlet class\n"
	@cp ./servlet/bin/realHTMLconnector.jar ./servlet/web/WEB-INF/lib/
	@printf "Creating war file\n"
	@cd ./servlet/web; jar cvf ../../realHTML4Natural.war .

cleanwarfile:
	@printf "Cleaning warfile\n"
	@rm -f ./realHTML4Natural.war
	@printf "Cleaning skeleton\n"
	@rm -f ./servlet/web/WEB-INF/classes/*.class
	@rm -f ./servlet/web/WEB-INF/lib/realHTMLconnector.jar

servletclass: cleanservletclass package
	@printf "Compiling Servlet\n"
	@javac -d ./servlet/bin -cp $(CLASSPATH) servlet/src/realHTMLServlet.java

cleanservletclass:
	@printf "Cleaning servlet class\n"
	@rm -f ./servlet/bin/realHTMLServlet.class

package: cleanpackage
	@printf "Compiling realHTML.tomcat.JSONMatcher\n"
	@javac -d ./servlet/bin -cp $(CLASSPATH) $(JSON_JAVA_PARSER)
	@printf "Compiling realHTML.tomcat.connector\n"
	@rm -rf ./bin/realHTMLtomcat/connector/*.class
	@javac -d ./servlet/bin -cp $(CLASSPATH) $(JAVA_UTILS_PACKAGE)
	@printf "Creating realHTMLconnector.jar\n"
	@cd ./servlet/bin && jar cf ./realHTMLconnector.jar ./realHTML

cleanpackage:
	@printf "Cleaning realHTML.tomcat.JSONMatcher\n"
	@rm -rf ./bin/realHTMLtomcat/JSONMatcher/*.class
	@printf "Cleaning realHTML.tomcat.connector\n"
	@rm -rf ./bin/realHTMLtomcat/connector/*.class


jnilib: jniclean jniheader $(JNI_MEMBER) $(JNI_JSON_HANDLERS)
	${CC} -G -o librealHTMLconnector.so ./jniLibrary/bin/*.o

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
