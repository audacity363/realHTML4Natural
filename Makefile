# Just a reminder for the next commit
# TODO: Put the the classes in the makefile template

CC = /usr/vac/bin/xlc
AR = /usr/bin/ar
JAR = /usr/java8_64/bin/jar
JAVAC = /usr/java8_64/bin/javac
JAVAH = /usr/java8_64/bin/javah

LFLAGS1_SO = -G
LFLAGS2_SO = 

TOMCATCLASSPATH = /opt/tomcat/tomcat-rh4n/lib/*
RH4NCLASSPATH = ./TomcatConnector/servlet/web/WEB-INF/lib/realHTMLconnector.jar:./TomcatConnector/servlet/web/WEB-INF/lib/minimal-json-0.9.5.jar:./TomcatConnector/servlet/web/WEB-INF/lib/commons-io-1.3.2.jar:./bin/servlet/lib
CLASSPATH = "$(TOMCATCLASSPATH):$(RH4NCLASSPATH)"

JNIINCLUDE = -I/usr/java8_64/include/ -I/usr/java8_64/include//linux/


INCLUDE = -I./include/ \
		  -I./libs/rh4n_utils/include \
          -I./libs/rh4n_logging/include/ \
		  -I./libs/rh4n_vars/include/ \
		  -I./libs/rh4n_jsongenerator/include/ \
		  -I./libs/rh4n_ldaparser/include/ \
		  -I./libs/rh4n_var2name/include/ \
		  -I./natuser_lib/include/ \
		  -I./TomcatConnector/jniLibrary/include/ \
		  $(JNIINCLUDE)

LIBS = -L./bin/libs \
	   -ldl -lrh4nutils -lrh4nlogging -lrh4nvar2name -lrh4nvars -lrh4nldaparser \
	   -lrh4njsongenerator -lcrypt

CARGS1 = -g -c -fpic $(INCLUDE)
CARGS2 = 
CARGS_SO = -c -g -fpic $(INCLUDE)

LIBOUTPUT = ./bin/libs/

help:
	@printf "Targets:\n"
	@printf "\t+----------------------------CORE:---------------------------+\n"
	@printf "\t| utils                                                      |\n"
	@printf "\t|   Utils library (./libs/rh4n_utils)                        |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| logging                                                    |\n"
	@printf "\t|   Logging library (./libs/rh4n_logging)                    |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| vars                                                       |\n"
	@printf "\t|   Vars library (./libs/rh4n_vars)                          |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| jsongenerator                                              |\n"
	@printf "\t|   JSON Generator library (./libs/rh4n_jsongenerator)       |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| ldaparser                                                  |\n"
	@printf "\t|   LDA parser library (./libs/rh4n_ldaparser)               |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| var2name                                                   |\n"
	@printf "\t|   Name and Postion matching library (./libs/rh4n_var2name) |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| natuserlib                                                 |\n"
	@printf "\t|   Shared library for natural (./natuser_lib)               |\n"
	@printf "\t+------------------------------------------------------------+\n\n"
	@printf "\t+--------------------------Tomcat:---------------------------+\n"
	@printf "\t| tomcatconnector_package                                    |\n"
	@printf "\t|   Utils for the servlet                                    |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| tomcatconnector_servlet                                    |\n"
	@printf "\t|   Servlet which handels requests                           |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| tomcatconnector_warfile                                    |\n"
	@printf "\t|   Creates a deployable warfile                             |\n"
	@printf "\t+------------------------------------------------------------+\n"
	@printf "\t| jnilibrary                                                 |\n"
	@printf "\t|   JNI library required by the servlet                      |\n"
	@printf "\t|                                                            |\n"
	@printf "\t| jniheader                                                  |\n"
	@printf "\t|   Generates JNI header file required by the JNI Library    |\n"
	@printf "\t+------------------------------------------------------------+\n\n"
	@printf "\tall: Compiles everything\n"

all: utils logging vars var2name ldaparser natuserlib jnilibrary tomcatconnector_warfile
	@printf "You find the binarys under ./bin\n"

#                         +------------------+
#-------------------------|      Utils       |---------------------------------
#                         +------------------+

UTILS_SRC = ./libs/rh4n_utils/src
UTILS_BIN = ./bin/rh4n_utils
UTILS_OBJS = rh4n_utils.o
UTILS_LIB = librh4nutils.a

utils: utils_clean utils_pre $(UTILS_OBJS)
	@printf "Creating $(LIBOUTPUT)/$(UTILS_LIB)\n"
	@$(AR) -cru $(LIBOUTPUT)/$(UTILS_LIB) $(UTILS_BIN)/*.o
	@printf "Done compiling and linking utils\n"

$(UTILS_OBJS):
	@printf "CC $(UTILS_SRC)/$*.c => $(UTILS_BIN)/$*.o\n"
	@$(CC) $(CARGS1) -o $(UTILS_BIN)/$*.o $(UTILS_SRC)/$*.c

utils_pre:
	@printf "Creating utils output folder\n"
	@mkdir -p $(UTILS_BIN)
	@mkdir -p $(LIBOUTPUT)

utils_clean:
	@printf "Cleaning utils library\n"
	@rm -f $(LIBOUTPUT)/$(UTILS_BIN)
	@printf "Cleaning utils objects\n"
	@rm -f $(UTILS_BIN)/*.o

#                         +-----------------+
#-------------------------|     Logging     |----------------------------------
#                         +-----------------+

LOGGING_SRC = ./libs/rh4n_logging/src
LOGGING_BIN = ./bin/rh4n_logging
LOGGING_OBJS = rh4n_logging.c
LOGGING_LIB = librh4nlogging.a

logging: logging_clean logging_pre $(LOGGING_OBJS)
	@printf "Creating $(LIBOUTPUT)/$(LOGGING_LIB)\n"
	@$(AR) -cru $(LIBOUTPUT)/$(LOGGING_LIB) $(LOGGING_BIN)/*.o
	@printf "Done compiling and linking logging\n"

$(LOGGING_OBJS):
	@printf "CC $(LOGGING_SRC)/$*.c => $(LOGGING_BIN)/$*.o\n"
	@$(CC) $(CARGS1) -o $(LOGGING_BIN)/$*.o $(LOGGING_SRC)/$*.c

logging_pre:
	@printf "Creating logging output folder\n"
	@mkdir -p $(LOGGING_BIN)
	@mkdir -p $(LIBOUTPUT)

logging_clean:
	@printf "Cleaning logging library\n"
	@rm -f $(LIBOUTPUT)/$(UTILS_BIN)
	@printf "Cleaning logging objects\n"
	@rm -f $(LOGGING_BIN)/*.o


#                         +-----------------+
#-------------------------|      Vars       |----------------------------------
#                         +-----------------+

VARS_SRC = ./libs/rh4n_vars/src
VARS_BIN = ./bin/rh4n_vars
VARS_OBJS = rh4n_vars.o \
		    rh4n_vars_group.o \
		    rh4n_vars_print.o \
		    rh4n_vars_string.o \
		    rh4n_vars_array.o \
		    rh4n_vars_bool.o \
		    rh4n_vars_integer.o \
		    rh4n_vars_float.o \
		    rh4n_vars_ustring.o \
		    rh4n_vars_printjson.o \
		    rh4n_vars_free.o
VARS_LIB = librh4nvars.a

vars: logging vars_clean vars_pre $(VARS_OBJS)
	@printf "Creating $(LIBOUTPUT)/$(VARS_LIB)\n"
	@$(AR) -cru $(LIBOUTPUT)/$(VARS_LIB) $(VARS_BIN)/*.o
	@printf "Done compiling and linking vars\n"

$(VARS_OBJS):
	@printf "CC $(VARS_SRC)/$*.c => $(VARS_BIN)/$*.o\n"
	@$(CC) $(CARGS1) -o $(VARS_BIN)/$*.o $(VARS_SRC)/$*.c

vars_pre:
	@printf "Creating vars output folder\n"
	@mkdir -p $(VARS_BIN)
	@mkdir -p $(LIBOUTPUT)

vars_clean:
	@printf "Cleaning vars library\n"
	@rm -f $(LIBOUTPUT)/$(VARS_BIN)
	@printf "Cleaning vars objects\n"
	@rm -f $(VARS_BIN)/*.o

vars_test: var2name
	@$(CC) -g ./libs/rh4n_vars/test/main.c $(INCLUDE) -o ./libs/rh4n_vars/test/main $(LIBS)
	@./libs/rh4n_vars/test/main

#                         +-----------------+
#-------------------------|  JSON generator |----------------------------------
#                         +-----------------+

JSON_SRC = ./libs/rh4n_jsongenerator/src
JSON_BIN = ./bin/rh4n_jsongenerator
JSON_OBJS = rh4n_json_generator.o \
			rh4n_json_array.o \
			rh4n_json_group.o
JSON_LIB = librh4njsongenerator.a

json_generator: logging vars json_generator_clean json_generator_pre $(JSON_OBJS)
	@printf "Creating $(LIBOUTPUT)/$(JSON_LIB)\n"
	@$(AR) -cru $(LIBOUTPUT)/$(JSON_LIB) $(JSON_BIN)/*.o
	@printf "Done compiling and linking vars\n"

$(JSON_OBJS):
	@printf "CC $(JSON_SRC)/$*.c => $(JSON_BIN)/$*.o\n"
	@$(CC) $(CARGS1) -o $(JSON_BIN)/$*.o $(JSON_SRC)/$*.c

json_generator_pre:
	@printf "Creating json generator output folder\n"
	@mkdir -p $(JSON_BIN)
	@mkdir -p $(LIBOUTPUT)

json_generator_clean:
	@printf "Cleaning json generator library\n"
	@rm -f $(LIBOUTPUT)/$(JSON_BIN)
	@printf "Cleaning vars objects\n"
	@rm -f $(JSON_BIN)/*.o

json_test: json_generator
	@$(CC) -g ./libs/rh4n_jsongenerator/tests/main.c $(INCLUDE) -o ./libs/rh4n_jsongenerator/tests/main $(LIBS)
	@./libs/rh4n_jsongenerator/tests/main 2> ./jsonout
	@cat ./jsonout

#                         +-----------------+
#-------------------------|    LDA parser   |----------------------------------
#                         +-----------------+

LDAPARSER_SRC = ./libs/rh4n_ldaparser/src
LDAPARSER_BIN = ./bin/rh4n_ldaparser
LDAPARSER_OBJS = rh4n_ldaparser.o \
			     rh4n_lda_filehandling.o \
			     rh4n_lda_array.o \
			     rh4n_lda_utils.o \
			     rh4n_lda_variables.o \
			     rh4n_lda_print.o \
			     rh4n_lda_free.o
LDAPARSER_LIB = librh4nldaparser.a

ldaparser: logging ldaparser_clean ldaparser_pre $(LDAPARSER_OBJS)
	@printf "Creating $(LIBOUTPUT)/$(LDAPARSER_LIB)\n"
	@$(AR) -cru $(LIBOUTPUT)/$(LDAPARSER_LIB) $(LDAPARSER_BIN)/*.o
	@printf "Done compiling and linking ldaparser\n"

$(LDAPARSER_OBJS):
	@printf "CC $(LDAPARSER_SRC)/$*.c => $(LDAPARSER_BIN)/$*.o\n"
	@$(CC) $(CARGS1) -o $(LDAPARSER_BIN)/$*.o $(LDAPARSER_SRC)/$*.c

ldaparser_pre:
	@printf "Creating ldaparser output folder\n"
	@mkdir -p $(LDAPARSER_BIN)
	@mkdir -p $(LIBOUTPUT)

ldaparser_clean:
	@printf "Cleaning ldaparser library\n"
	@rm -f $(LIBOUTPUT)/$(UTILS_BIN)
	@printf "Cleaning ldaparser objects\n"
	@rm -f $(LDAPARSER_BIN)/*.o

#                         +-----------------+
#-------------------------|    Var2Name     |----------------------------------
#                         +-----------------+

VAR2NAME_SRC = ./libs/rh4n_var2name/src
VAR2NAME_BIN = ./bin/rh4n_var2name
VAR2NAME_OBJS = rh4n_v2n.o \
			    rh4n_v2n_resolvenames.o \
			    rh4n_v2n_resolveposition.o

VAR2NAME_LIB = librh4nvar2name.a

var2name: vars logging ldaparser var2name_clean var2name_pre $(VAR2NAME_OBJS)
	@printf "Creating $(LIBOUTPUT)/$(VAR2NAME_LIB)\n"
	@$(AR) -cru $(LIBOUTPUT)/$(VAR2NAME_LIB) $(VAR2NAME_BIN)/*.o
	@printf "Done compiling and linking var2name\n"

$(VAR2NAME_OBJS):
	@printf "CC $(VAR2NAME_SRC)/$*.c => $(VAR2NAME_BIN)/$*.o\n"
	@$(CC) $(CARGS1) -o $(VAR2NAME_BIN)/$*.o $(VAR2NAME_SRC)/$*.c

var2name_pre:
	@printf "Creating var2name output folder\n"
	@mkdir -p $(VAR2NAME_BIN)
	@mkdir -p $(LIBOUTPUT)

var2name_clean:
	@printf "Cleaning var2name library\n"
	@rm -f $(LIBOUTPUT)/$(UTILS_BIN)
	@printf "Cleaning var2name objects\n"
	@rm -f $(VAR2NAME_BIN)/*.o

var2name_test: var2name
	@$(CC) -g ./libs/rh4n_var2name/test/main.c $(INCLUDE) -o ./libs/rh4n_var2name/test/main $(LIBS)
	@./libs/rh4n_var2name/test/main

#                         +-----------------+
#-------------------------|   Natuser lib   |----------------------------------
#                         +-----------------+

NATUSER_OBJS = rh4n_nat_jumptab.o \
			   ncuxinit.o \
			   rh4n_nat_genjson.o \
			   rh4n_nat_init.o \
			   rh4n_nat_utils.o \
			   rh4n_nat_variablereadout.o \
			   rh4n_nat_geturlobj.o \
			   rh4n_nat_mbn.o \
			   rh4n_nat_getbodyobj.o \
			   rh4n_nat_getbodyvar.o \
			   rh4n_nat_geturlvar.o \
			   rh4n_nat_getuser.o \
			   rh4n_nat_debug_init.o
			   
NATUSER_READOUT_OBJS = rh4n_nat_a_handling.o \
					   rh4n_nat_i_handling.o \
					   rh4n_nat_l_handling.o \
					   rh4n_nat_f_handling.o \
					   rh4n_nat_dt_handling.o \
					   rh4n_nat_np_handling.o \
					   rh4n_nat_u_handling.o

NATUSER_PUTVAR_OBJS = rh4n_nat_put_a.o \
					  rh4n_nat_put_i.o \
					  rh4n_nat_put_l.o \
					  rh4n_nat_put_f.o \
					  rh4n_nat_put_aton.o

NATUSER_SRC = ./natuser_lib/src
NATUSER_BIN = ./bin/natuserlib

natuserlib: utils logging vars json_generator ldaparser var2name natuserlib_clean natuserlib_pre \
			$(NATUSER_OBJS) $(NATUSER_READOUT_OBJS) $(NATUSER_PUTVAR_OBJS)
	@printf "Linking librealHTML4Natural.so\n"
	$(CC) $(LFLAGS1_SO) $(NATUSER_BIN)/*.o $(LIBS) $(LFLAGS2_SO) -o ./bin/librealHTML4Natural.so
	@printf "Done compiling and linking natuserlib\n"

nat_test: natuserlib
	@printf "Cleaning Natural test\n"
	@rm -f ./natuser_lib/test/rh4n_nat_test_genjson
	@printf "CC rh4n_nat_test_genjson.c => rh4n_nat_test_genjson.c\n"
	@$(CC) -g ./natuser_lib/test/rh4n_nat_test_genjson.c $(INCLUDE) \
		-o ./natuser_lib/test/rh4n_nat_test_genjson $(LIBS)
	@printf "Running test\n"
	@export NATUSER=./bin/librealHTML4Natural.so; ./natuser_lib/test/rh4n_nat_test_genjson
	

$(NATUSER_OBJS): 
	@printf "CC $(NATUSER_SRC)/$*.c => $(NATUSER_BIN)/$*.o\n"
	@$(CC) $(CARGS_SO) -o $(NATUSER_BIN)/$*.o $(NATUSER_SRC)/$*.c

$(NATUSER_READOUT_OBJS): 
	@printf "CC $(NATUSER_SRC)/$*.c => $(NATUSER_BIN)/var_parm_handling/$*.o\n"
	@$(CC) $(CARGS_SO) -o $(NATUSER_BIN)/$*.o $(NATUSER_SRC)/var_parm_handling/$*.c

$(NATUSER_PUTVAR_OBJS):
	@printf "CC $(NATUSER_SRC)/put_var_handling/$*.c => $(NATUSER_BIN)/$*.o\n"
	@$(CC) $(CARGS_SO) -o $(NATUSER_BIN)/$*.o $(NATUSER_SRC)/put_var_handling/$*.c

natuserlib_pre:
	@printf "Creating natuser output folder\n"
	@mkdir -p $(NATUSER_BIN)

natuserlib_clean:
	@printf "Cleaning natuser objects\n"
	@rm -f $(NATUSER_BIN)/*.o
	@rm -f ./bin/librealHTML4Natural.so
	

#                         +-----------------+
#-------------------------| Tomcat Connector|----------------------------------
#                         +-----------------+

TOMCONNECTOR_SRC = ./TomcatConnector/servlet/src
TOMCONNECTOR_LIB_BIN = ./bin/servlet/lib

JAVA_JSON_PARSER_SRC = $(TOMCONNECTOR_SRC)/realHTML/tomcat/JSONMatcher
JAVA_JSON_PARSER =  $(JAVA_JSON_PARSER_SRC)/JSONArrayException.java \
					$(JAVA_JSON_PARSER_SRC)/JSONObjectException.java \
					$(JAVA_JSON_PARSER_SRC)/TypeException.java \
					$(JAVA_JSON_PARSER_SRC)/Types.java \
					$(JAVA_JSON_PARSER_SRC)/ArrayType.java \
					$(JAVA_JSON_PARSER_SRC)/JSONUtils.java \
					$(JAVA_JSON_PARSER_SRC)/LLNode.java \
					$(JAVA_JSON_PARSER_SRC)/LLHandler.java \
					$(JAVA_JSON_PARSER_SRC)/JSONObject.java \
					$(JAVA_JSON_PARSER_SRC)/JSONArray.java \
					$(JAVA_JSON_PARSER_SRC)/PrintFuncs.java \
					$(JAVA_JSON_PARSER_SRC)/JSONParser.java

JAVA_UTILS_PACKAGE_SRC = $(TOMCONNECTOR_SRC)/realHTML/tomcat/connector
JAVA_UTILS_PACKAGE = $(JAVA_UTILS_PACKAGE_SRC)/Environ.java \
					 $(JAVA_UTILS_PACKAGE_SRC)/RH4NReturn.java \
					 $(JAVA_UTILS_PACKAGE_SRC)/RH4NParams.java \
					 $(JAVA_UTILS_PACKAGE_SRC)/JNINatural.java \
					 $(JAVA_UTILS_PACKAGE_SRC)/JNILoader.java \
					 $(JAVA_UTILS_PACKAGE_SRC)/RH4NCallParms.java 

JAVA_UTILS_EXCEPTIONS_SRC = $(TOMCONNECTOR_SRC)/realHTML/servlet/exceptions
JAVA_UTILS_EXCEPTIONS = $(JAVA_UTILS_EXCEPTIONS_SRC)/RouteException.java \
						$(JAVA_UTILS_EXCEPTIONS_SRC)/EnvironmentException.java \
						$(JAVA_UTILS_EXCEPTIONS_SRC)/EnvironmentVarException.java \
						$(JAVA_UTILS_EXCEPTIONS_SRC)/XMLException.java

JAVA_ROUTING_PACKAGE_SRC = $(TOMCONNECTOR_SRC)/realHTML/tomcat/routing
JAVA_ROUTING_PACKAGE = $(JAVA_ROUTING_PACKAGE_SRC)/Route.java \
					   $(JAVA_ROUTING_PACKAGE_SRC)/PathType.java \
					   $(JAVA_ROUTING_PACKAGE_SRC)/PathEntry.java \
					   $(JAVA_ROUTING_PACKAGE_SRC)/PathTemplate.java \
					   $(JAVA_ROUTING_PACKAGE_SRC)/Routing.java 

JAVA_ENVIRONMENT_PACKAGE_SRC = $(TOMCONNECTOR_SRC)/realHTML/tomcat/environment
JAVA_ENVIRONMENT_PACKAGE = $(JAVA_ENVIRONMENT_PACKAGE_SRC)/EnvironmentVar.java \
						   $(JAVA_ENVIRONMENT_PACKAGE_SRC)/Environment.java \
						   $(JAVA_ENVIRONMENT_PACKAGE_SRC)/EnvironmentBuffer.java

JAVA_XML_PACKAGE_SRC = $(TOMCONNECTOR_SRC)/realHTML/tomcat/xml
JAVA_XML_PACKAGE = $(JAVA_XML_PACKAGE_SRC)/Export.java \
				   $(JAVA_XML_PACKAGE_SRC)/Import.java

JAVA_GUI_PACKAGE_SRC = $(TOMCONNECTOR_SRC)/realHTML/tomcat/gui
JAVA_GUI_PACKAGE = $(JAVA_GUI_PACKAGE_SRC)/RouteTree.java \
				   $(JAVA_GUI_PACKAGE_SRC)/RouteSorting.java

TOMCAT_SERVLETS = $(TOMCONNECTOR_SRC)/RealHTMLInit.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLHandler.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLLogin.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLLogout.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLLoadConfig.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLManageEnvironment.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLManageEnvironmentVars.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLManageRoute.java \
				  $(TOMCONNECTOR_SRC)/RealHTMLSaveConfig.java


TOMCAT_SERVLETS_BIN = ./bin/servlet/servlets

WARFILE_PREFIX = ./TomcatConnector/servlet/web

tomcatconnector_package: tomcatconnector_package_clean tomcatconnector_package_pre
	@printf "Compiling realHTML.servlet.exceptions\n"
	@$(JAVAC) -d $(TOMCONNECTOR_LIB_BIN) -cp $(CLASSPATH) $(JAVA_UTILS_EXCEPTIONS)
	@printf "Compiling realHTML.tomcat.JSONMatcher\n"
	@$(JAVAC) -d $(TOMCONNECTOR_LIB_BIN) -cp $(CLASSPATH) $(JAVA_JSON_PARSER)
	@printf "Compiling realHTML.tomcat.routing\n"
	@$(JAVAC) -d $(TOMCONNECTOR_LIB_BIN) -cp $(CLASSPATH) $(JAVA_ROUTING_PACKAGE)
	@printf "Compiling realHTML.tomcat.environment\n"
	@$(JAVAC) -d $(TOMCONNECTOR_LIB_BIN) -cp $(CLASSPATH) $(JAVA_ENVIRONMENT_PACKAGE)
	@printf "Compiling realHTML.tomcat.xml\n"
	@$(JAVAC) -d $(TOMCONNECTOR_LIB_BIN) -cp $(CLASSPATH) $(JAVA_XML_PACKAGE)
	@printf "Compiling realHTML.tomcat.gui\n"
	@$(JAVAC) -d $(TOMCONNECTOR_LIB_BIN) -cp $(CLASSPATH) $(JAVA_GUI_PACKAGE)
	@printf "Compiling realHTML.tomcat.connector\n"
	@$(JAVAC) -d $(TOMCONNECTOR_LIB_BIN) -cp $(CLASSPATH) $(JAVA_UTILS_PACKAGE)
	@printf "Creating realHTMLconnector.jar\n"
	@cd $(TOMCONNECTOR_LIB_BIN) && jar cf ../../../TomcatConnector/servlet/web/WEB-INF/lib/realHTMLconnector.jar ./realHTML
	
tomcatconnector_package_pre:
	@printf "Creating tomcatconnector package output folder\n"
	@mkdir -p $(TOMCONNECTOR_LIB_BIN)

tomcatconnector_package_clean:
	@printf "Cleaning tomcatconnector package\n"
	@rm -rf $(TOMCONNECTOR_LIB_BIN)
	@printf "Cleaning realHTMLconnector.jar\n"
	@rm -f ./TomcatConnector/servlet/web/WEB-INF/lib/realHTMLconnector.jar

tomcatconnector_servlet: tomcatconnector_package tomcatconnector_servlet_clean tomcatconnector_servlet_pre
	@printf "Compiling servlets\n"
	@$(JAVAC) -d $(TOMCAT_SERVLETS_BIN) -cp $(CLASSPATH) $(TOMCAT_SERVLETS)
	@printf "Copying servlet classes into warfile\n"
	@cp $(TOMCAT_SERVLETS_BIN)/*.class $(WARFILE_PREFIX)/WEB-INF/classes/

tomcatconnector_servlet_pre:
	@printf "Creating servlets output folder\n"
	@mkdir -p $(TOMCAT_SERVLETS_BIN)

tomcatconnector_servlet_clean:
	@printf "Cleaning servlets\n"
	@rm -rf $(TOMCAT_SERVLETS_BIN)
	@printf "Cleaning servlets in warfile\n"
	@rm -f $(WARFILE_PREFIX)/WEB-INF/classes/*.class

tomcatconnector_warfile: jnilibrary tomcatconnector_package tomcatconnector_servlet \
						 tomcatconnector_warfile_pre tomcatconnector_warfile_clean
	@printf "Creating realHTML4Natural.war\n"
	@cd $(WARFILE_PREFIX); $(JAR) cvf ../../../bin/realHTML4Natural.war .

tomcatconnector_warfile_pre:
	@printf "Creating warfile output folder\n"
	@mkdir -p ./bin
	
tomcatconnector_warfile_clean:
	@printf "Cleaning warfile\n"
	@rm -f ./bin/realHTML4Natural.war

#                         +-----------------+
#-------------------------|   JNI library   |----------------------------------
#                         +-----------------+

JNI_JSON_HANDLERS = json_utils.o plain_handlers.o 1d_handlers.o \
					2d_handlers.o 3d_handlers.o handlers.o json_main.o

JNI_NAT_HANDLERS = rh4n_nat_parmgeneration.o rh4n_natcall_process.o rh4n_nat_errorhandling.o

JNI_MEMBER = rh4n_jni_main.o rh4n_jni_passwd.o rh4n_jni_javaParmReadout.o rh4n_jni_environ.o

JNI_BIN = ./bin/jnilib
JNI_SRC = ./TomcatConnector/jniLibrary/src

jnilibrary: utils logging vars jniheader jnilibrary_clean jnilibrary_pre $(JNI_JSON_HANDLERS) $(JNI_NAT_HANDLERS) $(JNI_MEMBER)
	@printf "Linking librealHTMLconnector.so\n"
	@$(CC) $(LFLAGS1_SO) $(JNI_BIN)/*.o $(LIBS) -o ./bin/librealHTMLconnector.so

$(JNI_MEMBER):
	@printf "CC $(JNI_SRC)/$*.c => $(JNI_BIN)/$*.o\n"
	@$(CC) $(CARGS_SO) -o $(JNI_BIN)/$*.o $(JNI_SRC)/$*.c

$(JNI_JSON_HANDLERS):
	@printf "CC $(JNI_SRC)/json/$*.c => $(JNI_BIN)/$*.o\n"
	@$(CC) $(CARGS_SO) -o $(JNI_BIN)/$*.o $(JNI_SRC)/json/$*.c

$(JNI_NAT_HANDLERS):
	@printf "CC $(JNI_SRC)/natural/$*.c => $(JNI_BIN)/$*.o\n"
	@$(CC) $(CARGS_SO) -o $(JNI_BIN)/$*.o $(JNI_SRC)/natural/$*.c

jniheader: tomcatconnector_package
	@printf "Generating JNI headerfile\n"
	@cd $(TOMCONNECTOR_LIB_BIN); $(JAVAH) -jni \
		-o ../../../TomcatConnector/jniLibrary/include/realHTML_tomcat_connector_JNINatural.h \
		realHTML.tomcat.connector.JNINatural

jnilibrary_pre:
	@printf "Creating jnilib output folder\n"
	@mkdir -p $(JNI_BIN)

jnilibrary_clean:
	@printf "Cleaning jnilibrary objects\n"
	@rm -f $(JNI_BIN)/*.o
	@printf "Cleaning librealHTMLConnector.so\n"
	@rm -f ./bin/librealHTMLConnector.so

