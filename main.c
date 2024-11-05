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
	// char* json_lit = "{ \"one\": [1, { \"apple\": [null, 1, 2, true, false, \"asdf\"]}, 3, 4, 5, 6, 7,]}";
	char* json_lit = "{ \"one\": [1, { \"apple\": [null, 1, 2, true, false, \"asdf\"]}, 3, 4, 5, 6, 7,]}";


	// TODO: ADD RINGBUFFER OVERWRITE PROTECTION!!!
	// TODO: Add return types for success / errors
	// TODO: Search for all other TODOs
	// TODO: Remove all STDLIB includes, swap nulls to NULL and add custom implementation of STR functions
	// TODO: Add option to print errors or not

	// TODO: STD LIBS
	// -	We want stdlibs to be one by default and logging errors to be on by default --> but can be turned off 

	// To search from a node we need a couple of things:
	// all nodes, current node, reference node


	char json[1024];
	strcpy( json, json_lit );
	
	CJSON_NODE nodes[1024];
	unsigned int num_nodes = 1024;

	//////////////////////////////////////////////////////////////////////////////////////
    // Lex the tokens
	CJSON_parse( json, nodes, &num_nodes );
	/////////////////////////////////////////////////////////////////////////////////////

	// unsigned int r = CJSON_parse( json, &cjson );


	// Need to ensure that we can search from any point in the nodes list
	CJSON_NODE* arr_node = CJSON_search( nodes, "one", "1" );
	CJSON_NODE* one = CJSON_search( arr_node, "apple", "5" );

	if ( one == NULL )
		printf("NULL\n");
	else
		printf( "one: %s\n", one->buf );

	return 0;
}
