#ifndef TEST_LEXER_H
#define TEST_LEXER_H

#include <string.h>

#include "types.h"
#include "lexer.h"

#include "flase.h"

FLASE_DEFINE_TEST( lexer_is_whitespace )
{
    FLASE_ASSERT( CJSON_lexer_is_whitespace( ' ' ) == 1, "Space is whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '\t' ) == 1, "Tab is whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '\r' ) == 1, "\\r is whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '\n' ) == 1, "\\n is whitespace" );

    FLASE_ASSERT( CJSON_lexer_is_whitespace( '"' ) == 0, "\" is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( 't' ) == 0, "t is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( 'f' ) == 0, "f is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( 'n' ) == 0, "n is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '{' ) == 0, "{ is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '}' ) == 0, "} is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '[' ) == 0, "[ is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( ']' ) == 0, "] is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '-' ) == 0, "- is not whitespace" );
    FLASE_ASSERT( CJSON_lexer_is_whitespace( '0' ) == 0, "0 is not whitespace" );
}

FLASE_DEFINE_TEST( lexer_next_string_state )
{
    // Out state
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_OUT, ' ' ) == CJSON_LEXER_STRING_STATE_OUT, "Space doesn't change out state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_OUT, '\t' ) == CJSON_LEXER_STRING_STATE_OUT, "Tab doesn't change out state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_OUT, '\r' ) == CJSON_LEXER_STRING_STATE_OUT, "\r doesn't change out state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_OUT, '\n' ) == CJSON_LEXER_STRING_STATE_OUT, "\n doesn't change out state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_OUT, '"' ) == CJSON_LEXER_STRING_STATE_START, "\" leads to start state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_OUT, '\\' ) == CJSON_LEXER_STRING_STATE_OUT, "\\ leads to out state" );

    // Start state
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_START, '"' ) == CJSON_LEXER_STRING_STATE_END, "\" leads to end state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_START, ' ' ) == CJSON_LEXER_STRING_STATE_IN, "Space leads to in state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_START, '\t' ) == CJSON_LEXER_STRING_STATE_IN, "Tab leads to in state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_START, 'c' ) == CJSON_LEXER_STRING_STATE_IN, "Character leads to in state" );

    // In state
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_IN, 'c' ) == CJSON_LEXER_STRING_STATE_IN, "Character leads to in state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_IN, '\\' ) == CJSON_LEXER_STRING_STATE_CTRL, "\\ leads to control state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_IN, '"' ) == CJSON_LEXER_STRING_STATE_END, "\" leads to end state" );

    // Control state
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_CTRL, '"' ) == CJSON_LEXER_STRING_STATE_IN, "\" leads to control state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_CTRL, '\\' ) == CJSON_LEXER_STRING_STATE_IN, "\\ leads to control state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_CTRL, 'c' ) == CJSON_LEXER_STRING_STATE_IN, "Character leads to in state" );

    // End state
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_END, 'c' ) == CJSON_LEXER_STRING_STATE_OUT, "Character leads to out state" );
    FLASE_ASSERT( CJSON_lexer_next_string_state( CJSON_LEXER_STRING_STATE_END, '"' ) == CJSON_LEXER_STRING_STATE_START, "\" leads to start state" );
}

FLASE_DEFINE_TEST( lexer_tokenize_valid )
{
    char* json_lit = "{ \"apple\": { \"tea\": [1, 2, -123223, 4, \"test\"]}}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );

    FLASE_ASSERT( r == 1, "Tokenizer passes" );
    FLASE_ASSERT( num_tokens == 19, "Tokenizer recorded correct number of tokens");

    FLASE_ASSERT( tokens[0].start == 0 && tokens[0].end == 0, "Token 0 has correct start and end" );
    FLASE_ASSERT( tokens[1].start == 2 && tokens[1].end == 8, "Token 1 has correct start and end" );
    FLASE_ASSERT( tokens[2].start == 9 && tokens[2].end == 9, "Token 2 has correct start and end" );
    FLASE_ASSERT( tokens[3].start == 11 && tokens[3].end == 11, "Token 3 has correct start and end" );
    FLASE_ASSERT( tokens[4].start == 13 && tokens[4].end == 17, "Token 4 has correct start and end" );
    FLASE_ASSERT( tokens[5].start == 18 && tokens[5].end == 18, "Token 5 has correct start and end" );
    FLASE_ASSERT( tokens[6].start == 20 && tokens[6].end == 20, "Token 6 has correct start and end" );
    FLASE_ASSERT( tokens[7].start == 21 && tokens[7].end == 21, "Token 7 has correct start and end" );
    FLASE_ASSERT( tokens[8].start == 22 && tokens[8].end == 22, "Token 8 has correct start and end" );
    FLASE_ASSERT( tokens[9].start == 24 && tokens[9].end == 24, "Token 9 has correct start and end" );
    FLASE_ASSERT( tokens[10].start == 25 && tokens[10].end == 25, "Token 10 has correct start and end" );
    FLASE_ASSERT( tokens[11].start == 27 && tokens[11].end == 33, "Token 11 has correct start and end" );
    FLASE_ASSERT( tokens[12].start == 34 && tokens[12].end == 34, "Token 12 has correct start and end" );
    FLASE_ASSERT( tokens[13].start == 36 && tokens[13].end == 36, "Token 13 has correct start and end" );
    FLASE_ASSERT( tokens[14].start == 37 && tokens[14].end == 37, "Token 14 has correct start and end" );
    FLASE_ASSERT( tokens[15].start == 39 && tokens[15].end == 44, "Token 15 has correct start and end" );
    FLASE_ASSERT( tokens[16].start == 45 && tokens[16].end == 45, "Token 16 has correct start and end" );
    FLASE_ASSERT( tokens[17].start == 46 && tokens[17].end == 46, "Token 17 has correct start and end" );
    FLASE_ASSERT( tokens[18].start == 47 && tokens[18].end == 47, "Token 18 has correct start and end" );

    FLASE_ASSERT( tokens[0].type == CJSON_TOKEN_OBJS, "Token 0 has correct type" );
    FLASE_ASSERT( tokens[1].type == CJSON_TOKEN_STR, "Token 1 has correct type" );
    FLASE_ASSERT( tokens[2].type == CJSON_TOKEN_COL, "Token 2 has correct type" );
    FLASE_ASSERT( tokens[3].type == CJSON_TOKEN_OBJS, "Token 3 has correct type" );
    FLASE_ASSERT( tokens[4].type == CJSON_TOKEN_STR, "Token 4 has correct type" );
    FLASE_ASSERT( tokens[5].type == CJSON_TOKEN_COL, "Token 5 has correct type" );
    FLASE_ASSERT( tokens[6].type == CJSON_TOKEN_ARRS, "Token 6 has correct type" );
    FLASE_ASSERT( tokens[7].type == CJSON_TOKEN_NUM, "Token 7 has correct type" );
    FLASE_ASSERT( tokens[8].type == CJSON_TOKEN_COM, "Token 8 has correct type" );
    FLASE_ASSERT( tokens[9].type == CJSON_TOKEN_NUM, "Token 9 has correct type" );
    FLASE_ASSERT( tokens[10].type == CJSON_TOKEN_COM, "Token 10 has correct type" );
    FLASE_ASSERT( tokens[11].type == CJSON_TOKEN_NUM, "Token 11 has correct type" );
    FLASE_ASSERT( tokens[12].type == CJSON_TOKEN_COM, "Token 12 has correct type" );
    FLASE_ASSERT( tokens[13].type == CJSON_TOKEN_NUM, "Token 13 has correct type" );
    FLASE_ASSERT( tokens[14].type == CJSON_TOKEN_COM, "Token 14 has correct type" );
    FLASE_ASSERT( tokens[15].type == CJSON_TOKEN_STR, "Token 15 has correct type" );
    FLASE_ASSERT( tokens[16].type == CJSON_TOKEN_ARRE, "Token 16 has correct type" );
    FLASE_ASSERT( tokens[17].type == CJSON_TOKEN_OBJE, "Token 17 has correct type" );
    FLASE_ASSERT( tokens[18].type == CJSON_TOKEN_OBJE, "Token 18 has correct type" );
}

FLASE_DEFINE_TEST( lexer_tokenize_not_enough_tokens )
{
    char* json_lit = "{ \"apple\": { \"tea\": [1, 2, -123223, 4, \"test\"]}}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[10];
    unsigned int num_tokens = 10;
    
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 0, "Tokenizer should fail because not enough tokens" );
}

FLASE_DEFINE_TEST( lexer_double_value )
{
    char* json_lit = "{\"apple\": 1 1}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    unsigned int r = CJSON_lexer_tokenize( json, tokens, &num_tokens );
    FLASE_ASSERT( r == 1, "Tokenizing parse sample success" );

    FLASE_ASSERT( num_tokens == 6, "Correct number of tokens tokenized" );
}

FLASE_DEFINE_TEST( lexer_termination_pass )
{
    char* json_lit = "{ \"apple\": { \"tea\": [1, 2, -123223, 4, \"test\"]}}";
    char json[1024];
    strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    CJSON_lexer_tokenize( json, tokens, &num_tokens );
    CJSON_lexer_termination_pass( json, tokens, num_tokens );
    
    // Check that the value tokens are null terminated
    for ( unsigned int token_i = 0; token_i < num_tokens; token_i++ )
    {
        CJSON_TOKEN* token = &( tokens[token_i] );
        unsigned char token_type = token->type;

        if ( token_type == CJSON_TOKEN_STR ) 
        {
            FLASE_ASSERT( json[token->end] == '\0', "STR token is null terminated" );
        }
        else if ( token_type == CJSON_TOKEN_NUM ||
            token_type == CJSON_TOKEN_FAL ||
            token_type == CJSON_TOKEN_TRU ||
            token_type == CJSON_TOKEN_NUL )
            {
                FLASE_ASSERT( json[token->end + 1] == '\0', "NUM, TRUE, FALSE and NUL tokens are null terminated" );
            }
    }
}

void test_lexer_run_tests()
{
    FLASE_RUN_TEST( lexer_is_whitespace );
    FLASE_RUN_TEST( lexer_next_string_state );
    FLASE_RUN_TEST( lexer_tokenize_valid );
    FLASE_RUN_TEST( lexer_tokenize_not_enough_tokens );
    FLASE_RUN_TEST( lexer_double_value );
    FLASE_RUN_TEST( lexer_termination_pass );
}

#endif // TEST_LEXER_H