#ifndef CJSON_PARSE_H
#define CJSON_PARSE_H

#include "types.h"
#include "lexer.h"
#include "verify.h"

/* Get the token indices where the key tokens are */
unsigned int _CJSON_get_key_tokens(
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations );

/* Get the token indicees of where the values are  */
unsigned int _CJSON_get_value_tokens( 
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations );

unsigned int _CJSON_parse( 
    CJSON* cjson, 
    CJSON_TOKEN* tokens, 
    const unsigned int num_tokens, 
    char* buf );

unsigned int CJSON_parse_with_settings( char* buf, CJSON* cjson, const unsigned int settings );
unsigned int CJSON_parse( char* buf, CJSON* cjson );

#endif // CJSON_PARSE_H
