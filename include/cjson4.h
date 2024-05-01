// Implementation 4 ... sigh

#ifndef CJSON_H
#define CJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CJSON_LEXER_TOKEN_LCB      0x00
#define _CJSON_LEXER_TOKEN_RCB      0x01
#define _CJSON_LEXER_TOKEN_LSB      0x02
#define _CJSON_LEXER_TOKEN_RSB      0x03
#define _CJSON_LEXER_TOKEN_COLON    0x04
#define _CJSON_LEXER_TOKEN_COMMA    0x05
#define _CJSON_LEXER_TOKEN_STR      0x06

typedef struct _CJSON_LEXER_TOKEN
{
    char type;
    const char* text;
    struct _CJSON_LEXER_TOKEN* next;
} _CJSON_LEXER_TOKEN;

_CJSON_LEXER_TOKEN*     _CJSON_lexer_lex_tokens(const char* buf);
void                    _CJSON_lexer_eat_whitespace(const char* buf, unsigned long* buf_pos);
const char*             _CJSON_lexer_eat_str(const char* buf, unsigned long* buf_pos);
void                    _CJSON_lexer_free_tokens(_CJSON_LEXER_TOKEN* tokens);
void                    _CJSON_lexer_init_token(_CJSON_LEXER_TOKEN* token, char type, const char* text);

// _CJSON_lexer_verify();
// _CJSON_lexer_verify_scopes();
// _CJSON_lexer_verify_syntax();

#endif // CJSON
