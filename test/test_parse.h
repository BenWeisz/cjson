#ifndef TEST_PARSE_H
#define TEST_PARSE_H

#include "types.h"
#include "lexer.h"
#include "parse.h"

//////////////////////////////////////////////////////
// CJSON_get_key_value_tokens
//////////////////////////////////////////////////////
FLASE_DEFINE_TEST( parse_get_key_tokens_bad_start )
{
    char* json_lit = "[]";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Tokens must start with {" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_bad_double_comma )
{
    char* json_lit = "{\"apple\": 1,,}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad double comma" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_bad_comma_colon )
{
    char* json_lit = "{\"apple\": 1,:}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad comma and colon" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_bad_comma )
{
    char* json_lit = "{\"apple\", 1,:}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad comma" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_bad_value )
{
    char* json_lit = "{\"apple\":,}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Get key failure" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_bad_double_value )
{
    char* json_lit = "{\"apple\": 1 1}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Get key success" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_single_token )
{
    char* json_lit = "{ \"test\": \"apple\" }";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    
    FLASE_ASSERT( r == 1, "Get key success" );
    FLASE_ASSERT( num_token_locations == 1, "There is one key token" );
    FLASE_ASSERT( token_locations[0] == 1, "Key in good location" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_single_token_embedded )
{
    char* json_lit = "{ \"test\": { \"test2\": 2 } }";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = CJSON_get_key_tokens( tokens, num_tokens, 3, token_locations, &num_token_locations );
    
    FLASE_ASSERT( r == 1, "Bad double value" );
    FLASE_ASSERT( num_token_locations == 1, "Single embedded key token" );
    FLASE_ASSERT( token_locations[0] == 4, "Key in good location" );
}

FLASE_DEFINE_TEST( parse_get_key_tokens_multi_token )
{
    char* json_lit = "{ \"test\": \"apple\", \"test2\": 1, \"test3\": -1233 }";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    r = CJSON_get_key_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    
    FLASE_ASSERT( r == 1, "Get key value success" ); // TODO: Same thing here
    FLASE_ASSERT( num_token_locations == 3, "Found 3 key tokens" );
    FLASE_ASSERT( token_locations[0] == 1, "Key1 in good location" );
    FLASE_ASSERT( token_locations[1] == 5, "Key2 in good location" );
    FLASE_ASSERT( token_locations[2] == 9, "Key3 in good location" );
}

//////////////////////////////////////////////////////
// CJSON_get_value_tokens
//////////////////////////////////////////////////////
FLASE_DEFINE_TEST( parse_get_value_tokens_bad_start )
{
    char* json_lit = "{}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Tokens must start with [" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_bad_double_comma )
{
    char* json_lit = "[1, ,]";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad double comma" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_bad_comma )
{
    char* json_lit = "[1,,1]";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad comma" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_bad_brackets )
{
    char* json_lit = "[[,1]";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );
    FLASE_ASSERT( r == 0, "Bad brackets" );
}

FLASE_DEFINE_TEST( parse_get_value_tokens_single_token )
{
    char* json_lit = "[1]";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_value_tokens( tokens, num_tokens, 0, token_locations, &num_token_locations );

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
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    
    unsigned int token_locations[1024];
    unsigned int num_token_locations = 1024;
    unsigned int r = CJSON_get_value_tokens( tokens, num_tokens, 6, token_locations, &num_token_locations );

    FLASE_ASSERT( r == 1, "Get value tokens succeeded" );
    FLASE_ASSERT( num_token_locations == 5, "Found all 5 tokens" );
    FLASE_ASSERT( token_locations[0] == 7, "Value1 token in right place" );
    FLASE_ASSERT( token_locations[1] == 9, "Value2 token in right place" );
    FLASE_ASSERT( token_locations[2] == 11, "Value3 token in right place" );
    FLASE_ASSERT( token_locations[3] == 13, "Value4 token in right place" );
    FLASE_ASSERT( token_locations[4] == 15, "Value5 token in right place" );
}

//////////////////////////////////////////////////////
// CJSON_parse_wrapper
//////////////////////////////////////////////////////
FLASE_DEFINE_TEST( parse__parse_single_level_object )
{
    char* json_lit = "{ \"apple\": 1, \"pear\": \"gold\", }";
    char json[1024];
    strcpy( json, json_lit );

    // Tokenize the json string
    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Parsing test, lexer passes" );

    // Setup the parsing nodes
    CJSON_NODE nodes[1024];
    unsigned int num_nodes = 1024;

    // Parse the tokens
    r = CJSON_parse_wrapper(
        nodes,
        &num_nodes,
        tokens,
        num_tokens,
        json
    );
    
    CJSON_lexer_termination_pass( json, tokens, num_tokens );

    FLASE_ASSERT( r == 1, "CJSON_parse_wrapper success" );
    FLASE_ASSERT( num_nodes == 5, "Correct number of nodes" );
    
      FLASE_ASSERT( nodes[0].type == CJSON_NODE_TYPE_OBJ, "First node is OBJ" );
      FLASE_ASSERT( nodes[0].buf[0] == '{', "_parse OBJ text matches" );
      FLASE_ASSERT( nodes[0].parent == -1, "Root node has correct parent" );

      FLASE_ASSERT( nodes[1].type == CJSON_NODE_TYPE_KEY, "First sub node is a key" );
      FLASE_ASSERT( strcmp( nodes[1].buf, "apple" ) == 0, "Apple is the first key");
    FLASE_ASSERT( nodes[1].parent == 0, "Apple is parented to the root node" );

    FLASE_ASSERT( nodes[2].type == CJSON_NODE_TYPE_KEY, "Second sub node is a key" );
    FLASE_ASSERT( strcmp( nodes[2].buf, "pear" ) == 0, "Pear is the second key" );
    FLASE_ASSERT( nodes[2].parent == 0, "Pear is parented to the root node" );

    FLASE_ASSERT( nodes[3].type == CJSON_NODE_TYPE_VALUE, "Value 1 node appears after keys" );
    FLASE_ASSERT( strcmp( nodes[3].buf, "1" ) == 0, "Value 1 has a value of 1" );
    FLASE_ASSERT( nodes[3].parent == 1, "Value 1 has a parent value of 1" );

    FLASE_ASSERT( nodes[4].type == CJSON_NODE_TYPE_VALUE, "Value 2 node appears after keys" );
    FLASE_ASSERT( strcmp( nodes[4].buf, "gold" ) == 0, "Value 1 has a value of gold" );
    FLASE_ASSERT( nodes[4].parent == 2, "Value 2 has parent value of 2" );
}

FLASE_DEFINE_TEST( parse__parse_array )
{
    char* json_lit = "{ \"arr\": [1, 2, 3, 4, 5, ], }";
    char json[1024];
    strcpy( json, json_lit );

    // Tokenize the json string
    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Parsing test, lexer passes" );

    // Setup the parsing nodes
    CJSON_NODE nodes[1024];
    unsigned int num_nodes = 1024;

    // Parse the tokens
    r = CJSON_parse_wrapper(
        nodes,
        &num_nodes,
        tokens,
        num_tokens,
        json
    );
    
    CJSON_lexer_termination_pass( json, tokens, num_tokens );

    FLASE_ASSERT( r == 1, "_parse success" );
    FLASE_ASSERT( num_nodes == 8, "Correct number of nodes" );
    
    FLASE_ASSERT( nodes[0].type == CJSON_NODE_TYPE_OBJ, "First node is OBJ" );
    FLASE_ASSERT( nodes[0].buf[0] == '{', "_parse OBJ text matches" );
    FLASE_ASSERT( nodes[0].parent == -1, "Root node has correct parent" );

    FLASE_ASSERT( nodes[1].type == CJSON_NODE_TYPE_KEY, "First sub node is a key" );
    FLASE_ASSERT( strcmp( nodes[1].buf, "arr" ) == 0, "arr is the first key");
    FLASE_ASSERT( nodes[1].parent == 0, "arr is parented to the root node" );

    FLASE_ASSERT( nodes[2].type == CJSON_NODE_TYPE_ARR, "First value is an array");
    FLASE_ASSERT( nodes[2].buf[0] == '[', "First value is array in buf" );
    FLASE_ASSERT( nodes[2].parent == 1, "Parent of array value is arr key node" );

    FLASE_ASSERT( nodes[3].type == CJSON_NODE_TYPE_VALUE, "First array value exists" );
    FLASE_ASSERT( nodes[3].buf[0] == '1', "First array value is 1" );
    FLASE_ASSERT( nodes[3].parent == 2, "First array node's parent is array node" );

    FLASE_ASSERT( nodes[4].type == CJSON_NODE_TYPE_VALUE, "Second array value exists" );
    FLASE_ASSERT( nodes[4].buf[0] == '2', "Second array value is 2" );
    FLASE_ASSERT( nodes[4].parent == 2, "Second array node's parent is array node" );

    FLASE_ASSERT( nodes[5].type == CJSON_NODE_TYPE_VALUE, "Third array value exists" );
    FLASE_ASSERT( nodes[5].buf[0] = '3', "Third array value is 3" );
    FLASE_ASSERT( nodes[5].parent == 2, "Third array node's parent is array node" );

    FLASE_ASSERT( nodes[6].type == CJSON_NODE_TYPE_VALUE, "Fourth array value exists" );
    FLASE_ASSERT( nodes[6].buf[0] == '4', "Fourth array value is 4" );
    FLASE_ASSERT( nodes[6].parent == 2, "Fourth array node's parent is array node" );

    FLASE_ASSERT( nodes[7].type == CJSON_NODE_TYPE_VALUE, "Fifth array value exists" );
    FLASE_ASSERT( nodes[7].buf[0] == '5', "Fifth array value is 5" );
    FLASE_ASSERT( nodes[7].parent, "Fifth array node's parent is a array node" );
}

FLASE_DEFINE_TEST( parse__parse_object_in_array )
{
    char* json_lit = "{ \"oina\": [ { \"val1\": 1 }, { \"val2\": null }, {} ] }";
    char json[1024];
    strcpy( json, json_lit );

    // Tokenize the json string
    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Parsing test, lexer passes" );

    // Setup the parsing nodes
    CJSON_NODE nodes[1024];
    unsigned int num_nodes = 1024;

    // Parse the tokens
    r = CJSON_parse_wrapper(
        nodes,
        &num_nodes,
        tokens,
        num_tokens,
        json
    );
    
    CJSON_lexer_termination_pass( json, tokens, num_tokens );

    FLASE_ASSERT( r == 1, "_parse success" );
    FLASE_ASSERT( num_nodes == 10, "Correct number of nodes" );

    FLASE_ASSERT( nodes[0].type == CJSON_NODE_TYPE_OBJ, "Root node exists" );
    FLASE_ASSERT( nodes[0].buf[0] == '{', "Buf is open bracket at root node" );
    FLASE_ASSERT( nodes[0].parent == -1, "This node is the root node" );

    FLASE_ASSERT( nodes[1].type == CJSON_NODE_TYPE_KEY, "First key exists" );
    FLASE_ASSERT( strcmp( nodes[1].buf, "oina" ) == 0, "Key is oina" );
    FLASE_ASSERT( nodes[1].parent == 0, "The key node's parent is the root node" );

    FLASE_ASSERT( nodes[2].type == CJSON_NODE_TYPE_ARR, "First value is an array" );
    FLASE_ASSERT( nodes[2].buf[0] == '[', "Buf at first value is an array" );
    FLASE_ASSERT( nodes[2].parent == 1, "First value's parent is a key" );

    FLASE_ASSERT( nodes[3].type == CJSON_NODE_TYPE_OBJ, "First array index is an object" );
    FLASE_ASSERT( nodes[3].buf[0] == '{', "Buf at first array value is object" );
    FLASE_ASSERT( nodes[3].parent == 2, "Parent of first array element is array node" );
    
    FLASE_ASSERT( nodes[4].type == CJSON_NODE_TYPE_OBJ, "Second array index is an object" );
    FLASE_ASSERT( nodes[4].buf[0] == '{', "Buf at second array value is object" );
    FLASE_ASSERT( nodes[4].parent == 2, "Parent of second array element is array node" );
    
    FLASE_ASSERT( nodes[5].type == CJSON_NODE_TYPE_OBJ, "Third array index is an object" );
    FLASE_ASSERT( nodes[5].buf[0] == '{', "Buf at third array value is object" );
    FLASE_ASSERT( nodes[5].parent == 2, "Parent of third array element is array node" );

    FLASE_ASSERT( nodes[6].type == CJSON_NODE_TYPE_KEY, "First element object has key" );
    FLASE_ASSERT( strcmp( nodes[6].buf, "val1" ) == 0, "Key is val1" );
    FLASE_ASSERT( nodes[6].parent == 3, "Parent of val1 key is object that is the first element of the only array" );
    
    FLASE_ASSERT( nodes[7].type == CJSON_NODE_TYPE_KEY, "Second element object has key" );
    FLASE_ASSERT( strcmp( nodes[7].buf, "val2" ) == 0, "Key is val2" );
    FLASE_ASSERT( nodes[7].parent == 4, "Parent of val2 key is object that is the second element of the only array" );

    FLASE_ASSERT( nodes[8].type == CJSON_NODE_TYPE_VALUE, "First element object has key" );
    FLASE_ASSERT( nodes[8].buf[0] == '1', "val1 value is 1" );
    FLASE_ASSERT( nodes[8].parent == 6, "1's parent is val1" );
    
    FLASE_ASSERT( nodes[9].type == CJSON_NODE_TYPE_VALUE, "First element object has key" );
    FLASE_ASSERT( strcmp( nodes[9].buf, "null" ) == 0, "Key is val2" );
    FLASE_ASSERT( nodes[9].parent == 7, "null's parent is val2" );
}

FLASE_DEFINE_TEST( parse_parse )
{
    char* json_lit = "{ \"oina\": [ { \"val1\": 1 }, { \"val2\": null }, {} ] }";
    char json[1024];
    strcpy( json, json_lit );

    // Setup the parsing nodes
    CJSON_NODE nodes[1024];
    unsigned int num_nodes = 1024;

    // Parse the tokens
    unsigned int r = CJSON_parse(
        json,
        nodes,
        &num_nodes
    );

    FLASE_ASSERT( r == 1, "Parsing passes" );
    FLASE_ASSERT( nodes[0].i == nodes[num_nodes - 1].rev_i, "i of first node and rev_i of last node matches" );
}

void test_parse_run_tests()
{
    FLASE_RUN_TEST( parse_get_key_tokens_bad_start );
    FLASE_RUN_TEST( parse_get_key_tokens_bad_double_comma );
    FLASE_RUN_TEST( parse_get_key_tokens_bad_comma_colon );
    FLASE_RUN_TEST( parse_get_key_tokens_bad_comma );
    FLASE_RUN_TEST( parse_get_key_tokens_bad_value );
    FLASE_RUN_TEST( parse_get_key_tokens_bad_double_value );
    FLASE_RUN_TEST( parse_get_key_tokens_single_token );
    FLASE_RUN_TEST( parse_get_key_tokens_single_token_embedded );
    FLASE_RUN_TEST( parse_get_key_tokens_multi_token );

    FLASE_RUN_TEST( parse_get_value_tokens_bad_start );
    FLASE_RUN_TEST( parse_get_value_tokens_bad_double_comma );
    FLASE_RUN_TEST( parse_get_value_tokens_bad_comma );
    FLASE_RUN_TEST( parse_get_value_tokens_bad_brackets );
    FLASE_RUN_TEST( parse_get_value_tokens_single_token );
    FLASE_RUN_TEST( parse_get_value_tokens_multi_token_embedded );

    FLASE_RUN_TEST( parse__parse_single_level_object );
    FLASE_RUN_TEST( parse__parse_array );
    FLASE_RUN_TEST( parse__parse_object_in_array );

    FLASE_RUN_TEST( parse_parse );
}

#endif // TEST_PARSE_H
