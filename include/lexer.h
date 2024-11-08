#ifndef CJSON_LEXER_H
#define CJSON_LEXER_H

#include "types.h"
#include "log.h"
#include "str.h"

unsigned char CJSON_lexer_next_string_state( const unsigned char curr_state, const char c );

unsigned int CJSON_lexer_tokenize( const char* buf, CJSON_TOKEN* tokens, unsigned int* num_tokens );

void CJSON_lexer_termination_pass( char* buf, CJSON_TOKEN* tokens, unsigned int num_tokens );

#endif // CJSON_LEXER_H