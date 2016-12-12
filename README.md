## C_JSON_STREAM

This is an extremely simple library to write JSON formatted data as a stream 
without the complexity implementing an object model to represent the data. The
goal is to enable JSON output in C with reasonably minimal code and complexity.

JSON syntactical rules[1] are enforced, but at present, formatting rules for literal 
values are not. So structual constraints of JSON (objects, arrays, members, 
pairs, elements) will always be correct, but strings and numbers can be passed
that result in incorrect JSON. (Numbers should be serialized and passed as
strings.)

This library is closely patterned off the javax.json.stream JsonGenerator 
Interface. This is intended to serve the same purpose; a tool exists solely  
to easily write JSON-formatted data. [2]

This library was created because nearly all other JSON writing libaries include 
an object model and parsing, and review and acceptance will be quicker if the
code is shorter and simpler.

References: 

[1]: http://www.json.org/
[2]: http://docs.oracle.com/javaee/7/api/javax/json/stream/JsonGenerator.html

Some other C libraries that looked neat when I was searching for ones that might 
fit the need of this library are:


[JSMN minimalistic JSON parser](http://zserge.com/jsmn.html)
[Lightweight JSON Parsing in C](http://www.cis.rit.edu/~krz/hacks/jsoncvt/) 

The following library seems to do EXACTLY what this one does, but does not
appear to be in a public repository as of this writing. (12/2016)

[jWrite - a really simple JSON writer in C](https://www.codeproject.com/articles/887604/jwrite-a-really-simple-json-writer-in-c)

