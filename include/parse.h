#ifndef CJSON_PARSE_H
#define CJSON_PARSE_H

#include "types.h"
#include "lexer.h"
#include "verify.h"

/* Use this for Objects */
unsigned int _CJSON_get_key_value_tokens( 
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations );

/* Use this for Arrays */
unsigned int _CJSON_get_value_tokens( 
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations );

unsigned int _CJSON_parse( 
    const char* buf,
    CJSON* cjson,
    unsigned int* curr_num_nodes, 
    const int parent_node_pos,
    CJSON_TOKEN* tokens, 
    const unsigned int num_tokens, 
    const unsigned int token_pos );

unsigned int CJSON_parse( char* buf, CJSON* cjson );
unsigned int CJSON_parse_with_settings( char* buf, CJSON* cjson, const unsigned int settings );

#endif // CJSON_PARSE_H
