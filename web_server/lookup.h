// XML Tag
#define ALIAS "alias"
#define RETURN "return"
#define PROGRAMM "programm"
#define ROUTE "route"
#define LIBRARY "library"
#define DEBUG "debug"
#define TEXT "#text"
#define PATH_PARM "path"

//Notetypes
#define OPEN 1
#define CLOSE 15

typedef struct {
    char url[500];
    char programm[10];
    char alias[100];
    char returncode[4];
    char library[20];
    bool debug;
    bool found;
    bool found_programm;
    bool found_debug;
    bool found_alias;
    bool found_returncode;
    bool found_library;
    bool error;
} path_struct;

typedef struct {
    bool found_root;
    bool found_prog;
    bool found_alias;
    bool found_return;
    bool found_library;
} found_struct;

path_struct getpathsettings(char*, char*, char*);
