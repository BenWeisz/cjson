#include "../include/parse.h"

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