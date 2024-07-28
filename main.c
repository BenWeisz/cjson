#include <stdio.h>

#include "types.h"
#include "search.h"

#define TEMP_LEN 24

int main() 
{
	struct CJSON_NODE nodes[TEMP_LEN];
	nodes[0].type = CJSON_NODE_TYPE_OBJ;
	nodes[0].buf = "{";
	nodes[0].parent = -1;

	nodes[1].type = CJSON_NODE_TYPE_KEY;
	nodes[1].buf = "test";
	nodes[1].parent = 0;

	nodes[2].type = CJSON_NODE_TYPE_KEY;
	nodes[2].buf = "apple";
	nodes[2].parent = 0;

	nodes[3].type = CJSON_NODE_TYPE_KEY;
	nodes[3].buf = "pear";
	nodes[3].parent = 0;

	nodes[4].type = CJSON_NODE_TYPE_KEY;
	nodes[4].buf = "hello";
	nodes[4].parent = 0;

	nodes[5].type = CJSON_NODE_TYPE_ARR;
	nodes[5].buf = "[";
	nodes[5].parent = 1;

	nodes[6].type = CJSON_NODE_TYPE_VALUE;
	nodes[6].buf = "1";
	nodes[6].parent = 2;

	nodes[7].type = CJSON_NODE_TYPE_OBJ;
	nodes[7].buf = "{";
	nodes[7].parent = 3;

	nodes[8].type = CJSON_NODE_TYPE_VALUE;
	nodes[8].buf = "2";
	nodes[8].parent = 4;

	nodes[9].type = CJSON_NODE_TYPE_VALUE;
	nodes[9].buf = "1";
	nodes[9].parent = 5;

	nodes[10].type = CJSON_NODE_TYPE_VALUE;
	nodes[10].buf = "2";
	nodes[10].parent = 5;

	nodes[11].type = CJSON_NODE_TYPE_VALUE;
	nodes[11].buf = "3";
	nodes[11].parent = 5;

	nodes[12].type = CJSON_NODE_TYPE_VALUE;
	nodes[12].buf = "4";
	nodes[12].parent = 5;

	nodes[13].type = CJSON_NODE_TYPE_VALUE;
	nodes[13].buf = "hello";
	nodes[13].parent = 5;

	nodes[14].type = CJSON_NODE_TYPE_KEY;
	nodes[14].buf = "one";
	nodes[14].parent = 7;

	nodes[15].type = CJSON_NODE_TYPE_KEY;
	nodes[15].buf = "two";
	nodes[15].parent = 7;

	nodes[16].type = CJSON_NODE_TYPE_KEY;
	nodes[16].buf = "three";
	nodes[16].parent = 7;

	nodes[17].type = CJSON_NODE_TYPE_VALUE;
	nodes[17].buf = "1";
	nodes[17].parent = 14;

	nodes[18].type = CJSON_NODE_TYPE_ARR;
	nodes[18].buf = "[";
	nodes[18].parent = 15;

	nodes[19].type = CJSON_NODE_TYPE_OBJ;
	nodes[19].buf = "{";
	nodes[19].parent = 16;

	nodes[20].type = CJSON_NODE_TYPE_VALUE;
	nodes[20].buf = "2";
	nodes[20].parent = 18;

	nodes[21].type = CJSON_NODE_TYPE_VALUE;
	nodes[21].buf = "3";
	nodes[21].parent = 18;

	nodes[22].type = CJSON_NODE_TYPE_OBJ;
	nodes[22].buf = "{";
	nodes[22].parent = 18;

	nodes[23].type = CJSON_NODE_TYPE_ARR;
	nodes[23].buf = "[";
	nodes[23].parent = 18;

	struct CJSON cjson;
	cjson.nodes = nodes;
	cjson.num_nodes = 24;

	struct CJSON_NODE* node = CJSON_search( &cjson, "pear", "two", "2" );
	printf("%s\n", node->buf);

	return 0;
}
