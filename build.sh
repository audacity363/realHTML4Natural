#!/bin/bash

function find_bin {
    for x in ${PATH//://${1} }${1}; do
        [ -f "$x" ] && echo $x
    done
}

function find_proc {
    printf "Searching for %s..." $1;
    procpath=`find_bin $1`;
    if [ -z $procpath ]; then
        printf "...didn't find %s\n" $1
        if [ $2 == 1 ]; then
            exit 1
        fi
    fi
    printf "...found %s at [%s]\n" $1 $procpath;
}

function check_progs {
    # Checking for a compiler. GCC or XLC are supported
    find_proc gcc 0
    if [ -z $procpath ]; then
        find_proc xlc 0
        if [ -z $procpath ]; then
            printf "Can not find any compatible compiler\n",
            exit 1
        fi
        cc_str="xlc"
    else
        cc_str="gcc"
    fi
    
    cc=$procpath
    printf "Using compiler at [%s]\n" $cc

    # Checking for make
    find_proc make 1
    makepath=$procpath

    # Check for javac 
    find_proc javac 1
    javacpath=$procpath

    # Check for jar
    find_proc jar 1
    jarpath=$procpath

    # Check for awk
    find_proc awk 1
    awkpath=$procpath

    # Check for sed 
    find_proc sed 1
    sedpath=$procpath

    #check for ar
    find_proc ar 1
    arpath=$procpath
}

function find_tomcat {
    printf "Searching for tomcat lib path...\n";
    tomcat_lib_path="$(find / -name 'servlet-api.jar' 2>/dev/null)"
    tomcat_lib_path_arr=($tomcat_lib_path);

    if [ ${#tomcat_lib_path_arr[@]} -eq 0 ]; then
        printf "...didn't find a servlet-api.jar file\n";
        exit 1;
    fi
   
    # Check if multiple jars where found
    if [ ${#tomcat_lib_path_arr[@]} -gt 1 ]; then
        printf "Found %d versions of the servlet api jar. Which one should I use?\n" ${#tomcat_lib_path_arr[@]}
        i=0
        for file in $tomcat_lib_path; do
            printf "[%d] %s\n" $i $file;
            i=`expr $i + 1`;
        done
        printf "Please choose a number [0]: ";
        read -r index
        if [ -z $index ]; then index=0; fi
        tomcat_lib_path=${tomcat_lib_path_arr[$index]};
        printf "Using servlet api jar: [%s]\n" $tomcat_lib_path
    else
        printf "...found api at [%s]\n" $tomcat_lib_path;
    fi
}

function find_jdk_includes {
    printf "Searching for java headers..."
    java_inc="$(find / -name 'jni.h' 2>/dev/null)";
    java_inc_arr=($java_inc);

    if [ ${#java_inc_arr[@]} -eq 0 ]; then
        printf "...didn't find java headers. Sure that jdk is istalled?\n"
        exit 1
    fi 

    if [ ${#java_inc_arr[@]} -gt 1 ]; then
        printf "Found %d locations of jni.h. Which one should I use?\n" ${#java_inc_arr[@]}
        i=0
        for file in $java_inc_arr; do
            printf "[%d] %s\n" $i `dirname ${file}`;
            i=`expr $i + 1`;
        done
        printf "Please choose a number [0]: ";
        read -r index
        if [ -z $index ]; then index=0; fi
        java_inc=${java_inc_arr[$index]};
        java_inc=`dirname ${java_inc}`;
        printf "Using headers in: [%s]\n" $java_inc;
    else
        java_inc="`dirname ${java_inc}`/";
        printf "...found headers at [%s]\n" $java_inc;
    fi
}

curwd=`pwd`;
curwd="$curwd/";
scriptname=$0;
basedir=`dirname "${scriptname}"`

#if [ $curwd != $basedir ]; then
#    printf "Please start the script from %s\n" $basedir
#    exit 1
#fi

# Check if all programs exists
check_progs

# Find the tomcat servlet jar
find_tomcat
CLASSPATH="$tomcat_lib_path:$curwd/TomcatConnector/servlet/src/:$curwd/TomcatConnector/servlet/web/WEB-INF/lib/realHTMLconnector.jar";

find_jdk_includes

if [ $cc_str == 'gcc' ]; then
    ccflags1="-c -g -fPIC";
    ccflags2="";
    ldargs1so="-shared";
    ldargs2so="";
else
    ccflags1="dummy";
    ldargsso="-G";
fi

# Check if a tomcat with the servlet api is installed

makefilepath=`pwd`/Makefile

# Copy the template to the real file name. Ignoring comment lines

cat `pwd`/Makefile.rh4n | grep -v "^#" > $makefilepath

$sedpath -i "s~{{CC}}~$cc~g" $makefilepath
$sedpath -i "s~{{CLASSPATH}}~$CLASSPATH~g" $makefilepath
$sedpath -i "s~{{AR}}~$arpath~g" $makefilepath
$sedpath -i "s~{{CARGS1}}~$ccflags1~g" $makefilepath
$sedpath -i "s~{{CARGS2}}~$ccflags2~g" $makefilepath
$sedpath -i "s~{{LDARGS1_SO}}~$ldargs1so~g" $makefilepath
$sedpath -i "s~{{LDARGS2_SO}}~$ldargs2so~g" $makefilepath
$sedpath -i "s~{{CURWD}}~$curwd~g" $makefilepath
$sedpath -i "s~{{JARBIN}}~$jarpath~g" $makefilepath
$sedpath -i "s~{{JAVAINC}}~$java_inc~g" $makefilepath
$sedpath -i "s~{{JAVACBIN}}~$javacpath~g" $makefilepath

printf "==================================================================\n";
cat $makefilepath

printf "==================================================================\n";

make all
