#include "cjson4.h"

// Convert the buf string to a list of _CJSON_LEXER_TOKENs
_CJSON_LEXER_TOKEN* _CJSON_lexer_lex_tokens(const char* buf)
{
    unsigned long buf_pos = 0;
    _CJSON_lexer_eat_whitespace(buf, &buf_pos);

    char buf_token = buf[buf_pos];
    if (buf_token != '{')
    {
        printf("ERROR: Missing root node!\n");
        return NULL;
    }

    // After processing first node, grab the next non-whitespace character
    buf_pos++;
    _CJSON_lexer_eat_whitespace(buf, &buf_pos);
    buf_token = buf[buf_pos];

    _CJSON_LEXER_TOKEN* tokens = (_CJSON_LEXER_TOKEN*)malloc(sizeof(_CJSON_LEXER_TOKEN));
    if (tokens == NULL)
    {
        printf("ERROR: Failed to allocate space for CJSON lexer tokens!\n");
        return NULL;
    }
    _CJSON_LEXER_TOKEN* curr_token = tokens;

    curr_token->type = _CJSON_LEXER_TOKEN_LCB;
    curr_token->text = NULL;
    curr_token->next = NULL;

    // Just to be pedantic :) instead of using a string literal
    char control_tokens[7];
    control_tokens[_CJSON_LEXER_TOKEN_LCB] = '{';
    control_tokens[_CJSON_LEXER_TOKEN_RCB] = '}';
    control_tokens[_CJSON_LEXER_TOKEN_LSB] = '[';
    control_tokens[_CJSON_LEXER_TOKEN_RSB] = ']';
    control_tokens[_CJSON_LEXER_TOKEN_COLON] = ':';
    control_tokens[_CJSON_LEXER_TOKEN_COMMA] = ',';
    control_tokens[6] = '\0';

    while (buf_token != '\0')
    {
        int control_token_i = 0;
        char control_token = control_tokens[control_token_i];
        while (control_token != '\0')
        {
            if (buf_token == control_token)
            {
                break;
            }

            control_token_i++;
            control_token = control_tokens[control_token_i];
        }

        if (control_token_i == _CJSON_LEXER_TOKEN_STR)
        {
            curr_token->next = (_CJSON_LEXER_TOKEN*)malloc(sizeof(_CJSON_LEXER_TOKEN));
            if (curr_token->next == NULL)
            {
                printf("ERROR: Failed to allocate memory for CJSON lexer\n");
                _CJSON_lexer_free_tokens(tokens);
                return NULL;
            }
            curr_token = curr_token->next;

            // Read all strings, numbers, true, false, null values
            const char* str_token = _CJSON_lexer_eat_str(buf, &buf_pos);
            if (str_token == NULL)
            {
                _CJSON_lexer_free_tokens(tokens);
                return NULL;
            }

            _CJSON_lexer_init_token(curr_token, _CJSON_LEXER_TOKEN_STR, str_token);
        }
        else {
            curr_token->next = (_CJSON_LEXER_TOKEN*)malloc(sizeof(_CJSON_LEXER_TOKEN));
            if (curr_token->next == NULL)
            {
                printf("ERROR: Failed to allocate memory for CJSON lexer\n");
                _CJSON_lexer_free_tokens(tokens);
                return NULL;
            }
            curr_token = curr_token->next;
            _CJSON_lexer_init_token(curr_token, control_token_i, NULL);
        }

        // Get the next non-whitespace character
        buf_pos++;
        _CJSON_lexer_eat_whitespace(buf, &buf_pos);
        buf_token = buf[buf_pos];
    }

    return tokens;
}

// Start reading buf at position buf_pos until you encounter the
// end of buf or a non-whitespace character. Move buf_pos to the
// next non-whitespace character.
void _CJSON_lexer_eat_whitespace(const char* buf, unsigned long* buf_pos)
{
    char buf_token = buf[*buf_pos];
    while (buf_token != '\0')
    {
        if (buf_token != ' ' &&
            buf_token != '\t' &&
            buf_token != '\r' &&
            buf_token != '\n') return;

        *buf_pos += 1;
        buf_token = buf[*buf_pos];
    }
}

// Starting at buf_pos in buf, read all the characters corresponding to one
// string, number, true, false or null value.
const char* _CJSON_lexer_eat_str(const char* buf, unsigned long* buf_pos)
{
    unsigned long temp_buf_pos = *buf_pos;
    char buf_token = buf[*buf_pos];
    unsigned long str_token_size = 0;

    // Handle strings
    if (buf_token == '"')
    {
        // Skip first '"'
        *buf_pos += 1;
        buf_token = buf[*buf_pos];
        str_token_size++;

        // "apple\"sauce"
        unsigned int escape_mode = 0;
        while (buf_token != '\0')
        {
            if (buf_token == '"' && !escape_mode)
            {
                str_token_size++; // '"'
                break;
            }

            if (escape_mode)
            {
                escape_mode = 0;
            }
            else {
                if (buf_token == '\\')
                {
                    escape_mode = 1;
                }
            }

            *buf_pos += 1;
            buf_token = buf[*buf_pos];
            str_token_size++;
        }
    }
    else
    {
        while (buf_token != '\0')
        {
            if (buf_token == ' ' ||
                buf_token == '\n' ||
                buf_token == '\t' ||
                buf_token == '\r' ||
                buf_token == '{' ||
                buf_token == '}' ||
                buf_token == '[' ||
                buf_token == ']' ||
                buf_token == ':' ||
                buf_token == ',')
            {
                break;
            }

            *buf_pos += 1;
            buf_token = buf[*buf_pos];
            str_token_size++;
        }

        *buf_pos -= 1; // Let the loop take care of moving past the last character
    }

    char* str_token = (char*)malloc(sizeof(char) * (str_token_size + 1));
    if (str_token == NULL)
    {
        printf("ERROR: Failed to allocate space for CJSON lexer string\n");
        return NULL;
    }

    memcpy(str_token, &(buf[temp_buf_pos]), str_token_size);
    str_token[str_token_size] = '\0';

    return str_token;
}

// Free all the lexer tokens
void _CJSON_lexer_free_tokens(_CJSON_LEXER_TOKEN* tokens)
{
    _CJSON_LEXER_TOKEN* curr_token = tokens;
    while (curr_token != NULL)
    {
        _CJSON_LEXER_TOKEN* temp_token = curr_token;
        curr_token = curr_token->next;

        free(temp_token);
    }
}

// Init a single token
void _CJSON_lexer_init_token(_CJSON_LEXER_TOKEN* token, char type, const char* text)
{
    token->type = type;
    token->text = text;
    token->next = NULL;
}

// Run scope verification followed by literal verification and lexer token augmentation
// Scope verification: makes sure that the structual tokens represent a well formed json object
// Literal verification: makes sure that all string literals map to a valid json literal type
const int _CJSON_lexer_verify(_CJSON_LEXER_TOKEN* tokens)
{
    const int scope_verification_status = _CJSON_lexer_verify_scopes(tokens);
    const int literal_verification_status = _CJSON_lexer_verify_and_augment_literals(tokens);
    return scope_verification_status && literal_verification_status;
}

// Verify that the structure of the lexed tokes is well formed
const int _CJSON_lexer_verify_scopes(_CJSON_LEXER_TOKEN* tokens)
{
    char scope_stack[1024 * 1024];
    int scope_stack_head = 0;

    _CJSON_LEXER_TOKEN* curr_token = tokens;
    while (curr_token != NULL)
    {
        if (curr_token->type == _CJSON_LEXER_TOKEN_LCB)
        {
            scope_stack[scope_stack_head++] = _CJSON_LEXER_TOKEN_LCB;
        }
        else if (curr_token->type == _CJSON_LEXER_TOKEN_LSB)
        {
            scope_stack[scope_stack_head++] = _CJSON_LEXER_TOKEN_LSB;
        }
        else if (curr_token->type == _CJSON_LEXER_TOKEN_RCB)
        {
            if (scope_stack_head == 0 || scope_stack[scope_stack_head - 1] != _CJSON_LEXER_TOKEN_LCB)
                return 0;
            scope_stack_head--;
        }
        else if (curr_token->type == _CJSON_LEXER_TOKEN_RSB)
        {
            if (scope_stack_head == 0 || scope_stack[scope_stack_head - 1] != _CJSON_LEXER_TOKEN_LSB)
                return 0;
            scope_stack_head--;
        }
        curr_token = curr_token->next;
    }

    if (scope_stack_head != 0)
        return 0;

    return 1;
}

// Verify that string literals map to valid literals in json and augments 
// literal lexer tokens with information about their type.
const int _CJSON_lexer_verify_and_augment_literals(_CJSON_LEXER_TOKEN* tokens)
{
    _CJSON_LEXER_TOKEN* curr_token = tokens;
    while (curr_token != NULL)
    {
        if (curr_token->type == _CJSON_LEXER_TOKEN_STR)
        {
            if (strncmp(curr_token->text, "false", 4) == 0)
            {
                curr_token->type = _CJSON_LEXER_TOKEN_STR_FALSE;
            }
            else if (strncmp(curr_token->text, "true", 4) == 0)
            {
                curr_token->type = _CJSON_LEXER_TOKEN_STR_TRUE;
            }
            else if (strncmp(curr_token->text, "null", 4) == 0)
            {
                curr_token->type = _CJSON_LEXER_TOKEN_STR_NULL;
            }
            else 
            {
                char starting_char = curr_token->text[0];
                if (starting_char == '"')
                {
                    const int status = _CJSON_lexer_verify_is_string(curr_token);
                    if (status != 1)
                    {
                        return 0;
                    }
                    curr_token->type = _CJSON_LEXER_TOKEN_STR_STR;
                }
                else if (starting_char == '-' || (starting_char >= '0' && starting_char <= '9'))
                {
                    const int status = _CJSON_lexer_verify_is_num(curr_token);
                    if (status != 1)
                    {
                        return 0;
                    }

                    if (_CJSON_lexer_verify_is_float(curr_token) == 0)
                    {
                        curr_token->type = _CJSON_LEXER_TOKEN_STR_INT;
                    }
                    else 
                    {
                        curr_token->type = _CJSON_LEXER_TOKEN_STR_FLOAT;
                    }
                }
                else 
                {
                    return 0;
                }
            }
        }
            
        curr_token = curr_token->next;
    }

    return 1;
}

// Verify that a string literal represents a json string literal
const int _CJSON_lexer_verify_is_string(_CJSON_LEXER_TOKEN* token)
{
    unsigned long curr_char_i = 0;
    char curr_char = token->text[curr_char_i++];
    if (curr_char != '"')
        return 0;

    curr_char = token->text[curr_char_i++];
    char state = _CJSON_STR_STATE_START;
    while (curr_char != '\0')
    {
        if (state == _CJSON_STR_STATE_START)
        {
            if (curr_char == '\\')
            {
                state = _CJSON_STR_STATE_CTRL;
            }
            else if (curr_char == '"')
            {
                return 1;
            }
            else {
                state = _CJSON_STR_STATE_PLAIN;
            }
        }
        else if (state == _CJSON_STR_STATE_PLAIN)
        {
            if (curr_char == '\\')
            {
                state = _CJSON_STR_STATE_CTRL;
            }
            else if (curr_char == '"')
            {
                return 1;
            }
            // If you're alread in the PLAIN state, you remain in the PLAIN state
        }
        else if (state == _CJSON_STR_STATE_CTRL)
        {
            if (curr_char != '"' && 
                curr_char != '\\' && 
                curr_char != '/' && 
                curr_char != 'b' &&
                curr_char != 'f' &&
                curr_char != 'n' && 
                curr_char != 'r' &&
                curr_char != 't')
            {
                return 0;
            }
            state = _CJSON_STR_STATE_PLAIN;
        }
        else
        {
            return 0;
        }
        curr_char = token->text[curr_char_i++];
    }

    return 0;
}

// Verify that a string literal represents a json numerical type
const int _CJSON_lexer_verify_is_num(_CJSON_LEXER_TOKEN* token)
{
    unsigned long curr_char_i = 0;
    char curr_char = token->text[curr_char_i++];
    
    char state;
    if (curr_char == '-')
    {
        state = _CJSON_NUM_STATE_SIGN;
    }
    else if (curr_char == '0')
    {
        state = _CJSON_NUM_STATE_START_ZERO;
    }
    else if (curr_char >= '0' && curr_char <= '9')
    {
        state = _CJSON_NUM_STATE_START_WHOLE;
    }
    else
    {
        return 0;
    }
    curr_char = token->text[curr_char_i++];

    while (curr_char != '\0')
    {
        if (state == _CJSON_NUM_STATE_SIGN)
        {
            if (curr_char == '0')
            {
                state = _CJSON_NUM_STATE_START_ZERO;
            }
            else if (curr_char >= '1' && curr_char <= '9')
            {
                state = _CJSON_NUM_STATE_START_WHOLE;
            }
            else
            {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_START_ZERO) 
        {
            if (curr_char == '.')
            {
                state = _CJSON_NUM_STATE_POINT;
            }
            else if (curr_char == 'e' || curr_char == 'E')
            {
                state = _CJSON_NUM_STATE_EXP_E;
            }
            else
            {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_START_WHOLE) 
        {
            if (curr_char >= '0' && curr_char <= '9')
            {
                state = _CJSON_NUM_STATE_START_WHOLE_CONT;
            }
            else if (curr_char == '.')
            {
                state = _CJSON_NUM_STATE_POINT;
            }
            else if (curr_char == 'e' || curr_char == 'E')
            {
                state = _CJSON_NUM_STATE_EXP_E;
            }
            else 
            {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_START_WHOLE_CONT) 
        {
            if (curr_char == '.')
            {
                state = _CJSON_NUM_STATE_POINT;
            }
            else if (curr_char == 'e' || curr_char == 'E')
            {
                state = _CJSON_NUM_STATE_EXP_E;
            }
            else if (curr_char < '0' || curr_char > '9')
            {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_POINT) 
        {
            if (curr_char >= '0' && curr_char <= '9')
            {
                state = _CJSON_NUM_STATE_MANTISSA;
            }
            else
            {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_MANTISSA) 
        {
            if (curr_char == 'e' || curr_char == 'E')
            {
                state = _CJSON_NUM_STATE_EXP_E;
            }
            else if (curr_char < '0' || curr_char > '9')
            {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_EXP_E) 
        {
            if (curr_char == '-' || curr_char == '+')
            {
                state = _CJSON_NUM_STATE_EXP_SIGN;
            }
            else if (curr_char >= '0' && curr_char <= '9')
            {
                state = _CJSON_NUM_STATE_EXP_MAG;
            }
            else {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_EXP_SIGN) 
        {
            if (curr_char >= '0' && curr_char <= '9')
            {
                state = _CJSON_NUM_STATE_EXP_MAG;
            }
            else
            {
                return 0;
            }
        }
        else if (state == _CJSON_NUM_STATE_EXP_MAG) 
        {
            if (curr_char < '0' || curr_char > '9')
            {
                return 0;
            }
        }
        else 
        {
            return 0;
        }
        curr_char = token->text[curr_char_i++];
    }

    return 1;
}

// Verify that a string literal is a float literal type in json
const int _CJSON_lexer_verify_is_float(_CJSON_LEXER_TOKEN* token)
{
    unsigned long curr_char_i = 0;
    char curr_char = token->text[curr_char_i++];

    while (curr_char != '\0')
    {
        if (curr_char == '.')
        {
            return 1;
        }
        curr_char = token->text[curr_char_i++];
    }

    return 0;
}