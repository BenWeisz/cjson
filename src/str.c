#include "../include/str.h"

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