#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "search.h"
#include "lexer.h"
#include "verify.h"
#include "parse.h"

int main() 
{
    // char* json_lit = "{ \"oina\": [ { \"val1\": 1 }, { \"val2\": null }, {} ] }";
    char* json_lit = "{ \"o\": null }";
	char json[1024];
	strcpy( json, json_lit );
	
	CJSON_NODE nodes[1024];
	unsigned int num_nodes = 1024;

	unsigned int r = CJSON_parse( json, nodes, &num_nodes );

	return 0;
}
