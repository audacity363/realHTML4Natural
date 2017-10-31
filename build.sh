#!/bin/bash

function find_bin {
    progs=()
    for x in ${PATH//://${1} }${1}; do
        [ -f "$x" ] && progs+=($x)
    done
}

function find_proc {
    printf "Searching for %s..." $1;
    find_bin $1;
    if [ ${#progs[@]} -eq 0 ]; then
        printf "...didn't find %s\n" $1
        if [ $2 == 1 ]; then
            exit 1
        fi
    fi

    if [ ${#progs[@]} -gt 1 ]; then
        printf "\n"
        echo ${progs[*]}
        i=0;
        for file in ${progs[@]}; do
            printf "[%d] %s\n" $i $file
            i=`expr $i + 1`;
        done
    
        printf "Found multiple. Which one should I use? [0] ";
        read -r index;
        if [ -z $index ]; then index=0; fi
        procpath=${progs[$index]};
        printf "...found %s at [%s]\n" $1 $procpath;

    else
        procpath=${progs[0]};
        printf "...found %s at [%s]\n" $1 $procpath;
    fi
}

function check_progs {
    # Checking for a compiler. GCC or XLC are supported
    find_proc xlc 0
    if [ -z $procpath ]; then
        find_proc gcc 0
        if [ -z $procpath ]; then
            printf "Can not find any compatible compiler\n",
            exit 1
        fi
        cc_str="gcc"
    else
        cc_str="xlc"
    fi
    
    cc=$procpath
    printf "Using compiler at [%s]\n" $cc

    # Checking for make
    find_proc make 1
    makepath=$procpath

    # Check for javac 
    find_proc javac 1
    javacpath=$procpath

    # Check for javac 
    find_proc perl 1
    perlpath=$procpath

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
        for file in ${java_inc_arr[@]}; do
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

printf "%s\n" $PATH

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
    ccflags1="-g -c -fpic";
    ldargs1so="-G -bsymbolic -bexpall -bnoentry";
    ldargs2so="";
fi

# Check if a tomcat with the servlet api is installed

makefilepath=`pwd`/Makefile

# Copy the template to the real file name. Ignoring comment lines

cat `pwd`/Makefile.rh4n | grep -v "^#" > $makefilepath

$perlpath -pi -e "s~{{CC}}~$cc~g" $makefilepath
$perlpath -pi -e "s~{{CLASSPATH}}~$CLASSPATH~g" $makefilepath
$perlpath -pi -e "s~{{AR}}~$arpath~g" $makefilepath
$perlpath -pi -e "s~{{CARGS1}}~$ccflags1~g" $makefilepath
$perlpath -pi -e "s~{{CARGS2}}~$ccflags2~g" $makefilepath
$perlpath -pi -e "s~{{LDARGS1_SO}}~$ldargs1so~g" $makefilepath
$perlpath -pi -e "s~{{LDARGS2_SO}}~$ldargs2so~g" $makefilepath
$perlpath -pi -e "s~{{CURWD}}~$curwd~g" $makefilepath
$perlpath -pi -e "s~{{JARBIN}}~$jarpath~g" $makefilepath
$perlpath -pi -e "s~{{JAVAINC}}~$java_inc~g" $makefilepath
$perlpath -pi -e "s~{{JAVACBIN}}~$javacpath~g" $makefilepath

exit

printf "==================================================================\n";
cat $makefilepath

printf "==================================================================\n";

make all
