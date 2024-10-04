#include "../include/parse.h"

unsigned int _CJSON_get_key_value_tokens( 
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations )
{
    CJSON_TOKEN* token = &( tokens[token_pos] );
    if ( token->type != CJSON_TOKEN_OBJS ) return 0;

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
            if ( scope_level == 0 ) return 0;
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
                else return 0;
            }
            else if ( kv_state == CJSON_PARSE_KV_STATE_KEY )
            {
                if ( token_type == CJSON_TOKEN_COL ) kv_state = CJSON_PARSE_KV_STATE_COL;
                else return 0;
            }
            else if ( kv_state == CJSON_PARSE_KV_STATE_COL )
            {
                if ( token_type == CJSON_TOKEN_OBJE ||
                    token_type == CJSON_TOKEN_ARRE || 
                    token_type == CJSON_TOKEN_COL || 
                    token_type == CJSON_TOKEN_COM ||
                    token_type == CJSON_TOKEN_UNK )
                {
                    return 0;
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
                else return 0;
            }
        }
    }

    if ( scope_level != 0 )
    {
        return 0;
    }

    *num_token_locations = curr_num_token_locations;

    return 1;
}

unsigned int _CJSON_get_value_tokens( 
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens,
    const unsigned int token_pos, 
    unsigned int* token_locations, 
    unsigned int* num_token_locations )
{
    CJSON_TOKEN* token = &( tokens[token_pos] );
    if ( token->type != CJSON_TOKEN_ARRS ) return 0;

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
                    return 0;
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
                else return 0;
            }
        }
    }

    if ( scope_level != 0 )
    {
        return 0;
    }

    *num_token_locations = curr_num_token_locations;

    return 1;
}

unsigned int _CJSON_parse(
    const char* buf,
    CJSON* cjson, 
    unsigned int* curr_num_nodes,
    const int parent_node_pos,
    CJSON_TOKEN* tokens,
    const unsigned int num_tokens, 
    const unsigned int token_pos )
{
    CJSON_TOKEN* token = &( tokens[token_pos] );
    unsigned char token_type = token->type;

    if ( token_type == CJSON_TOKEN_STR )
    {
        CJSON_NODE* node = &( cjson->nodes[( *curr_num_nodes )++] );
        node->type = CJSON_NODE_TYPE_VALUE;
        node->buf = ( char* )&( buf[token->start + 1] );
        node->parent = parent_node_pos;
    }
    else if ( token_type == CJSON_TOKEN_NUM ||
        token_type == CJSON_TOKEN_FAL ||
        token_type == CJSON_TOKEN_TRU ||
        token_type == CJSON_TOKEN_NUL )
    {
        CJSON_NODE* node = &( cjson->nodes[( *curr_num_nodes )++] );
        node->type = CJSON_NODE_TYPE_VALUE;
        node->buf = ( char* )&( buf[token->start] );
        node->parent = parent_node_pos;
    }
    else if ( token_type == CJSON_TOKEN_OBJS )
    {
        // Get the key-value locations in the tokens array
        unsigned int token_locations[CJSON_PARSE_LEVEL_MAX_ELEMENTS * 2];
        unsigned int key_locations[CJSON_PARSE_LEVEL_MAX_ELEMENTS];
        unsigned int num_token_locations = CJSON_PARSE_LEVEL_MAX_ELEMENTS * 2;
        unsigned int r = _CJSON_get_key_value_tokens( tokens, num_tokens, token_pos, token_locations, &num_token_locations );
        if ( r == 0 ) return 0;

        // Set the object node
        unsigned int obj_node_location = *curr_num_nodes;
        CJSON_NODE* obj_node = &( cjson->nodes[( *curr_num_nodes )++] );
        obj_node->type = CJSON_NODE_TYPE_OBJ;
        obj_node->buf = ( char* )&( buf[tokens[token_pos].start] );
        obj_node->parent = parent_node_pos;

        const unsigned num_kvs = num_token_locations / 2;

        // Set the key nodes
        for ( unsigned int key_i = 0; key_i < num_kvs; key_i++ )
        {
            unsigned int token_location = token_locations[key_i * 2];
            CJSON_TOKEN* token = &( tokens[token_location] );

            CJSON_NODE* key_node = &( cjson->nodes[( *curr_num_nodes )++] );
            key_node->type = CJSON_NODE_TYPE_KEY;
            key_node->buf = ( char* )&( buf[token->start + 1] );
            key_node->parent = obj_node_location;
            key_locations[key_i] = (*curr_num_nodes) - 1;
        }

        // Set the value nodes
        for ( unsigned int value_i = 0; value_i < num_kvs; value_i++ )
        {
            unsigned int token_location = token_locations[1 + ( value_i * 2 )];
            unsigned int parent_loc = key_locations[value_i];
            r = _CJSON_parse( buf, cjson, curr_num_nodes, parent_loc, tokens, num_tokens, token_location );
            if ( r == 0 ) return 0;
        }
    }
    else if ( token_type == CJSON_TOKEN_ARRS )
    {
        // Get the value locations in the tokens array
        unsigned int token_locations[CJSON_PARSE_LEVEL_MAX_ELEMENTS];
        unsigned int num_token_locations = CJSON_PARSE_LEVEL_MAX_ELEMENTS;
        unsigned int r = _CJSON_get_value_tokens( tokens, num_tokens, token_pos, token_locations, &num_token_locations );
        if ( r == 0 ) return 0;

        // Set the array node
        unsigned int arr_node_location = *curr_num_nodes;
        CJSON_NODE* arr_node = &( cjson->nodes[( *curr_num_nodes )++] );
        arr_node->type = CJSON_NODE_TYPE_ARR;
        arr_node->buf = ( char* )&( buf[tokens[token_pos].start] );
        arr_node->parent = parent_node_pos;

        // Set the value nodes
        for ( unsigned int value_i = 0; value_i < num_token_locations; value_i++ )
        {
            unsigned int token_location = token_locations[value_i];
            r = _CJSON_parse( buf, cjson, curr_num_nodes, arr_node_location, tokens, num_tokens, token_location );
            if ( r == 0 ) return 0;
        }
    }
    else return 0;

    return 1;
}
unsigned int CJSON_parse_with_settings( char* buf, CJSON* cjson, const unsigned int settings )
{
    // CJSON_LEXER_TOKEN_BUFFER_SIZE
    CJSON_TOKEN tokens[CJSON_LEXER_TOKEN_BUFFER_SIZE];

    // Lex the tokens
    unsigned int num_tokens = CJSON_LEXER_TOKEN_BUFFER_SIZE;
    unsigned int r = _CJSON_lexer_tokenize( buf, tokens, &num_tokens );
    if ( r == 0 ) return 0;
    
    if ( settings & CJSON_SCOPE_CHECKING )
    {
        // Verify that the scope are logical
        r = _CJSON_verify_scopes( tokens, num_tokens );
        if ( r == 0 ) return 0;
    }

    // Parse the tokens
    unsigned int num_nodes = 0;
    r = _CJSON_parse(
        buf,
        cjson,
        &num_nodes,
        -1,
        tokens,
        num_tokens,
        0
    );

    if ( num_nodes > cjson->num_nodes ) return 0;
    cjson->num_nodes = num_nodes;

    if ( settings & CJSON_VALUE_TERMINATION )
    {
        // Terminate the tokens in the buffer
        _CJSON_lexer_termination_pass( buf, tokens, num_tokens );
    }
    
    return 1;
}

unsigned int CJSON_parse( char* buf, CJSON* cjson )
{
    return CJSON_parse_with_settings( buf, cjson, CJSON_SCOPE_CHECKING | CJSON_VALUE_TERMINATION );
}