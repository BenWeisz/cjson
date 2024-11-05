#ifndef CJSON_LOG_H
#define CJSON_LOG_H

#include "config.h"

#include <stdio.h>
#define CJSON_PRINT( ... ) printf( __VA_ARGS__ )

#ifdef CJSON_DEBUG
    #define CJSON_LOG( ... ) do { \
            CJSON_PRINT( "\e[1;32mLOG:\e[0m | [%s:%d] : ", __FILE__, __LINE__ ); \
            CJSON_PRINT( __VA_ARGS__ ); \
        } while( 0 )
    #define CJSON_WARN( ... ) do { \
            CJSON_PRINT( "\e[1;33mWARN:\e[0m | [%s:%d] : ", __FILE__, __LINE__ ); \
            CJSON_PRINT( __VA_ARGS__ ); \
        } while( 0 )
    #define CJSON_ERROR( ... ) do { \
            CJSON_PRINT( "\e[1;31mERROR\e[0m | [%s:%d] : ", __FILE__, __LINE__ ); \
            CJSON_PRINT( __VA_ARGS__ ); \
        } while( 0 )
#else
    #define CJSON_LOG( ... )
    #define CJSON_WARN( ... )
    #define CJSON_ERROR( ... )
#endif

#endif // CJSON_LOG_H
