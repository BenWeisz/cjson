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

    // Just to be pedantic :)
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
            buf_token != 0x0A &&
            buf_token != 0x0D) return;

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

void _CJSON_lexer_init_token(_CJSON_LEXER_TOKEN* token, char type, const char* text)
{
    token->type = type;
    token->text = text;
    token->next = NULL;
}

const int _CJSON_lexer_verify(_CJSON_LEXER_TOKEN* tokens)
{
    return 0;
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
