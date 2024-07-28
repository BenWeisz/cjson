#ifndef CJSON_SEARCH_H
#define CJSON_SEARCH_H

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "types.h"

long CJSON_search_find_first_with_parent(
	const struct CJSON* cjson,
	long parent );

struct CJSON_NODE* _CJSON_search( const struct CJSON* cjson, const long n, const char* keys[] );

// Credits: Tim Schaeffer: https://stackoverflow.com/questions/3272444/use-variadic-functions-in-c89-without-passing-number-of-arguments-or-a-final-arg 
#define CJSON_search( cjson, ... ) \
	({ \
		struct CJSON_NODE* r; \
		do { \
			const char* keys[] = { __VA_ARGS__ }; \
			r = _CJSON_search( ( cjson ), sizeof( keys ) / sizeof( const char* ), keys ); \
		} while( 0 ); \
		r; \
	 })

#define CJSON_IS_NUM( x ) ( ( x )[ 0 ] >= '0' && ( x )[ 0 ] <= '9' )

#endif // CJSON_SEARCH_H
