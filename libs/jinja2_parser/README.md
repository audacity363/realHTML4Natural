## Jinja-Parser

## Bescheibung
Ein Parser fuer HTML-Templates welche im Jinja2-Style geschrieben wurden
- Variablen werden in {{ }} Klammern angesprochen
- Funktionen werden in {% %} Klammern ausgefuehrt
- Es koennen Macros (alias Funktionen) deklariert werden

### Verfuehgbare Funktionen
#### typeof(\%VARNAME\%)
    Gibt den Typ der Variable aus
#### printVars()
    Gibt alle bekannten Variablen aus
#### if
    If Funktion im Python-Stil.
#### end-if
    Beendet einen IF-Block
#### for
    For Schleife im Python-Stil
#### end-for
    Beendet einen For-Block
#### break
    bricht aus einer For-Schleife aus
#### continue
    Startet den naechsten Zyklus der For-Schleife


## Beispiel

<pre>
<code>
\<html>
{% import "header.html" %}

{% macro "test5(test, test1="Test HEllo world")" %}
\<h1>{{test1}}\</h1>
\<pre>
{%printVars()%}
\</pre>
{% end-macro %}
\<body>
    {% import "navbar.html" %}
    \<div>
    Type of test1: {% typeof(test1) %}
    \</div>
    \<table class="table" style="width: 50%; margin-left: auto; margin-right:auto">
        \<thead>
            \<td>ID\</td>
            \<td>Name\</td>
            \<td>link\</td>
            \<td>Admin?\</td>
        {%if ISADMIN == 1%}
            \<td>\</td>
        {%end-if%}
        \</thead>
        \<tbody>
        {%for user in username%}
        {% if user == " "%}
        {%break%}
        {%end-if%}
            \<tr>
                \<td>{{loop.i}}\</td>
                \<td>{{user}}\</td>
                \<td>\<a href="{{links[loop.i]}}">klick\</a>\</td>
                {%if ADMIN[loop.i] == 1%}
                    \<td>\<span>YES\</span>\</td>
                {%else%}
                    \<td>\<span>NO\</span>\</td>
                {%end-if%}
                {%if ISADMIN == 1%}
                    \<td>\<button class="btn btn-danger">User loeschen\</button>\</td>
                {%end-if%}
            \</tr>
        {%end-for%}
        \</tbody>
    \</table>
    {% test5(username, ADMIN) %}
\</body>
\<script>
var testvar = {{ ADMIN }};
console.log(testvar);
\</script>
\</html>
</code>
</pre>
