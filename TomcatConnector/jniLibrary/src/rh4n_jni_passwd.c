#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>

#include "realHTML_tomcat_connector_JNINatural.h"

JNIEXPORT jint JNICALL Java_realHTML_tomcat_connector_JNINatural_jni_1passwd
  (JNIEnv *env, jobject jclass, jstring juser, jstring jpasswd) {

    const char *user = NULL, *password = NULL;
    char *encrypted = NULL;
    struct passwd *pwd = NULL;
    int ret = 0, authOk = 0;
    
    if((user = (*env)->GetStringUTFChars(env, juser, NULL)) == NULL) {
        ret = -1;
        goto passwdcleanup;
    }


    if((password = (*env)->GetStringUTFChars(env, jpasswd, NULL)) == NULL) {
        ret = -2;
        goto passwdcleanup;
    }
 
    if(strlen(user) == 0) {
        ret = -3;
        goto passwdcleanup;
    }

    if((pwd = getpwnam(user)) == NULL) {
        printf("Could not get password record: [%s]\n", strerror(errno)); fflush(stdout);
        ret = -4;
        goto passwdcleanup;
    }
    
    encrypted = crypt(password, pwd->pw_passwd);
#if 0
    if((encrypted = crypt(passwd, pwd->pw_passwd)) == NULL) {
        printf("Crypterror: [%s]\n", strerror(errno)); fflush(stdout);
        ret = -5;
        goto passwdcleanup;
    }
#endif
    
    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if(!authOk) {
        printf("Wrong password\n"); fflush(stdout);
        ret = 1;
        goto passwdcleanup;
    }

    ret = 0;

passwdcleanup:
   
    if(user != NULL)   (*env)->ReleaseStringUTFChars(env, juser, user);
    if(password != NULL) (*env)->ReleaseStringUTFChars(env, jpasswd, password);

    password = NULL;
    pwd = NULL;
    encrypted = NULL;

    return(ret);
}
