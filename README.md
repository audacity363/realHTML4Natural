## realHtml4Natural
RealHtml4Natural is an webframework for Natural build on top of the Natural Native Interfaces and distributed under the GPL-3.0 license.
The project started in 2014 because i had enough from the slow and inconvenient webframework Natural4Ajax. 

A full documentation you can find here: http://engemann.me/realhtml/docs

It is basicly a converter from JSON to Natural variables and the other way around. With this functionality you can implement a REST API with Natural as an backend. 

## Example

This is just the natural part. For the full example with all steps consult the documentation.

``` natural
define data
    parameter 
        1 HTTP_REQUEST_TYPE (A) DYNAMIC /* Contains "GET", "POST", "PUT", "DELETE" 
        1 RH4NINTERNALS (B) DYNAMIC
    local using yourlda
end-define

output-struct := "Hello World"

call interface4 "RHGENJS" rh4ninternals "output-struct@YOURLDA" output-struct
                           
END
```
LDA "yourlda":
```
DEFINE DATA LOCAL
/* >Natural Source Header 000000
/* :Mode S
/* :CP
/* <Natural Source Header
1 output-struct
    2 message (A) DYNAMIC
end-define
```

This program would generate the following JSON:

``` json
{
    "message": "Hello World"
}
```
