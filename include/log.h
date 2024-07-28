#ifndef CJSON_LOG_H
#define CJSON_LOG_H

#include <stdio.h>

#ifdef CJSON_DEBUG
	#define CJSON_LOG( ... ) do { \
			printf( "\e[1;32mLOG:\e[0m "__VA_ARGS__ ); \
		} while( 0 )
	#define CJSON_WARN( ... ) do { \
			printf( "\e[1;33mWARN:\e[0m "__VA_ARGS__ ); \
		} while( 0 )
	#define CJSON_ERROR( ... ) do { \
			printf( "\e[1;31mERROR:\e[0m "__VA_ARGS__ ); \
		} while( 0 )
#else
	#define CJSON_LOG( ... )
	#define CJSON_WARN( ... )
	#define CJSON_ERROR( ... )
#endif

#endif // CJSON_LOG_H
