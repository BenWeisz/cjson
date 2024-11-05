#include "../include/verify.h"

unsigned int CJSON_verify_scopes( CJSON_TOKEN* tokens, unsigned int num_tokens )
{
    unsigned int token_i = 0;
    CJSON_TOKEN* token = &( tokens[token_i++] );

    if ( token->type != CJSON_TOKEN_OBJS )
    {
        CJSON_ERROR( "The token type of the root node's first token must be an opening object type token!\n" );
        return 0;
    }

    unsigned char scope_stack[CJSON_VERIFY_SCOPE_STACK_SIZE];
    unsigned long scope_stack_top = 0;

    scope_stack[scope_stack_top++] = CJSON_VERIFY_OBJ_MARKER;

    while ( token_i < num_tokens )
    {
        token = &( tokens[token_i++] );

        if ( token->type == CJSON_TOKEN_OBJS ) scope_stack[scope_stack_top++] = CJSON_VERIFY_OBJ_MARKER;
        else if ( token->type == CJSON_TOKEN_ARRS ) scope_stack[scope_stack_top++] = CJSON_VERIFY_ARR_MARKER;
        else if ( token->type == CJSON_TOKEN_OBJE )
        {
            if ( scope_stack_top > 0 && scope_stack[scope_stack_top - 1] == CJSON_VERIFY_OBJ_MARKER ) scope_stack_top--;
            else
            {
                CJSON_ERROR( "Trying to close scope that doesn't exist OR attempting to close array scope with object closing token!\n" );
                return 0;
            }
        }
        else if ( token->type == CJSON_TOKEN_ARRE )
        {
            if ( scope_stack_top > 0 && scope_stack[scope_stack_top - 1] == CJSON_VERIFY_ARR_MARKER ) scope_stack_top--;
            else
            {
                CJSON_ERROR( "Trying to close scope that doesn't exist OR attempting to close object scope with an array closing token!\n" );
                return 0;
            }
        }
    }

    return scope_stack_top == 0;
}