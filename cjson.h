#ifndef CJSON_CONFIG_H
#define CJSON_CONFIG_H

#ifndef CJSON_LEXER_TOKEN_BUFFER_SIZE
    #define CJSON_LEXER_TOKEN_BUFFER_SIZE       1024
#endif // CJSON_LEXER_TOKEN_BUFFER_SIZE

#ifndef CJSON_VERIFY_SCOPE_STACK_SIZE
    #define CJSON_VERIFY_SCOPE_STACK_SIZE       1024
#endif // CJSON_VERIFY_SCOPE_STACK_SIZE

#ifndef CJSON_PARSE_LEVEL_MAX_ELEMENTS
    #define CJSON_PARSE_LEVEL_MAX_ELEMENTS      1024
#endif // CJSON_PARSE_LEVEL_MAX_ELEMENTS

#ifndef CJSON_PARSE_QUEUE_SIZE
    #define CJSON_PARSE_QUEUE_SIZE              ( CJSON_LEXER_TOKEN_BUFFER_SIZE / 2 ) // This should probably be enough
#endif // CJSON_PARSE_QUEUE_SIZE

#endif // CJSON_CONFIG_H
#ifndef CJSON_TYPES_H
#define CJSON_TYPES_H


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

#ifndef CJSON_LOG_H
#define CJSON_LOG_H


#ifdef CJSON_DEBUG
    #include <stdio.h>
    #define CJSON_PRINT( ... ) printf( __VA_ARGS__ )
    #define CJSON_LOG( ... ) do { \
            CJSON_PRINT( "\e[1;32mLOG:\e[0m | [%s:%d] : ", __FILE__, __LINE__ ); \
            CJSON_PRINT( __VA_ARGS__ ); \
        } while( 0 )
    #define CJSON_WARN( ... ) do { \
            CJSON_PRINT( "\e[1;33mWARN:\e[0m | [%s:%d] : ", __FILE__, __LINE__ ); \
            CJSON_PRINT( __VA_ARGS__ ); \
        } while( 0 )
    #define CJSON_ERROR( ... ) do { \
            CJSON_PRINT( "\e[1;31mERROR\e[0m | [%s:%d] : ", __FILE__, __LINE__ ); \
            CJSON_PRINT( __VA_ARGS__ ); \
        } while( 0 )
#else
    #define CJSON_LOG( ... )
    #define CJSON_WARN( ... )
    #define CJSON_ERROR( ... )
#endif

#endif // CJSON_LOG_H

#ifndef CJSON_STR
#define CJSON_STR


/* Character code */
unsigned int CJSON_char_is_whitespace( const char c );

/* String code */
unsigned long CJSON_strtol( const char* str );
unsigned int CJSON_strncmp( const char* str1, const char* str2, unsigned long n );
unsigned long CJSON_strlen( const char* str );

#endif // CJSON_STR
#ifndef CJSON_LEXER_H
#define CJSON_LEXER_H


unsigned char CJSON_lexer_next_string_state( const unsigned char curr_state, const char c );

unsigned int CJSON_lexer_tokenize( const char* buf, CJSON_TOKEN* tokens, unsigned int* num_tokens );

void CJSON_lexer_termination_pass( char* buf, CJSON_TOKEN* tokens, unsigned int num_tokens );

#endif // CJSON_LEXER_H
#ifndef CJSON_SEARCH_H
#define CJSON_SEARCH_H


long CJSON_search_find_first_with_parent(
    CJSON_NODE* nodes,
    long low,
    long high,
    long parent );

CJSON_NODE* CJSON_search_wrapper( CJSON_NODE* nodes, const long n, const char* keys[] );

// Credits: Tim Schaeffer: https://stackoverflow.com/questions/3272444/use-variadic-functions-in-c89-without-passing-number-of-arguments-or-a-final-arg 
#define CJSON_search( nodes, ... ) \
    ({ \
        CJSON_NODE* r; \
        do { \
            const char* keys[] = { __VA_ARGS__ }; \
            r = CJSON_search_wrapper( nodes, sizeof( keys ) / sizeof( const char* ), keys ); \
        } while( 0 ); \
        r; \
     })

#define CJSON_IS_NUM( x ) ( ( ( x )[ 0 ] >= '0' && ( x )[ 0 ] <= '9' ) || ( x )[ 0 ] == '-' )

#endif // CJSON_SEARCH_H

#ifndef CJSON_VERIFY_H
#define CJSON_VERIFY_H


unsigned int CJSON_verify_scopes( CJSON_TOKEN* tokens, unsigned int num_tokens );

#endif // CJSON_VERIFY_H
#ifndef CJSON_PARSE_H
#define CJSON_PARSE_H


/* Get the token indices where the key tokens are */
unsigned int CJSON_get_key_tokens(
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations );

/* Get the token indicees of where the values are  */
unsigned int CJSON_get_value_tokens( 
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations );

/* Populate the rev_i parameter of all of the CJSON_NODE structs */
void CJSON_parse_indexing_pass( CJSON_NODE* nodes, unsigned int num_nodes );

unsigned int CJSON_parse_wrapper( 
    CJSON_NODE* nodes,
    unsigned int* num_nodes,
    CJSON_TOKEN* tokens, 
    const unsigned int num_tokens, 
    char* buf );

unsigned int CJSON_parse_with_settings( char* buf, CJSON_NODE* nodes, unsigned int* num_nodes, const unsigned int settings );
unsigned int CJSON_parse( char* buf, CJSON_NODE* nodes, unsigned int* num_nodes );

#endif // CJSON_PARSE_H

#ifdef CJSON_IMPLEMENTATION

/* Character code */
unsigned int CJSON_char_is_whitespace( const char c )
{
    return c == '\t' || c == '\n' || c == '\r' || c == ' ';
}

/* String code */
unsigned long CJSON_strtol( const char* str )
{
    unsigned int pos = 0;
    char c = str[pos];
    while ( CJSON_char_is_whitespace( c ) ) {
        c = str[pos++];
    }

    if ( c < '0' || c > '9' )
    {
        CJSON_ERROR( "CJSON_strtol passed non-number, returning 0" );
        return 0;
    }

    unsigned long r = 0;
    while ( c >= '0' && c <= '9' )
    {
        r *= 10;
        r += c - '0';
        c = str[pos++];
    }

    return r;
}

unsigned int CJSON_strncmp( const char* str1, const char* str2, unsigned long n )
{
    unsigned long i = 0;
    while ( str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0' && i < n ) i++;
    
    return str1[i] != str2[i];
}

unsigned long CJSON_strlen( const char* str )
{
    unsigned long len = 0;
    while ( str[len] != '\0' ) len++;
    return len;
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
            else if ( CJSON_char_is_whitespace( c ) == 0 )
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
        return CJSON_FAILURE;
    }

    *num_tokens = num_tokens_used - 1;
    return CJSON_SUCCESS;
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

unsigned int CJSON_verify_scopes( CJSON_TOKEN* tokens, unsigned int num_tokens )
{
    unsigned int token_i = 0;
    CJSON_TOKEN* token = &( tokens[token_i++] );

    if ( token->type != CJSON_TOKEN_OBJS )
    {
        CJSON_ERROR( "The token type of the root node's first token must be an opening object type token!\n" );
        return CJSON_FAILURE;
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
                return CJSON_FAILURE;
            }
        }
        else if ( token->type == CJSON_TOKEN_ARRE )
        {
            if ( scope_stack_top > 0 && scope_stack[scope_stack_top - 1] == CJSON_VERIFY_ARR_MARKER ) scope_stack_top--;
            else
            {
                CJSON_ERROR( "Trying to close scope that doesn't exist OR attempting to close object scope with an array closing token!\n" );
                return CJSON_FAILURE;
            }
        }
    }

    if ( scope_stack_top != 0 )
    {
        CJSON_ERROR( "Some scopes were not closed!\n" );
    }

    return scope_stack_top == 0;
}

/* Binary search for the first CJSON node with a parent value of <parent> */
long CJSON_search_find_first_with_parent(
    CJSON_NODE* nodes,
    long low,
    long high,
    long parent )
{	
    while ( low <= high )
    {
        long mid = ( low + high ) / 2;
        long mid_v = nodes[ mid ].parent;

        if ( mid_v < parent )
        {
            low = mid + 1;
        }
        else if ( mid > 0 && nodes[ mid - 1 ].parent >= parent )
        {
            high = mid - 1;
        }
        else if ( mid_v == parent )
        {
            return mid;
        }
        else
        {
            return -1;
        }
    }

    return -1;
}

CJSON_NODE* CJSON_search_wrapper( CJSON_NODE* nodes, const long n, const char* keys[] )
{
    long key_i = 0;
    long node_i = 0;
    while ( key_i < n )
    {
        const char* key = keys[ key_i++ ];
        CJSON_NODE* node = &( nodes[ node_i ] );

        if ( node->type == CJSON_NODE_TYPE_OBJ )
        {
            long key_node_i = CJSON_search_find_first_with_parent( nodes, node_i, nodes[0].rev_i + 1 - 1, node->i );
            if ( key_node_i == -1 )
            {
                CJSON_ERROR( "Trying to get value from object that has no keys / values!\n" );
                return CJSON_NULL;
            }
            
            CJSON_NODE* key_node = &( nodes[ key_node_i ] );
            while ( key_node->parent == node->i )
            {
                if ( CJSON_strncmp( key_node->buf, key, CJSON_strlen( key ) ) == 0 ) break;
                
                key_node_i++;
                key_node = &( nodes[key_node_i] );
            }
            if ( key_node->parent != node->i )
            {
                CJSON_ERROR( "Failed to find key \"%s\" in downstream object!\n", key );
                return CJSON_NULL;
            }

            node_i = CJSON_search_find_first_with_parent( nodes, key_node_i, nodes[0].rev_i + 1 - 1, key_node->i );
        }
        else if ( node->type == CJSON_NODE_TYPE_ARR )
        {
            if ( !CJSON_IS_NUM( key ) )
            {
                CJSON_ERROR( "Attempting to index into array using non-integeral index!\n" );
                return CJSON_NULL;
            }

            unsigned long ind = CJSON_strtol( key );
            long element_node_i = CJSON_search_find_first_with_parent( nodes, node_i, nodes[0].rev_i + 1 - 1, node->i );
            if ( element_node_i == -1 )
            {
                CJSON_ERROR( "Attempting to index into empty downstream array!\n" );
                return CJSON_NULL;
            }

            node_i = element_node_i + ind;

             // nodes[0].rev_i + 1 is the pseudo length of nodes
            if ( node_i >= nodes[0].rev_i + 1 || node_i < 0 )
            {
                CJSON_ERROR( "Out of bounds error due to indexing outside of array!\n" );
                return CJSON_NULL;
            }
            else if ( nodes[ node_i ].parent != node->i )
            {
                CJSON_ERROR( "Critical error with CJSON_search_find_first_with_parent" );
                return CJSON_NULL;
            }
        }
        else if ( node->type == CJSON_NODE_TYPE_KEY || node->type == CJSON_NODE_TYPE_VALUE )
        {
            CJSON_ERROR( "Search algorithm got derailed and tried indexing / keying into a value type / key type!\n" );
            return CJSON_NULL;
        }
    }

    return &( nodes[ node_i ] );
}


unsigned int CJSON_get_key_tokens(
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations )
{
    CJSON_TOKEN* token = &( tokens[token_pos] );
    if ( token->type != CJSON_TOKEN_OBJS )
    {
        CJSON_ERROR( "The first token must be of an object type if we are to get it's keys!\n" );
        return CJSON_FAILURE;
    }

    unsigned int curr_num_token_locations = 0;
    unsigned int scope_level = 1;
    unsigned char kv_state = CJSON_PARSE_KV_STATE_COM;
    for ( int token_i = token_pos + 1; token_i < num_tokens && curr_num_token_locations < *num_token_locations; token_i++ )
    {
        token = &( tokens[token_i] );
        if ( scope_level == 0 ) break;
        
        unsigned char token_type = token->type;
        if ( token_type == CJSON_TOKEN_OBJS || token_type == CJSON_TOKEN_ARRS )
        {
            scope_level++;
            if ( scope_level == 2 ) kv_state = CJSON_PARSE_KV_STATE_VAL;
        }
        else if ( token_type == CJSON_TOKEN_OBJE || token_type == CJSON_TOKEN_ARRE )
        {
            if ( scope_level == 0 )
            {
                CJSON_ERROR( "Attempting to close scope that doesnt exist!\n" );
                return CJSON_FAILURE;
            }
            scope_level--;
        }
        else if ( scope_level == 1 )
        {
            if ( kv_state == CJSON_PARSE_KV_STATE_COM )
            {
                if ( token_type == CJSON_TOKEN_STR )
                {
                    kv_state = CJSON_PARSE_KV_STATE_KEY;
                    token_locations[curr_num_token_locations++] = token_i;
                }
                else
                {
                    CJSON_ERROR( "The token type following a comma type token must be of string type!\n" );
                    return CJSON_FAILURE;
                }
            }
            else if ( kv_state == CJSON_PARSE_KV_STATE_KEY )
            {
                if ( token_type == CJSON_TOKEN_COL ) kv_state = CJSON_PARSE_KV_STATE_COL;
                else
                {
                    CJSON_ERROR( "The token type following a string token that is a key must be a colon type token!\n" );
                    return CJSON_FAILURE;
                }
            }
            else if ( kv_state == CJSON_PARSE_KV_STATE_COL )
            {
                if ( token_type == CJSON_TOKEN_OBJE ||
                    token_type == CJSON_TOKEN_ARRE || 
                    token_type == CJSON_TOKEN_COL || 
                    token_type == CJSON_TOKEN_COM ||
                    token_type == CJSON_TOKEN_UNK )
                {
                    CJSON_ERROR( "The token type following a colon type token must be a value class token!\n" );
                    return CJSON_FAILURE;
                }
                else kv_state = CJSON_PARSE_KV_STATE_VAL;
            }
            else if ( kv_state == CJSON_PARSE_KV_STATE_VAL )
            {
                if ( token_type == CJSON_TOKEN_COM ) kv_state = CJSON_PARSE_KV_STATE_COM;
                else
                {
                    CJSON_ERROR( "A comma type token must follow a set of value type tokens!\n" );
                    return CJSON_FAILURE;
                }
            }
        }
    }

    if ( scope_level != 0 )
    {
        CJSON_ERROR( "After all tokens are processed, all scopes must be closed!\n" );
        return CJSON_FAILURE;
    }

    *num_token_locations = curr_num_token_locations;

    return CJSON_SUCCESS;
}

unsigned int CJSON_get_value_tokens( 
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations )
{
    CJSON_TOKEN* token = &( tokens[token_pos] );
    if ( token->type != CJSON_TOKEN_ARRS )
    {
        CJSON_ERROR( "The first token must be of an array opening token if we are to get the value's of the array!\n" );
        return CJSON_FAILURE;
    }

    unsigned int curr_num_token_locations = 0;
    unsigned int scope_level = 1;
    unsigned char kv_state = CJSON_PARSE_KV_STATE_COM;
    for ( int token_i = token_pos + 1; token_i < num_tokens && curr_num_token_locations < *num_token_locations; token_i++ )
    {
        token = &( tokens[token_i] );
        if ( scope_level == 0 ) break;
        
        unsigned char token_type = token->type;
        if ( token_type == CJSON_TOKEN_OBJS || token_type == CJSON_TOKEN_ARRS )
        {
            scope_level++;
            if ( scope_level == 2 )
            {
                kv_state = CJSON_PARSE_KV_STATE_VAL;
                token_locations[curr_num_token_locations++] = token_i;
            }
        }
        else if ( token_type == CJSON_TOKEN_OBJE || token_type == CJSON_TOKEN_ARRE )
        {
            scope_level--;
        }
        else if ( scope_level == 1 )
        {
            if ( kv_state == CJSON_PARSE_KV_STATE_COM )
            {
                if ( token_type == CJSON_TOKEN_OBJE ||
                    token_type == CJSON_TOKEN_ARRE || 
                    token_type == CJSON_TOKEN_COL || 
                    token_type == CJSON_TOKEN_COM ||
                    token_type == CJSON_TOKEN_UNK )
                {
                    CJSON_ERROR( "A value type token must follow a comma type token!\n" );
                    return CJSON_FAILURE;
                }
                else 
                {
                    kv_state = CJSON_PARSE_KV_STATE_VAL;
                    token_locations[curr_num_token_locations++] = token_i;
                }
            }
            else if ( kv_state == CJSON_PARSE_KV_STATE_VAL )
            {
                if ( token_type == CJSON_TOKEN_COM ) kv_state = CJSON_PARSE_KV_STATE_COM;
                else
                {
                    CJSON_ERROR( "A comma type token must follow a set of value type tokens!\n" );
                    return CJSON_FAILURE;
                }
            }
        }
    }

    if ( scope_level != 0 )
    {
        CJSON_ERROR( "After all tokens are processed, all scopes must be closed!\n" );
        return CJSON_FAILURE;
    }

    *num_token_locations = curr_num_token_locations;

    return CJSON_SUCCESS;
}

void CJSON_parse_indexing_pass( CJSON_NODE* nodes, unsigned int num_nodes )
{
    for ( unsigned int node_i = 0; node_i < num_nodes; node_i++ )
    {
        nodes[node_i].i = node_i;
        nodes[node_i].rev_i = num_nodes - 1 - node_i;
    }
}

unsigned int CJSON_parse_wrapper( CJSON_NODE* nodes, unsigned int* num_nodes, CJSON_TOKEN* tokens, const unsigned int num_tokens, char* buf )
{
    // Setup parsing for managing the parsing order
    CJSON_PARSE_QUEUE_ELEMENT parse_queue[CJSON_PARSE_QUEUE_SIZE];
    unsigned int parse_queue_front = 0;
    unsigned int parse_queue_back = 0;

    // Load first token
    CJSON_TOKEN* token = &( tokens[0] );
    unsigned char token_type = token->type;

    if ( token->type != CJSON_TOKEN_OBJS )
    {
        CJSON_ERROR( "The first token parsed must be the root object's opening token type token!\n" );
        return CJSON_FAILURE;
    }
    
    // Queue the first parse element
    CJSON_PARSE_QUEUE_ELEMENT* parse_element = &( parse_queue[parse_queue_back] );
    parse_element->token_location = 0;
    parse_element->token_key_flag = CJSON_PARSE_QUEUE_OTHER_FLAG;
    parse_element->token_parent = -1;
    parse_queue_back = (parse_queue_back + 1) % CJSON_PARSE_QUEUE_SIZE;

    // Compute queue length (this is a demo)
    unsigned int token_queue_size = 1;
    unsigned int curr_node_i = 0;
    while ( token_queue_size != 0 && curr_node_i < *num_nodes )
    {
        // Pop the top parsing element in the queue
        parse_element = &( parse_queue[parse_queue_front] );
        parse_queue_front = ( parse_queue_front + 1 ) % CJSON_PARSE_QUEUE_SIZE;

        // Get the current token
        token = &( tokens[parse_element->token_location] );
        token_type = token->type;
        
        if ( token_type == CJSON_TOKEN_OBJS )
        {
            unsigned int token_locations[CJSON_PARSE_LEVEL_MAX_ELEMENTS];
            unsigned int num_token_locations = CJSON_PARSE_LEVEL_MAX_ELEMENTS;

            unsigned int r = CJSON_get_key_tokens(tokens, num_tokens, parse_element->token_location, token_locations, &num_token_locations);
            if ( r == 0 )
            {
                // Error reporting handled by CJSON_get_key_tokens
                return CJSON_FAILURE;
            }

            unsigned int i = 0;
            while ( i < num_token_locations )
            {
                CJSON_PARSE_QUEUE_ELEMENT* new_element = &( parse_queue[parse_queue_back] );
                new_element->token_location = token_locations[i];
                new_element->token_key_flag = CJSON_PARSE_QUEUE_KEY_FLAG;
                new_element->token_parent = curr_node_i;
                parse_queue_back = (parse_queue_back + 1) % CJSON_PARSE_QUEUE_SIZE;
                i++;
            }

            // Add an object node
            nodes[curr_node_i].type = CJSON_NODE_TYPE_OBJ;
            nodes[curr_node_i].buf = &( buf[token->start] );
            nodes[curr_node_i].parent = parse_element->token_parent;
        }
        else if ( token_type == CJSON_TOKEN_ARRS )
        {
            unsigned int token_locations[CJSON_PARSE_LEVEL_MAX_ELEMENTS];
            unsigned int num_token_locations = CJSON_PARSE_LEVEL_MAX_ELEMENTS;

            unsigned int r = CJSON_get_value_tokens(tokens, num_tokens, parse_element->token_location, token_locations, &num_token_locations);
            if ( r == 0 ) 
            {
                // Error reporting handled by CJSON_get_value_tokens
                return CJSON_FAILURE;
            }

            unsigned int i = 0;
            while ( i < num_token_locations )
            {
                CJSON_PARSE_QUEUE_ELEMENT* new_element = &( parse_queue[parse_queue_back] );
                new_element->token_location = token_locations[i];
                new_element->token_key_flag = CJSON_PARSE_QUEUE_OTHER_FLAG;
                new_element->token_parent = curr_node_i;
                parse_queue_back = (parse_queue_back + 1) % CJSON_PARSE_QUEUE_SIZE;
                i++;
            }

            // Add an array node
            nodes[curr_node_i].type = CJSON_NODE_TYPE_ARR;
            nodes[curr_node_i].buf = &( buf[token->start] );
            nodes[curr_node_i].parent = parse_element->token_parent;
        }
        else if ( token_type == CJSON_TOKEN_STR )
        {
            if ( parse_element->token_key_flag == CJSON_PARSE_QUEUE_KEY_FLAG )
            {
                CJSON_PARSE_QUEUE_ELEMENT* new_element = &( parse_queue[parse_queue_back] );
                new_element->token_location = parse_element->token_location + 2; // Skip the colon
                new_element->token_key_flag = CJSON_PARSE_QUEUE_OTHER_FLAG;
                new_element->token_parent = curr_node_i;
                parse_queue_back = ( parse_queue_back + 1 ) % CJSON_PARSE_QUEUE_SIZE;
    
                nodes[curr_node_i].type = CJSON_NODE_TYPE_KEY;
            }
            else
            {
                nodes[curr_node_i].type = CJSON_NODE_TYPE_VALUE;
            }
            
            nodes[curr_node_i].buf = &( buf[token->start + 1] );
            nodes[curr_node_i].parent = parse_element->token_parent;
        }
        else 
        {
            nodes[curr_node_i].type = CJSON_NODE_TYPE_VALUE;
            nodes[curr_node_i].buf = &( buf[token->start] );
            nodes[curr_node_i].parent = parse_element->token_parent;
        }

        // Compute the token_queue size
        if ( parse_queue_back >= parse_queue_front )
            token_queue_size = parse_queue_back - parse_queue_front;
        else token_queue_size = CJSON_PARSE_QUEUE_SIZE - parse_queue_front + parse_queue_back;

        curr_node_i++;
    }

    // Ran out of space for new nodes
    if ( token_queue_size != 0 )
    {
        CJSON_ERROR( "All CJSON_NODE slots used up in parsing the input string, please increase the number of CJSON_NODE's you're allocating for this parsing function!\n" );
        return CJSON_FAILURE;
    }

    *num_nodes = curr_node_i;

    return CJSON_SUCCESS;
}

unsigned int CJSON_parse_with_settings( char* buf, CJSON_NODE* nodes, unsigned int* num_nodes, const unsigned int settings )
{
    // CJSON_LEXER_TOKEN_BUFFER_SIZE
    CJSON_TOKEN tokens[CJSON_LEXER_TOKEN_BUFFER_SIZE];

    // Lex the tokens
    unsigned int num_tokens = CJSON_LEXER_TOKEN_BUFFER_SIZE;
    unsigned int r = CJSON_lexer_tokenize( buf, tokens, &num_tokens );
    if ( r == 0 ) 
    {
        // Error reporting handled by CJSON_lexer_tokenize
        return CJSON_FAILURE;
    }
    
    if ( settings & CJSON_PARSE_SCOPE_CHECKING )
    {
        // Verify that the scope are logical
        r = CJSON_verify_scopes( tokens, num_tokens );
        if ( r == 0 )
        {
            // Error reporting handled by CJSON_verify_scopes
            return CJSON_FAILURE;
        }
    }

    // Parse the tokens
    r = CJSON_parse_wrapper( nodes, num_nodes, tokens, num_tokens, buf );
    if ( r == 0 )
    {
        // Error reporting handled by CJSON_parse_wrapper
        return CJSON_FAILURE;
    }

    // Populate the rev_i variable in each CJSON_NODE
    CJSON_parse_indexing_pass( nodes, *num_nodes );

    // Terminate the tokens in the buffer
    CJSON_lexer_termination_pass( buf, tokens, num_tokens );
    
    return CJSON_SUCCESS;
}

unsigned int CJSON_parse( char* buf, CJSON_NODE* nodes, unsigned int* num_nodes )
{
    return CJSON_parse_with_settings( buf, nodes, num_nodes, CJSON_PARSE_SCOPE_CHECKING );
}
#endif //CJSON_IMPLEMENTATION
