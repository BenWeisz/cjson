#include "../include/lexer.h"

unsigned int CJSON_lexer_is_whitespace( const char c )
{
    return c == '\t' || c == '\n' || c == '\r' || c == ' ';
}

unsigned char CJSON_lexer_next_string_state( const unsigned char curr_state, const char c )
{
    if ( curr_state == CJSON_LEXER_STRING_STATE_OUT )
    {
        if ( c == '"' ) return CJSON_LEXER_STRING_STATE_START;
        return CJSON_LEXER_STRING_STATE_OUT;
    }
    else if ( curr_state == CJSON_LEXER_STRING_STATE_START )
    {
        if ( c == '"' ) return CJSON_LEXER_STRING_STATE_END;
        else if ( c == '\\' ) return CJSON_LEXER_STRING_STATE_CTRL;
        return CJSON_LEXER_STRING_STATE_IN;
    }
    else if ( curr_state == CJSON_LEXER_STRING_STATE_IN )
    {
        if ( c == '"' ) return CJSON_LEXER_STRING_STATE_END;
        else if ( c == '\\' ) return CJSON_LEXER_STRING_STATE_CTRL;
        return CJSON_LEXER_STRING_STATE_IN;
    }
    else if ( curr_state == CJSON_LEXER_STRING_STATE_CTRL ) return CJSON_LEXER_STRING_STATE_IN;
    else if ( curr_state == CJSON_LEXER_STRING_STATE_END )
    {
        if ( c == '"' ) return CJSON_LEXER_STRING_STATE_START;
        return CJSON_LEXER_STRING_STATE_OUT;
    }
    return CJSON_LEXER_STRING_STATE_UNK;
}

unsigned int CJSON_lexer_tokenize( const char* buf, CJSON_TOKEN* tokens, unsigned int* num_tokens )
{
    unsigned int pos = 0;
    unsigned int num_tokens_used = 0;

    char c = buf[pos];
    unsigned char string_state = CJSON_LEXER_STRING_STATE_OUT;
    CJSON_TOKEN* token = &( tokens[num_tokens_used++] );
    token->start = CJSON_TOKEN_SENTINEL;
    token->end = CJSON_TOKEN_SENTINEL;
    token->type = CJSON_TOKEN_UNK;
    while ( c != '\0' && num_tokens_used < *num_tokens )
    {
        // Advance the String State
        c = buf[pos];
        string_state = CJSON_lexer_next_string_state( string_state, c );

        if ( string_state == CJSON_LEXER_STRING_STATE_START )
        {
            token->start = pos;
            token->type = CJSON_TOKEN_GET_TYPE( c );
        }
        else if ( string_state == CJSON_LEXER_STRING_STATE_END )
        {
            token->end = pos;
            token = &( tokens[num_tokens_used++] );
            token->start = CJSON_TOKEN_SENTINEL;
            token->end = CJSON_TOKEN_SENTINEL;
            token->type = CJSON_TOKEN_UNK;
        }
        else if ( string_state == CJSON_LEXER_STRING_STATE_OUT )
        {
            if ( c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',' )
            {
                if ( token->start != CJSON_TOKEN_SENTINEL )
                {
                    token->end = pos - 1;
                    token = &( tokens[num_tokens_used++] );
                }
                token->start = pos;
                token->end = pos;
                token->type = CJSON_TOKEN_GET_TYPE( c );
                token = &( tokens[num_tokens_used++] );
                token->start = CJSON_TOKEN_SENTINEL;
                token->end = CJSON_TOKEN_SENTINEL; 
                token->type = CJSON_TOKEN_UNK;
                // I don't like this, we should just memset all of these to 0xFFFFFFFF at the beginnning
                // But since thats in the inner part of the parsing code we'd have to use memset --> stdlib which is not ideal
                // We should just live with the above code or add the option for the user to hook up their own memset (thats maybe vecotrized unlike gcc's implementation)
                // But prenitializing can also be bad because the tokens array is just an estimate of the number of tokens we need
                // If we guess wrong, then we are spending a bunch of time initializing tokens structs we will never use. Thus killing the advantages of vectorizing memset
            }
            else if ( CJSON_lexer_is_whitespace( c ) == 0 )
            {
                if ( token->start == CJSON_TOKEN_SENTINEL )
                {
                    token->start = pos;
                    token->type = CJSON_TOKEN_GET_TYPE( c );
                }
            }
            else if ( token->start != CJSON_TOKEN_SENTINEL )
            {
                token->end = pos - 1;
                token = &( tokens[num_tokens_used++] );
                token->start = CJSON_TOKEN_SENTINEL;
                token->end = CJSON_TOKEN_SENTINEL; 
                token->type = CJSON_TOKEN_UNK;
            }
        }

        pos++;
    }

    if ( num_tokens_used >= *num_tokens )
    {
        CJSON_ERROR( "Ran out of tokens for use in the lexer! Please increase temp mem size macro.\n" );
        return 0;
    }

    *num_tokens = num_tokens_used - 1;
    return 1;
}

void CJSON_lexer_termination_pass( char* buf, CJSON_TOKEN* tokens, unsigned int num_tokens )
{
    for ( unsigned int token_i = 0; token_i < num_tokens; token_i++ )
    {
        CJSON_TOKEN* token = &( tokens[token_i] );
        unsigned char token_type = token->type;

        if ( token_type == CJSON_TOKEN_STR ) buf[token->end] = '\0';
        else if ( token_type == CJSON_TOKEN_NUM ||
            token_type == CJSON_TOKEN_FAL ||
            token_type == CJSON_TOKEN_TRU ||
            token_type == CJSON_TOKEN_NUL )
            buf[token->end + 1] = '\0'; 
    }
}