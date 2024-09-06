#ifndef CJSON_LEXER_H
#define CJSON_LEXER_H

#include "types.h"
#include "log.h"

unsigned int _CJSON_lexer_is_whitespace( const char c );

unsigned char _CJSON_lexer_next_string_state( const unsigned char curr_state, const char c );

unsigned int _CJSON_lexer_tokenize( const char* buf, CJSON_TOKEN* tokens, unsigned int* num_tokens );

void _CJSON_lexer_termination_pass( char* buf, CJSON_TOKEN* tokens, unsigned int num_tokens );

#endif // CJSON_LEXER_H