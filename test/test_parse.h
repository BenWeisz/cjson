#ifndef TEST_PARSE_H
#define TEST_PARSE_H

#include "types.h"
#include "lexer.h"
#include "parse.h"

//////////////////////////////////////////////////////
// _CJSON_get_key_value_tokens
//////////////////////////////////////////////////////
FLASE_DEFINE_TEST( parse_get_key_value_tokens_bad_start )
{
    char* json_lit = "[]";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Tokens must start with {" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_bad_double_comma )
{
    char* json_lit = "{\"apple\": 1,,}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad double comma" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_bad_comma_colon )
{
    char* json_lit = "{\"apple\": 1,:}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad comma and colon" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_bad_comma )
{
    char* json_lit = "{\"apple\", 1,:}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad comma" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_bad_value )
{
    char* json_lit = "{\"apple\":,}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Get key value failure" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_bad_double_value )
{
    char* json_lit = "{\"apple\": 1 1}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Get key value success" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_single_token )
{
    char* json_lit = "{ \"test\": \"apple\" }";
    char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    
    FLASE_ASSERT( r == 1, "Get key value success" );
    FLASE_ASSERT( num_token_locations == 2, "Found singular token" );
    FLASE_ASSERT( token_locations[0] == 1, "Key in good location" );
    FLASE_ASSERT( token_locations[1] == 3, "Value in good location" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_single_token_embedded )
{
    char* json_lit = "{ \"test\": { \"test2\": 2 } }";
    char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = _CJSON_get_key_value_tokens( tokens, num_tokens, 3, token_locations, &num_token_locations );
    
    FLASE_ASSERT( r == 1, "Bad double value" );
    FLASE_ASSERT( num_token_locations == 2, "Found singular token" );
    FLASE_ASSERT( token_locations[0] == 4, "Key in good location" );
    FLASE_ASSERT( token_locations[1] == 6, "Value in good location" );
}

FLASE_DEFINE_TEST( parse_get_key_value_tokens_multi_token )
{
    char* json_lit = "{ \"test\": \"apple\", \"test2\": 1, \"test3\": -1233 }";
    char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = _CJSON_get_key_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    
    FLASE_ASSERT( r == 1, "Get key value success" );
    FLASE_ASSERT( num_token_locations == 6, "Found multi tokens" );
    FLASE_ASSERT( token_locations[0] == 1, "Key1 in good location" );
    FLASE_ASSERT( token_locations[1] == 3, "Value1 in good location" );
    FLASE_ASSERT( token_locations[2] == 5, "Key2 in good location" );
    FLASE_ASSERT( token_locations[3] == 7, "Value2 in good location" );
    FLASE_ASSERT( token_locations[4] == 9, "Key3 in good location" );
    FLASE_ASSERT( token_locations[5] == 11, "Value3 in good location" );
}

//////////////////////////////////////////////////////
// _CJSON_get_value_tokens
//////////////////////////////////////////////////////
FLASE_DEFINE_TEST( parse_get_value_tokens_bad_start )
{
    char* json_lit = "{}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Tokens must start with [" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_bad_double_comma )
{
    char* json_lit = "[1, ,]";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad double comma" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_bad_comma )
{
    char* json_lit = "[1,,1]";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad comma" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_bad_brackets )
{
    char* json_lit = "[[,1]";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad brackets" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_single_token )
{
    char* json_lit = "[1]";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );

    FLASE_ASSERT( r == 1, "Get value tokens succeeded" );
    FLASE_ASSERT( num_token_locations == 1, "Found only 1 token" );
    FLASE_ASSERT( token_locations[0] == 1, "Value token in right place" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_multi_token_embedded )
{
    char* json_lit = "{ \"apple\": { \"tea\": [1, 2, -123223, 4, \"test\"]}}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = _CJSON_get_value_tokens( tokens, num_tokens, 6, token_locations, &num_token_locations );

    FLASE_ASSERT( r == 1, "Get value tokens succeeded" );
    FLASE_ASSERT( num_token_locations == 5, "Found all 5 tokens" );
    FLASE_ASSERT( token_locations[0] == 7, "Value1 token in right place" );
    FLASE_ASSERT( token_locations[1] == 9, "Value2 token in right place" );
    FLASE_ASSERT( token_locations[2] == 11, "Value3 token in right place" );
    FLASE_ASSERT( token_locations[3] == 13, "Value4 token in right place" );
    FLASE_ASSERT( token_locations[4] == 15, "Value5 token in right place" );
}

FLASE_DEFINE_TEST( parse__parse_single_level_object )
{
    char* json_lit = "{ \"apple\": 1, \"pear\": \"gold\", }";
	char json[1024];
	strcpy( json, json_lit );

	// Tokenize the json string
    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    unsigned int r = _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Parsing test, lexer passes" );

	// Setup the parsing nodes
    CJSON_NODE nodes[1024];
    CJSON cjson;
    cjson.nodes = nodes;
    cjson.num_nodes = 1024;

    // Parse the tokens
    unsigned int num_nodes = 0;
    r = _CJSON_parse(
        json,
        &cjson,
        &num_nodes,
        -1,
        tokens,
        num_tokens,
        0
    );
    
	_CJSON_lexer_termination_pass( json, tokens, num_tokens );

    FLASE_ASSERT( r == 1, "_parse success" );
	FLASE_ASSERT( num_nodes == 5, "Correct number of nodes" );
	
  	FLASE_ASSERT( cjson.nodes[0].type == CJSON_NODE_TYPE_OBJ, "First node is OBJ" );
  	FLASE_ASSERT( cjson.nodes[0].buf[0] == '{', "_parse OBJ text matches" );
  	FLASE_ASSERT( cjson.nodes[0].parent == -1, "Root node has correct parent" );

  	FLASE_ASSERT( cjson.nodes[1].type == CJSON_NODE_TYPE_KEY, "First sub node is a key" );
  	FLASE_ASSERT( strcmp( cjson.nodes[1].buf, "apple" ) == 0, "Apple is the first key");
	FLASE_ASSERT( cjson.nodes[1].parent == 0, "Apple is parented to the root node" );

	FLASE_ASSERT( cjson.nodes[2].type == CJSON_NODE_TYPE_KEY, "Second sub node is a key" );
	FLASE_ASSERT( strcmp( cjson.nodes[2].buf, "pear" ) == 0, "Pear is the second key" );
	FLASE_ASSERT( cjson.nodes[2].parent == 0, "Pear is parented to the root node" );

	FLASE_ASSERT( cjson.nodes[3].type == CJSON_NODE_TYPE_VALUE, "Value 1 node appears after keys" );
	FLASE_ASSERT( strcmp( cjson.nodes[3].buf, "1" ) == 0, "Value 1 has a value of 1" );
	FLASE_ASSERT( cjson.nodes[3].parent == 1, "Value 1 has a parent value of 1" );

	FLASE_ASSERT( cjson.nodes[4].type == CJSON_NODE_TYPE_VALUE, "Value 2 node appears after keys" );
	FLASE_ASSERT( strcmp( cjson.nodes[4].buf, "gold" ) == 0, "Value 1 has a value of gold" );
	FLASE_ASSERT( cjson.nodes[4].parent == 2, "Value 2 has parent value of 2" );
}

FLASE_DEFINE_TEST( parse__parse_array )
{
    char* json_lit = "{ \"arr\": [1, 2, 3, 4, 5, ], }";
	char json[1024];
	strcpy( json, json_lit );

	// Tokenize the json string
    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    unsigned int r = _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Parsing test, lexer passes" );

	// Setup the parsing nodes
    CJSON_NODE nodes[1024];
    CJSON cjson;
    cjson.nodes = nodes;
    cjson.num_nodes = 1024;

    // Parse the tokens
    unsigned int num_nodes = 0;
    r = _CJSON_parse(
        json,
        &cjson,
        &num_nodes,
        -1,
        tokens,
        num_tokens,
        0
    );
    
	_CJSON_lexer_termination_pass( json, tokens, num_tokens );

    FLASE_ASSERT( r == 1, "_parse success" );
	FLASE_ASSERT( num_nodes == 8, "Correct number of nodes" );
	
  	FLASE_ASSERT( cjson.nodes[0].type == CJSON_NODE_TYPE_OBJ, "First node is OBJ" );
  	FLASE_ASSERT( cjson.nodes[0].buf[0] == '{', "_parse OBJ text matches" );
  	FLASE_ASSERT( cjson.nodes[0].parent == -1, "Root node has correct parent" );

  	FLASE_ASSERT( cjson.nodes[1].type == CJSON_NODE_TYPE_KEY, "First sub node is a key" );
  	FLASE_ASSERT( strcmp( cjson.nodes[1].buf, "arr" ) == 0, "arr is the first key");
	FLASE_ASSERT( cjson.nodes[1].parent == 0, "arr is parented to the root node" );

	FLASE_ASSERT( cjson.nodes[2].type == CJSON_NODE_TYPE_ARR, "First value is an array");
	FLASE_ASSERT( cjson.nodes[2].buf[0] == '[', "First value is array in buf" );
	FLASE_ASSERT( cjson.nodes[2].parent == 1, "Parent of array value is arr key node" );

	FLASE_ASSERT( cjson.nodes[3].type == CJSON_NODE_TYPE_VALUE, "First array value exists" );
	FLASE_ASSERT( cjson.nodes[3].buf[0] == '1', "First array value is 1" );
	FLASE_ASSERT( cjson.nodes[3].parent == 2, "First array node's parent is array node" );

	FLASE_ASSERT( cjson.nodes[4].type == CJSON_NODE_TYPE_VALUE, "Second array value exists" );
	FLASE_ASSERT( cjson.nodes[4].buf[0] == '2', "Second array value is 2" );
	FLASE_ASSERT( cjson.nodes[4].parent == 2, "Second array node's parent is array node" );

	FLASE_ASSERT( cjson.nodes[5].type == CJSON_NODE_TYPE_VALUE, "Third array value exists" );
	FLASE_ASSERT( cjson.nodes[5].buf[0] = '3', "Third array value is 3" );
	FLASE_ASSERT( cjson.nodes[5].parent == 2, "Third array node's parent is array node" );

	FLASE_ASSERT( cjson.nodes[6].type == CJSON_NODE_TYPE_VALUE, "Fourth array value exists" );
	FLASE_ASSERT( cjson.nodes[6].buf[0] == '4', "Fourth array value is 4" );
	FLASE_ASSERT( cjson.nodes[6].parent == 2, "Fourth array node's parent is array node" );

	FLASE_ASSERT( cjson.nodes[7].type == CJSON_NODE_TYPE_VALUE, "Fifth array value exists" );
	FLASE_ASSERT( cjson.nodes[7].buf[0] == '5', "Fifth array value is 5" );
	FLASE_ASSERT( cjson.nodes[7].parent, "Fifth array node's parent is a array node" );
}

FLASE_DEFINE_TEST( parse__parse_object_in_array )
{
    char* json_lit = "{ \"oina\": [ { \"val1\": 1 }, { \"val2\": null }, {} ] }";
	char json[1024];
	strcpy( json, json_lit );

	// Tokenize the json string
    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    unsigned int r = _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Parsing test, lexer passes" );

	// Setup the parsing nodes
    CJSON_NODE nodes[1024];
    CJSON cjson;
    cjson.nodes = nodes;
    cjson.num_nodes = 1024;

    // Parse the tokens
    unsigned int num_nodes = 0;
    r = _CJSON_parse(
        json,
        &cjson,
        &num_nodes,
        -1,
        tokens,
        num_tokens,
        0
    );
    
	_CJSON_lexer_termination_pass( json, tokens, num_tokens );

    FLASE_ASSERT( r == 1, "_parse success" );
	FLASE_ASSERT( num_nodes == 10, "Correct number of nodes" );

	FLASE_ASSERT( cjson.nodes[0].type == CJSON_NODE_TYPE_OBJ, "Root node exists" );
	FLASE_ASSERT( cjson.nodes[0].buf[0] == '{', "Buf is open bracket at root node" );
	FLASE_ASSERT( cjson.nodes[0].parent == -1, "This node is the root node" );

	FLASE_ASSERT( cjson.nodes[1].type == CJSON_NODE_TYPE_KEY, "First key exists" );
	FLASE_ASSERT( strcmp( cjson.nodes[1].buf, "oina" ) == 0, "Key is oina" );
	FLASE_ASSERT( cjson.nodes[1].parent == 0, "The key node's parent is the root node" );

	FLASE_ASSERT( cjson.nodes[2].type == CJSON_NODE_TYPE_ARR, "First value is an array" );
	FLASE_ASSERT( cjson.nodes[2].buf[0] == '[', "Buf at first value is an array" );
	FLASE_ASSERT( cjson.nodes[2].parent == 1, "First value's parent is a key" );

	FLASE_ASSERT( cjson.nodes[3].type == CJSON_NODE_TYPE_OBJ, "First array index is an object" );
	FLASE_ASSERT( cjson.nodes[3].buf[0] == '{', "Buf at first array value is object" );
	FLASE_ASSERT( cjson.nodes[3].parent == 2, "Parent of first array element is array node" );

	// FLASE_ASS
}

void test_parse_run_tests()
{
    FLASE_RUN_TEST( parse_get_key_value_tokens_bad_start );
    FLASE_RUN_TEST( parse_get_key_value_tokens_bad_double_comma );
    FLASE_RUN_TEST( parse_get_key_value_tokens_bad_comma_colon );
    FLASE_RUN_TEST( parse_get_key_value_tokens_bad_comma );
    FLASE_RUN_TEST( parse_get_key_value_tokens_bad_value );
    FLASE_RUN_TEST( parse_get_key_value_tokens_bad_double_value );
    FLASE_RUN_TEST( parse_get_key_value_tokens_single_token );
    FLASE_RUN_TEST( parse_get_key_value_tokens_single_token_embedded );
    FLASE_RUN_TEST( parse_get_key_value_tokens_multi_token );

    FLASE_RUN_TEST( parse_get_value_tokens_bad_start );
    FLASE_RUN_TEST( parse_get_value_tokens_bad_double_comma );
    FLASE_RUN_TEST( parse_get_value_tokens_bad_comma );
    FLASE_RUN_TEST( parse_get_value_tokens_bad_brackets );
    FLASE_RUN_TEST( parse_get_value_tokens_single_token );
    FLASE_RUN_TEST( parse_get_value_tokens_multi_token_embedded );

    FLASE_RUN_TEST( parse__parse_single_level_object );
    FLASE_RUN_TEST( parse__parse_array );
    FLASE_RUN_TEST( parse__parse_object_in_array );
}

#endif // TEST_PARSE_H