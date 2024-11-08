#ifndef CJSON_STR
#define CJSON_STR

#include "log.h"

/* Character code */
unsigned int CJSON_char_is_whitespace( const char c );

/* String code */
unsigned long CJSON_strtol( const char* str );
unsigned int CJSON_strncmp( const char* str1, const char* str2, unsigned long n );
unsigned long CJSON_strlen( const char* str );

#endif // CJSON_STR