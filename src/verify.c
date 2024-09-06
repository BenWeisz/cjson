#include "../include/verify.h"

unsigned int _CJSON_verify_scopes( CJSON_TOKEN* tokens, unsigned int num_tokens )
{
    unsigned int token_i = 0;
    CJSON_TOKEN* token = &( tokens[token_i++] );

    if ( token->type != CJSON_TOKEN_OBJS ) return 0;

    unsigned char scope_stack[CJSON_VERIFY_SCOPE_STACK_SIZE];
    unsigned long scope_stack_top = 0;

    scope_stack[scope_stack_top++] = CJSON_VERIFY_OBJ;

    while ( token_i < num_tokens )
    {
        token = &( tokens[token_i++] );

        if ( token->type == CJSON_TOKEN_OBJS ) scope_stack[scope_stack_top++] = CJSON_VERIFY_OBJ;
        else if ( token->type == CJSON_TOKEN_ARRS ) scope_stack[scope_stack_top++] = CJSON_VERIFY_ARR;
        else if ( token->type == CJSON_TOKEN_OBJE )
        {
            if ( scope_stack_top > 0 && scope_stack[scope_stack_top - 1] == CJSON_VERIFY_OBJ ) scope_stack_top--;
            else return 0;
        }
        else if ( token->type == CJSON_TOKEN_ARRE )
        {
            if ( scope_stack_top > 0 && scope_stack[scope_stack_top - 1] == CJSON_VERIFY_ARR ) scope_stack_top--;
            else return 0;
        }
    }

    return scope_stack_top == 0;
}