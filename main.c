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
	// char* json_lit = "{ \"one\": { \"v1\": 1 }, \"two\": { \"v2\": 2} }";
	char* json_lit = "{ \"one\": [1, { \"apple\": [null, 1, 2, true, false, \"asdf\"]}, 3, 4, 5, 6, 7,]}";


	// TODO: ADD RINBUFFER OVERWRITE PROTECTION!!!
	// TODO: Implement searching from any cjson_node


	char json[1024];
	strcpy( json, json_lit );
	
	CJSON_NODE nodes[1024];
	CJSON cjson;
	cjson.nodes = nodes;
	cjson.num_nodes = 1024;

	//////////////////////////////////////////////////////////////////////////////////////
    // Lex the tokens
	CJSON_parse( json, &cjson );
	/////////////////////////////////////////////////////////////////////////////////////

	// unsigned int r = CJSON_parse( json, &cjson );


	// Need to ensure that we can search from any point in the nodes list
	CJSON_NODE* node = CJSON_search( &cjson, "one", "1", "apple", "1" );
	printf( "node buf: %s\n", node->buf );

	return 0;
}
