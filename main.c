#include <stdio.h>
#include <string.h>

#include "types.h"
#include "search.h"
#include "lexer.h"
#include "verify.h"
#include "parse.h"

#define TEMP_LEN 24

int main() 
{
	// char* json_lit = "{\"apple\"\t\t\t\t:1,\"orange\": { \"tea\":\"green\", \"joe\": 2,}     , \"kiwi\": [1, 2, 3], }";
	// char* json_lit = "{ \"apple\": { \"tea\": []}}";
	char* json_lit = "{ \"apple\": { \"tea\": [1, 2, 3, 4]}}";
	char json[1024];
	strcpy( json, json_lit );
	
	CJSON_NODE nodes[1024];
	CJSON cjson;
	cjson.nodes = nodes;
	cjson.num_nodes = 1024;

	unsigned int r = CJSON_parse( json, &cjson );
	printf("%u\n", r);
	CJSON_NODE* node = CJSON_search( &cjson, "apple", "tea", "3" );
	if ( node != NULL )
		printf( "%s\n", node->buf );

	return 0;
}
