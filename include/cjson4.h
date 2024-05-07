// Implementation 4 ... sigh

#ifndef CJSON_H
#define CJSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CJSON_LEXER_TOKEN_LCB              0x00
#define _CJSON_LEXER_TOKEN_RCB              0x01
#define _CJSON_LEXER_TOKEN_LSB              0x02
#define _CJSON_LEXER_TOKEN_RSB              0x03
#define _CJSON_LEXER_TOKEN_COLON            0x04
#define _CJSON_LEXER_TOKEN_COMMA            0x05
#define _CJSON_LEXER_TOKEN_STR              0x06

// Refine string lexer tokens during literal verification
#define _CJSON_LEXER_TOKEN_STR_STR          0x07
#define _CJSON_LEXER_TOKEN_STR_INT          0x08
#define _CJSON_LEXER_TOKEN_STR_FLOAT        0x09
#define _CJSON_LEXER_TOKEN_STR_FALSE        0x0A
#define _CJSON_LEXER_TOKEN_STR_TRUE         0x0B
#define _CJSON_LEXER_TOKEN_STR_NULL         0x0C

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

#define _CJSON_STR_STATE_START              0x00
#define _CJSON_STR_STATE_PLAIN              0x01
#define _CJSON_STR_STATE_CTRL               0x02

#define _CJSON_NUM_STATE_SIGN               0x00
#define _CJSON_NUM_STATE_START_ZERO         0x01
#define _CJSON_NUM_STATE_START_WHOLE        0x02
#define _CJSON_NUM_STATE_START_WHOLE_CONT   0x03
#define _CJSON_NUM_STATE_POINT              0x04
#define _CJSON_NUM_STATE_MANTISSA           0x05
#define _CJSON_NUM_STATE_EXP_E              0x06
#define _CJSON_NUM_STATE_EXP_SIGN           0x07
#define _CJSON_NUM_STATE_EXP_MAG            0x08

const int               _CJSON_lexer_verify(_CJSON_LEXER_TOKEN* tokens);
const int               _CJSON_lexer_verify_scopes(_CJSON_LEXER_TOKEN* tokens);
const int               _CJSON_lexer_verify_and_augment_literals(_CJSON_LEXER_TOKEN* tokens);
const int               _CJSON_lexer_verify_is_string(_CJSON_LEXER_TOKEN* token);
const int               _CJSON_lexer_verify_is_num(_CJSON_LEXER_TOKEN* token);
const int               _CJSON_lexer_verify_is_float(_CJSON_LEXER_TOKEN* token);

#endif // CJSON
