#ifndef CJSON_LOG_H
#define CJSON_LOG_H

#include "config.h"

#if CJSON_USE_STDLIB
	#include <stdio.h>
	#define CJSON_PRINT( ... ) printf( __VA_ARGS__ )
#else
	#ifndef CJSON_PRINT
	// #error You didn't define the CJSON_PRINT(x) function but you have CJSON_USE_STDLIB off
	// TODO: Remove this later
	#include <stdio.h>
	#define CJSON_PRINT( ... ) printf( __VA_ARGS__ )
	#endif
#endif // CJSON_USE_STDLIB

#ifdef CJSON_DEBUG
	#define CJSON_LOG( ... ) do { \
			CJSON_PRINT( "\e[1;32mLOG:\e[0m "__VA_ARGS__ ); \
		} while( 0 )
	#define CJSON_WARN( ... ) do { \
			CJSON_PRINT( "\e[1;33mWARN:\e[0m "__VA_ARGS__ ); \
		} while( 0 )
	#define CJSON_ERROR( ... ) do { \
			CJSON_PRINT( "\e[1;31mERROR:\e[0m "__VA_ARGS__ ); \
		} while( 0 )
#else
	#define CJSON_LOG( ... )
	#define CJSON_WARN( ... )
	#define CJSON_ERROR( ... )
#endif

#endif // CJSON_LOG_H
