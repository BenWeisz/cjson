#ifndef TEST_SEARCH_H
#define TEST_SEARCH_H

#include <string.h>

#include "types.h"
#include "search.h"

#include "flase.h"

FLASE_DEFINE_TEST( search_simple )
{
	struct CJSON_NODE nodes[3];
	nodes[0].type = CJSON_NODE_TYPE_OBJ;
	nodes[0].buf = "{";
	nodes[0].parent = -1;
	
	nodes[1].type = CJSON_NODE_TYPE_KEY;
	nodes[1].buf = "key1";
	nodes[1].parent = 0;

	nodes[2].type = CJSON_NODE_TYPE_VALUE;
	nodes[2].buf = "0";
	nodes[2].parent = 1;

	struct CJSON cjson;
	cjson.nodes = nodes;
	cjson.num_nodes = 3;

	struct CJSON_NODE* node = CJSON_search( &cjson, "key1" );

	FLASE_ASSERT( *node->buf == '0', "key1 is value 0" );
}

FLASE_DEFINE_TEST( search_nested_obj )
{
	struct CJSON_NODE nodes[7];
	nodes[0].type = CJSON_NODE_TYPE_OBJ;
	nodes[0].buf = "{";
	nodes[0].parent = -1;

	nodes[1].type = CJSON_NODE_TYPE_KEY;
	nodes[1].buf = "key1";
	nodes[1].parent = 0;

	nodes[2].type = CJSON_NODE_TYPE_OBJ;
	nodes[2].buf = "{";
	nodes[2].parent = 1;

	nodes[3].type = CJSON_NODE_TYPE_KEY;
	nodes[3].buf = "key2";
	nodes[3].parent = 2;

	nodes[4].type = CJSON_NODE_TYPE_OBJ;
	nodes[4].buf = "{";
	nodes[4].parent = 3;

	nodes[5].type = CJSON_NODE_TYPE_KEY;
	nodes[5].buf = "key3";
	nodes[5].parent = 4;

	nodes[6].type = CJSON_NODE_TYPE_VALUE;
	nodes[6].buf = "I'm nested";
	nodes[6].parent = 5;

	struct CJSON cjson;
	cjson.nodes = nodes;
	cjson.num_nodes = 7;

	struct CJSON_NODE* node;

	node = CJSON_search( &cjson, "key1", "key2", "key3" );
	FLASE_ASSERT( strcmp( node->buf, "I'm nested" ) == 0, "Search for nested value (object)" );

	node = CJSON_search( &cjson, "key1123", "key2" );
	FLASE_ASSERT( node == NULL, "Early out on bad key" );

	node = CJSON_search( &cjson, "key1", "key2123", "key3" );
	FLASE_ASSERT( node == NULL, "Mid out on bad key" );
}

FLASE_DEFINE_TEST( search_nested_arr )
{
		struct CJSON_NODE nodes[7];
	nodes[0].type = CJSON_NODE_TYPE_OBJ;
	nodes[0].buf = "{";
	nodes[0].parent = -1;

	nodes[1].type = CJSON_NODE_TYPE_KEY;
	nodes[1].buf = "key1";
	nodes[1].parent = 0;

	nodes[2].type = CJSON_NODE_TYPE_ARR;
	nodes[2].buf = "[";
	nodes[2].parent = 1;

	nodes[3].type = CJSON_NODE_TYPE_ARR;
	nodes[3].buf = "[";
	nodes[3].parent = 2;

	nodes[4].type = CJSON_NODE_TYPE_ARR;
	nodes[4].buf = "[";
	nodes[4].parent = 3;

	nodes[5].type = CJSON_NODE_TYPE_ARR;
	nodes[5].buf = "[";
	nodes[5].parent = 4;

	nodes[6].type = CJSON_NODE_TYPE_VALUE;
	nodes[6].buf = "I'm nested";
	nodes[6].parent = 5;

	struct CJSON cjson;
	cjson.nodes = nodes;
	cjson.num_nodes = 7;

	struct CJSON_NODE* node;

	node = CJSON_search( &cjson, "key1", "0", "0", "0", "0" );
	FLASE_ASSERT( strcmp( node->buf, "I'm nested" ) == 0, "Search for nested value (array)" );

	node = CJSON_search( &cjson, "asdf" );
	FLASE_ASSERT( node == NULL, "Early out on nested arrays" );

	node = CJSON_search( &cjson, "key1", "0", "10", "1" );
	FLASE_ASSERT( node == NULL, "Out on bad middle index" );

	node = CJSON_search( &cjson, "key1", "-1112123111", "10", "1" );
	FLASE_ASSERT( node == NULL, "Out on negative key" );
}

FLASE_DEFINE_TEST( search_mixed_nested )
{
	struct CJSON_NODE nodes[6];
	nodes[0].type = CJSON_NODE_TYPE_OBJ;
	nodes[0].buf = "{";
	nodes[0].parent = -1;

	nodes[1].type = CJSON_NODE_TYPE_KEY;
	nodes[1].buf = "key1";
	nodes[1].parent = 0;

	nodes[2].type = CJSON_NODE_TYPE_ARR;
	nodes[2].buf = "[";
	nodes[2].parent = 1;

	nodes[3].type = CJSON_NODE_TYPE_OBJ;
	nodes[3].buf = "{";
	nodes[3].parent = 2;

	nodes[4].type = CJSON_NODE_TYPE_KEY;
	nodes[4].buf = "key2";
	nodes[4].parent = 3;

	nodes[5].type = CJSON_NODE_TYPE_VALUE;
	nodes[5].buf = "I'm nested";
	nodes[5].parent = 4;

	struct CJSON cjson;
	cjson.nodes = nodes;
	cjson.num_nodes = 6;

	struct CJSON_NODE* node;

	node = CJSON_search( &cjson, "key1", "0", "key2" );
	FLASE_ASSERT( strcmp( node->buf, "I'm nested" ) == 0, "Search for mixed nested value (array)" );

	node = CJSON_search( &cjson, "key1", "0", "key2", "extra value" );
	FLASE_ASSERT( node == NULL, "Return null on extra keys / indices" );	
}

void test_search_run_tests()
{
	FLASE_RUN_TEST( search_simple );
	FLASE_RUN_TEST( search_nested_obj );
	FLASE_RUN_TEST( search_nested_arr );
	FLASE_RUN_TEST( search_mixed_nested );
}

#endif // TEST_SEARCH_H
