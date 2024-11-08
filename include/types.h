#ifndef CJSON_TYPES_H
#define CJSON_TYPES_H

#include "config.h"

/* Lexer Types */

#define CJSON_LEXER_STRING_STATE_OUT        0x00
#define CJSON_LEXER_STRING_STATE_START      0x01
#define CJSON_LEXER_STRING_STATE_IN         0x02
#define CJSON_LEXER_STRING_STATE_CTRL       0x03
#define CJSON_LEXER_STRING_STATE_END        0x04
#define CJSON_LEXER_STRING_STATE_UNK        0x05

#define CJSON_TOKEN_SENTINEL        0xFFFFFFFF
#define CJSON_TOKEN_STR             0x00
#define CJSON_TOKEN_NUM             0x01
#define CJSON_TOKEN_FAL             0x02
#define CJSON_TOKEN_TRU             0x03
#define CJSON_TOKEN_NUL             0x04
#define CJSON_TOKEN_OBJS            0x05
#define CJSON_TOKEN_OBJE            0x06
#define CJSON_TOKEN_ARRS            0x07
#define CJSON_TOKEN_ARRE            0x08
#define CJSON_TOKEN_COL             0x09
#define CJSON_TOKEN_COM             0x0A
#define CJSON_TOKEN_UNK             0x0B

#define CJSON_TOKEN_GET_TYPE( c ) \
    ({ \
        unsigned char r = CJSON_TOKEN_UNK; \
        if ( (c) == '"' ) r = CJSON_TOKEN_STR; \
        else if ( (c) == '-' || ( (c) >= '0' && (c) <= '9' ) ) r = CJSON_TOKEN_NUM; \
        else if ( (c) == 'f' ) r = CJSON_TOKEN_FAL; \
        else if ( (c) == 't' ) r = CJSON_TOKEN_TRU; \
        else if ( (c) == 'n' ) r = CJSON_TOKEN_NUL; \
        else if ( (c) == '{' ) r = CJSON_TOKEN_OBJS; \
        else if ( (c) == '}' ) r = CJSON_TOKEN_OBJE; \
        else if ( (c) == '[' ) r = CJSON_TOKEN_ARRS; \
        else if ( (c) == ']' ) r = CJSON_TOKEN_ARRE; \
        else if ( (c) == ':' ) r = CJSON_TOKEN_COL; \
        else if ( (c) == ',' ) r = CJSON_TOKEN_COM; \
        r; \
    })

typedef struct CJSON_TOKEN
{
    unsigned int start;
    unsigned int end;
    unsigned char type;
} CJSON_TOKEN;

/* Verify Types */

#define CJSON_VERIFY_OBJ_MARKER 0x00
#define CJSON_VERIFY_ARR_MARKER 0x01

#define CJSON_NODE_TYPE_OBJ     0x00
#define CJSON_NODE_TYPE_ARR     0x01
#define CJSON_NODE_TYPE_KEY     0x02
#define CJSON_NODE_TYPE_VALUE   0x03

/* Parse Types */

typedef struct CJSON_NODE
{
    unsigned char type;
    char* buf;
    int parent;
    unsigned int i;
    unsigned int rev_i; // Reverse index
} CJSON_NODE;

#define CJSON_PARSE_KV_STATE_KEY 0x00
#define CJSON_PARSE_KV_STATE_COL 0x01
#define CJSON_PARSE_KV_STATE_VAL 0x02
#define CJSON_PARSE_KV_STATE_COM 0x03

#define CJSON_PARSE_QUEUE_OTHER_FLAG    0x00
#define CJSON_PARSE_QUEUE_KEY_FLAG      0x01

typedef struct CJSON_PARSE_QUEUE_ELEMENT
{
    unsigned int token_location;
    unsigned char token_key_flag;
    int token_parent;
} CJSON_PARSE_QUEUE_ELEMENT;

#define CJSON_PARSE_SCOPE_CHECKING      0x01

/* Error Types */

#define CJSON_FAILURE                   0x00
#define CJSON_SUCCESS                   0x01

/* CJSON_NULL */
#define CJSON_NULL                      0x00

#endif // CJSON_TYPES_H
