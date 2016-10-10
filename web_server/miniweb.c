/////////////////////////////////////////////////////////////////////////////
//
// miniweb.c
//
// MiniWeb start-up code
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <mxml.h>

#include "httppil.h"
#include "httpapi.h"
#include "revision.h"
#ifdef MEDIA_SERVER
#include "mediaserver.h"
#endif

int uhMpd(UrlHandlerParam* param);
int ehMpd(MW_EVENT msg, int argi, void* argp);
int uhStats(UrlHandlerParam* param);
int uhVod(UrlHandlerParam* param);
int uhLib(UrlHandlerParam* param);
int uhVodStream(UrlHandlerParam* param);
int uhStream(UrlHandlerParam* param);
int ehVod(MW_EVENT msg, int argi, void* argp);
int uhTest(UrlHandlerParam* param);
int uh7Zip(UrlHandlerParam* param);
int uhFileStream(UrlHandlerParam* param);
int uhAsyncDataTest(UrlHandlerParam* param);
int uhRTSP(UrlHandlerParam* param);
int uhSerial(UrlHandlerParam* param);

UrlHandler urlHandlerList[]={
	{"stats", uhStats, NULL},
#ifdef ENABLE_SERIAL
	{"serial", uhSerial, NULL},
#endif
#ifdef HAVE_THREAD
	{"async", uhAsyncDataTest, NULL},
#endif
#ifdef MEDIA_SERVER
	{"test.sdp", uhRTSP, NULL},
	{"MediaServer/VideoItems/", uhMediaItemsTranscode, ehMediaItemsEvent},
#endif
#ifdef _7Z
	{"7z", uh7Zip, NULL},
#endif
#ifdef _MPD
	{"mpd", uhMpd, ehMpd},
#endif
#ifdef _VOD
	{"vodstream", uhVodStream,NULL},
	{"vodlib", uhLib,0},
	{"vodplay", uhVod,ehVod},
	{"stream", uhStream,NULL},
#endif
	{NULL},
};

#ifndef DISABLE_BASIC_WWWAUTH
AuthHandler authHandlerList[]={
	{"stats", "user", "pass", "group=admin", ""},
	{NULL}
};
#endif

HttpParam httpParam;

extern FILE *fpLog;


//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it needs to substitute variables
//////////////////////////////////////////////////////////////////////////
int DefaultWebSubstCallback(SubstParam* sp)
{
	// the maximum length of variable value should never exceed the number
	// given by sp->iMaxValueBytes
	if (!strcmp(sp->pchParamName,"mykeyword")) {
		return sprintf(sp->pchParamValue, "%d", 1234);
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it recevies posted data
//////////////////////////////////////////////////////////////////////////
int DefaultWebPostCallback(PostParam* pp)
{
  int iReturn=WEBPOST_OK;

  // by default redirect to config page
  //strcpy(pp->chFilename,"index.htm");

  return iReturn;
}

//////////////////////////////////////////////////////////////////////////
// callback from the web server whenever it receives a multipart
// upload file chunk
//////////////////////////////////////////////////////////////////////////
int DefaultWebFileUploadCallback(HttpMultipart *pxMP, OCTET *poData, size_t dwDataChunkSize)
{
  // Do nothing with the data
	int fd = (int)pxMP->pxCallBackData;
	if (!poData) {
		// to cleanup
		if (fd > 0) {
			close(fd);
			pxMP->pxCallBackData = NULL;
		}
		return 0;
	}
	if (!fd) {
		char filename[256];
		snprintf(filename, sizeof(filename), "%s/%s", httpParam.pchWebPath, pxMP->pchFilename);
		fd = open(filename, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, 0);
		pxMP->pxCallBackData = (void*)fd;
	}
	if (fd <= 0) return -1;
	write(fd, poData, dwDataChunkSize);
	if (pxMP->oFileuploadStatus & HTTPUPLOAD_LASTCHUNK) {
		close(fd);
		pxMP->pxCallBackData = NULL;
	}
	printf("Received %u bytes for multipart upload file %s\n", dwDataChunkSize, pxMP->pchFilename);
	return 0;
}

void Shutdown()
{
	//shutdown server
	mwServerShutdown(&httpParam);
	fclose(fpLog);
	UninitSocket();
}

char* GetLocalAddrString()
{
	// get local ip address
	struct sockaddr_in sock;
	char hostname[128];
	struct hostent * lpHost;
	gethostname(hostname, 128);
	lpHost = gethostbyname(hostname);
	memcpy(&(sock.sin_addr), (void*)lpHost->h_addr_list[0], lpHost->h_length);
	return inet_ntoa(sock.sin_addr);
}

int MiniWebQuit(int arg) {
	static int quitting = 0;
	if (quitting) return 0;
	quitting = 1;
	if (arg) printf("\nCaught signal (%d). MiniWeb shutting down...\n",arg);
	Shutdown();
	return 0;
}

void GetFullPath(char* buffer, char* argv0, char* path)
{
	char* p = strrchr(argv0, '/');
	if (!p) p = strrchr(argv0, '\\');
	if (!p) {
		strcpy(buffer, path);
	} else {
		int l = p - argv0 + 1;
		memcpy(buffer, argv0, l);
		strcpy(buffer + l, path);
	}
}

int loadConfig(char *path, char *environment)
{

    FILE *config_fp;
    char *c_port, *c_debug;

    mxml_node_t *tree, *env, *htdocs, *templates, *debug, *routes, *port, *tmp_node;

    printf("Loading config for [%s]\n", environment);

    if((config_fp = fopen(path, "r")) == NULL)
    {
        fprintf(stderr, "Error loading Config: [%s]\n", strerror(errno));
        return(-1);
    }

    if((tree = mxmlLoadFile(NULL, config_fp, MXML_NO_CALLBACK)) == NULL)
    {
        fprintf(stderr, "Error loading Config: [Can not get XML Tree]\n");
        fclose(config_fp);
        return(-2);
    }

    if((env = mxmlFindElement(tree, tree, "environment", "type", environment, MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Error loading Conf: [Could not find environment [%s]]\n", environment);
        return(-3);
    }

    if((htdocs = mxmlFindElement(env, tree, "htdocs", NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Error while loading Config: [\"htdocs\" entry not found]\n");
        fclose(config_fp);
        return(-3);
    }
    
    httpParam.pchWebPath = (char*)mxmlGetText(htdocs, NULL);

    if((routes = mxmlFindElement(env, tree, "routes", NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Error while loading Config: [\"routes\" entry not found]\n");
        fclose(config_fp);
        return(-4);
    }
    
    httpParam.config_path = (char*)mxmlGetText(routes, NULL);

    if((templates = mxmlFindElement(env, tree, "templates", NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Error while loading Config: [\"templates\" entry not found]\n");
        fclose(config_fp);
        return(-5);
    }

    httpParam.template_path = (char*)mxmlGetText(templates, NULL);

    if((tmp_node = mxmlFindElement(env, tree, "natsourcepath", NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Error while loading Config: [\"natsourcepath\" entry not found]\n");
        fclose(config_fp);
        return(-6);
    }

    httpParam.natsource_path = (char*)mxmlGetText(tmp_node, NULL);

    if((port = mxmlFindElement(env, tree, "port", NULL, NULL, MXML_DESCEND_FIRST)) != NULL)
    {
        printf("Found Port\n");
        c_port = (char*)mxmlGetText(port, NULL);
        printf("Port: [%s]\n", c_port);
        httpParam.httpPort = atoi(c_port);

    }


    if((debug = mxmlFindElement(env, tree, "debug", NULL, NULL, MXML_DESCEND_FIRST)) != NULL)
    {
        if((c_debug = (char*)mxmlGetText(debug, NULL)) == NULL)
        {
            fprintf(stderr, "Error while loading Config: Debug Tag is empty\n");
            fclose(config_fp);
            return(-6);
        }

        if(strcmp(c_debug, "true") == 0)
            httpParam.debug = 1;
        else if(strcmp(c_debug, "false") == 0)
            httpParam.debug = 0;
        else
        {
            fprintf(stderr, "Error while loding Config: DEBUG: Unkown value [%s]\n", c_debug);
            fclose(config_fp);
            return(-7);
        }
    }
    
    fclose(config_fp);
    printf("Successfully loaded configuration for environment [%s]\n", environment);
    return(0);
}

int main(int argc,char* argv[])
{
	fprintf(stderr,"MiniWeb (built on %s)\n(C)2005-2013 Written by Stanley Huang <stanleyhuangyc@gmail.com>\n\n", __DATE__);

    char *configfile_path, *environment = NULL;

    if((configfile_path = getenv("WEBCONFIG")) == NULL)
    {
        fprintf(stderr, "WEBCONFIG not set\n");
        exit(0);
    }


	signal(SIGINT, (void *) MiniWebQuit);
	signal(SIGTERM, (void *) MiniWebQuit);
	signal(SIGPIPE, SIG_IGN);

	//fill in default settings
	mwInitParam(&httpParam);
	httpParam.maxClients=32;
	httpParam.httpPort = 80;
	//GetFullPath(httpParam.pchWebPath, argv[0], "htdocs");
	httpParam.pxAuthHandler = authHandlerList;
	httpParam.pxUrlHandler=urlHandlerList;
	httpParam.flags=FLAG_DIR_LISTING;
	httpParam.tmSocketExpireTime = 15;
	httpParam.pfnPost = DefaultWebPostCallback;
	httpParam.pfnFileUpload = DefaultWebFileUploadCallback;

	//parsing command line arguments
	{
		int i;
		for (i=1;i<argc;i++) {
            if(argv[i][0]=='-' && argv[i][1]=='-')
            {
                if(strcmp(argv[i]+2, "environment") == 0)
                {
                    if(++i<argc) environment = argv[i]; 

                }
            }
			else if (argv[i][0]=='-') 
            {
				switch (argv[i][1]) {
				case 'h':
					fprintf(stderr,"Usage: miniweb	-h	: display this help screen\n"
						       "		-v	: log status/error info\n"
						       "		-p	: specifiy http port [default 80]\n"
						       "		-r	: specify http document directory [default htdocs]\n"
						       "		-l	: specify log file\n"
						       "		-m	: specifiy max clients [default 32]\n"
						       "		-M	: specifiy max clients per IP\n"
							   "		-s	: specifiy download speed limit in KB/s [default: none]\n"
							   "		-n	: disallow multi-part download [default: allow]\n"
						       "   	 	-d	: disallow directory listing [default ON]\n\n"
                               "        --environment: environment to load [default: none]\n\n");
					fflush(stderr);
                                        exit(1);

				case 'p':
					if ((++i)<argc) httpParam.httpPort=atoi(argv[i]);
					break;
				case 'r':
					if ((++i)<argc) strncpy(httpParam.pchWebPath, argv[i], sizeof(httpParam.pchWebPath) - 1);
					break;
				case 'l':
					if ((++i)<argc) fpLog=freopen(argv[i],"w",stderr);
					break;
				case 'm':
					if ((++i)<argc) httpParam.maxClients=atoi(argv[i]);
					break;
				case 'M':
					if ((++i)<argc) httpParam.maxClientsPerIP=atoi(argv[i]);
					break;
				case 's':
					if ((++i)<argc) httpParam.maxDownloadSpeed=atoi(argv[i]);
					break;
				case 'n':
					httpParam.flags |= FLAG_DISABLE_RANGE;
					break;
				case 'd':
					httpParam.flags &= ~FLAG_DIR_LISTING;
					break;
				}
			}

		}
	}

    if(environment == NULL)
    {
        fprintf(stderr, "Error: parameter --environment is not set\n");
        exit(1);
    }


    if(loadConfig(configfile_path, environment) < 0)
    {
        exit(1);
    }

	{
		int i;
		int error = 0;
		for (i = 0; urlHandlerList[i].pchUrlPrefix; i++) {
			if (urlHandlerList[i].pfnEventHandler) {
				if (urlHandlerList[i].pfnEventHandler(MW_PARSE_ARGS, urlHandlerList[i].pfnEventHandler, &httpParam))
					error++;
			}
		}
		if (error > 0) {
			printf("Error parsing command line options\n");
			return -1;
		}
	}

	InitSocket();

	{
		int n;
		printf("Host: %s:%d\n", GetLocalAddrString(), httpParam.httpPort);
		printf("Web root: %s\n",httpParam.pchWebPath);
		printf("Max clients (per IP): %d (%d)\n",httpParam.maxClients, httpParam.maxClientsPerIP);
		for (n=0;urlHandlerList[n].pchUrlPrefix;n++);
		printf("URL handlers: %d\n",n);
		if (httpParam.flags & FLAG_DIR_LISTING) printf("Dir listing enabled\n");
		if (httpParam.flags & FLAG_DISABLE_RANGE) printf("Byte-range disabled\n");

		//register page variable substitution callback
		//httpParam[i].pfnSubst=DefaultWebSubstCallback;

		//start server
		if (mwServerStart(&httpParam)) {
			printf("Error starting HTTP server\n");
		} else {
			mwHttpLoop(&httpParam);
		}
	}

	Shutdown();
	return 0;
}
////////////////////////////// END OF FILE //////////////////////////////
