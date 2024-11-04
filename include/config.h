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
    #define CJSON_PARSE_QUEUE_SIZE              CJSON_LEXER_TOKEN_BUFFER_SIZE
#endif // CJSON_PARSE_QUEUE_SIZE

#endif // CJSON_CONFIG_H