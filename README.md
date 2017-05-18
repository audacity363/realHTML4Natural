## realHtml4Natural
RealHtml4Natural is a alternative webframework for Natural build on top of the Natural Native Interfaces and distributed under the GPL-3.0 license.
The project started in 2014 because i had enough from the slow and inconvenient webframework Natural4Ajax. I wanted to write my own html and have complete control over the page generation process.

A full documentation you can find here: http://engemann.me/realhtml/docs


### Warning:
#### For Version 2.0_beta:
This Version is completely tested and works fine but I can't promise (and will not guarantee) that is will work so smooth on your system. When you find something that doesn't work it would be nice if you would contact me or create a issue here on GitHub. 

#### For Version 1.0:
In general the framework works but the webserver runs on http and not https. I don't know if there are some big bugs or memory leaks. (When you want to use the bleeding edge version the develop branch would be the right thing for you).
This is the first version of the framework. She was meant to just show that there is a better way to handle Natural in the web. I currently working on a new version wich will be completly tested and ready for production use (See the develop branch for more informations).

## Quick start guide

1. create a route file like [this](web_server/routes.xml)
2. create a config file like [this](web_server/config.xml)
(I would highly recommend to use the [TomcatConnector](https://github.com/audacity363/realHTML_TomcatConnector) inset of this Webserver)
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
   1 HTTP_KEYS (A/1:*)
   1 HTTP_VALS (A/1:*)
   1 HTTP_VALS_LENGTH (I4/1:*)
   1 HTTP_PARM_COUNT (I4)
   1 HTTP_REQ_TYPE (A) DYNAMIC

   1 REALHTML_TMPFILE (A) DYNAMIC
   1 REALHTML_SETTINGS (A) DYNAMIC
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
