# cjson

Welcome to ***cjson***, I know, I know, that is quite the unoriginal name for a C library that parses JSON (actually just a subset of it). It's especially a bad name because another more well known JSON parsing library for C already exists with that name ... but here we are. 

I've take some inspiration from [jsmn](https://github.com/zserge/jsmn) in terms of trying to keep the library as simple as possible. That has some consequences:
- No support for writing JSON
- No support for non ascii characters
- No value parsing (ie. all values are strings, don't worry you'll see what I mean later)

You might be asking what good is this library then?
- It doesn't require stdlib
- In place value string termination so we don't go off allocating extra space for strings
- The parsed nodes are layed out linearly so we don't have to go chasing after points
- Simple search interface
- The library is a single header

# Integration

Let's dive into how you might integrate this library into your project. Once this project is done, there will only be two files in this branch. 

1) This README
2) A file called `cjson.h`

All you'll need to do is copy this file to whatever project you are using and create a `cjson.c` file with the following contents:

```C
#define CJSON_IMPLEMENTATION
#include "cjson.h"
```

When you build your project this file will hold all of the implementations of the ***cjson*** library. From that point on, all you have to do is include `cjson.h` in the files you want to use it in and you're ready to go!

# Usage

Below we show code samples for parsing json and searching parsed json text.

## Parsing
The most important thing when parsing is making sure that your string array is ***non-const***. This is important because ***cjson*** will add in null terminators to cap off your tokens. This will give you an easier time parsing out your values.

Below is some sample code for how you might parse out your JSON data:

```C
#include "cjson.h"

char* json = ... // You're data loading method

CJSON_NODE nodes[1024];
unsigned int num_nodes = 1024;

unsigned int r = CJSON_parse( json, nodes, &num_nodes );

if ( r == CJSON_SUCCESS )
{
    // Success
}
else {
    // CJSON_FAILURE
}
```

It is crucial that you set the `num_nodes` variable to the number of nodes that you've allocated on the stack / heap. This value will be updated to reflect the number of `CJSON_NODE` elements that were used to parse the data you provided.

## Searching

Once you've parsed a bit of json, you'll probably want to extract your data. This is done using the `CJSON_search` search interface.

```C
// json = { "apple": [ 10, null, true, false, { "pear": -1 } ] }

CJSON_NODE nodes[1024];
unsigned int num_nodes = 1024;

CJSON_parse( json, nodes, &num_nodes );

// If "nodes" has an "apple" key the apple variable will 
// hold a reference to that node from the nodes variable
CJSON_NODE* apple = CJSON_search( nodes, "apple" );

// You can then search for nodes within the apple node, for example the 0th index
CJSON_NODE* zeroith = CJSON_search( apple, "0" );

// The value can then be extracted from the node through the
// "buf" variable in the CJSON_NODE
zeroith->buf; // This is a cstring that holds "10"

// You can also extract nodes directly instead of searching for nodes one by one
CJSON_NODE* pear = CJSON_search( nodes, "apple", "4", "pear" );

// The pear variable will now be a CJSON_NODE with a buf pointing to the cstring "-1"

// If somewhere along the way you access a key that 
// doesnt exist, the search API will return NULL
CJSON_NODE* pear_attempt = CJSON_search( nodes, "apppppppleeee", "4", "pear" );

// pear_attempt will be NULL here
```

# Contributing

If you want to contribute to this project, you probably don't want to be making changes to `cjson.h` in the main branch because you will have an aneurysm.
Thats because once all is said and done, this branch will only have this README and and single header bundle file called `cjson.h`.

It's recommended that you switch to the `dev` branch and make changes there, building and testing along the way to make sure you don't have any errors.
Once we've integrated your changes and make sure everything runs fine I'll use [re-package](https://github.com/BenWeisz/re-package) to bundle everything into a single header and I'll push it to the main branch.