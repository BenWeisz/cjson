#ifndef TEST_VERIFY_H
#define TEST_VERIFY_H

#include "types.h"
#include "verify.h"
#include "lexer.h"

#include "flase.h"

FLASE_DEFINE_TEST( verify_signal_good_scope )
{
    char* json_lit = "{ \"apple\": { \"tea\": [1, 2, -123223, 4, \"test\"]}}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );

    unsigned int r = _CJSON_verify_scopes( tokens, num_tokens );
    FLASE_ASSERT( r == 1, "Valid scope" );
}

FLASE_DEFINE_TEST( verify_signal_good_scope_lots_of_brackets )
{
    char* json_lit = "{ \"apple\": [[[[], [[], [[[[[]]]]]]]]]}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    unsigned int r = _CJSON_verify_scopes( tokens, num_tokens );
    FLASE_ASSERT( r == 1, "Valid scope - many arrays" );
}

FLASE_DEFINE_TEST( verify_signal_good_scope_simple )
{
    char* json_lit = "{}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    unsigned int r = _CJSON_verify_scopes( tokens, num_tokens );
    FLASE_ASSERT( r == 1, "Valid scope - simple" );
}

FLASE_DEFINE_TEST( verify_signal_bad_scope_simple )
{
    char* json_lit = "{]";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    unsigned int r = _CJSON_verify_scopes( tokens, num_tokens );
    FLASE_ASSERT( r == 0, "Invalid scope - simple" );
}

FLASE_DEFINE_TEST( verify_signal_bad_scope )
{
    char* json_lit = "{\"apple\":[{]}";
	char json[1024];
	strcpy( json, json_lit );

    CJSON_TOKEN tokens[1024];
    unsigned int num_tokens = 1024;
    
    _CJSON_lexer_tokenize( json, tokens, &num_tokens );
    unsigned int r = _CJSON_verify_scopes( tokens, num_tokens );
    FLASE_ASSERT( r == 0, "Invalid scope " );
}

void test_verify_run_tests()
{
    FLASE_RUN_TEST( verify_signal_good_scope );
    FLASE_RUN_TEST( verify_signal_good_scope_lots_of_brackets );
    FLASE_RUN_TEST( verify_signal_good_scope_simple );
    FLASE_RUN_TEST( verify_signal_bad_scope_simple );
    FLASE_RUN_TEST( verify_signal_bad_scope );
}

#endif // TEST_VERIFY_H
