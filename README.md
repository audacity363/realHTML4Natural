## realHtml4Natural
RealHtml4Natural is a alternative webframework for Natural build on top of the Natural Native Interfaces and distributed under the GPL-3.0 license.
The project started in 2014 because i had enough from the slow and inconvenient webframework Natural4Ajax. I wanted to write my own html and have complete control over the page generation process.

You can find more informations here: http://engemann.me/realhtml
A full documentation you can find here: http://engemann.me/realhtml


### Warning:
In general the framework works but the webserver runs on http and not https. I don't know if there are some big bugs or memory leaks.

## Quick start guide

1. create a route file like [this](web_server/routes.xml)
2. create a config file like [this](web_server/config.xml)
3. start the webserver with ./web_server/miniweb
4. create a new subprogram in a natural library which looks like this:
``` natural
define data
    parameter using reqinfos
    local using yourlda
end-define

move "your_template.html" to realhtml_template
move "yourlda" to realhtml_ldaname

* your code goes here

call interface4 "genpage" realhtml_ldaname realhtml_template 
                          req_file REQ_SETTINGS page 
END
```

The PDA "reqinfos" must look like this:
```
DEFINE DATA PARAMETER
/* >Natural Source Header 000000
/* :Mode S
/* :CP
/* <Natural Source Header
1 VALUES_REQ (A2024/1:20)
1 VALUE_LENGTH (I4/1:20)
1 REQ_KEYS (A100/1:20)
1 PARM_COUNT (I4)
1 REQ_TYPE (A5)
1 REQ_FILE (A100)
1 REQ_SETTINGS (A) DYNAMIC
END-DEFINE

```
5. Your LDA have to look like this:
```
DEFINE DATA LOCAL
/* >Natural Source Header 000000
/* :Mode S
/* :CP
/* <Natural Source Header
1 REALHTML_LDANAME (A20)
1 REALHTML_TEMPLATE (A20)
1 page
* under the page group you can define the variablen for the template
end-define
```
6. Then just write your template and place it into your template folder. 
7. Now you can call the Natural subprogram over your browser and the genrated html page will be shown.
