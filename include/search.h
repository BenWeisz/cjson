#ifndef CJSON_SEARCH_H
#define CJSON_SEARCH_H

#include "config.h"
#include "types.h"

#if CJSON_USE_STDLIB
	#include <stdlib.h>
	#include <string.h>
#endif // CJSON_USE_STDLIB

long CJSON_search_find_first_with_parent(
	CJSON_NODE* nodes,
	long low,
	long high,
	long parent );

CJSON_NODE* _CJSON_search( CJSON_NODE* nodes, const long n, const char* keys[] );

// Credits: Tim Schaeffer: https://stackoverflow.com/questions/3272444/use-variadic-functions-in-c89-without-passing-number-of-arguments-or-a-final-arg 
#define CJSON_search( nodes, ... ) \
	({ \
		CJSON_NODE* r; \
		do { \
			const char* keys[] = { __VA_ARGS__ }; \
			r = _CJSON_search( nodes, sizeof( keys ) / sizeof( const char* ), keys ); \
		} while( 0 ); \
		r; \
	 })

#define CJSON_IS_NUM( x ) ( ( ( x )[ 0 ] >= '0' && ( x )[ 0 ] <= '9' ) || ( x )[ 0 ] == '-' )

#endif // CJSON_SEARCH_H
