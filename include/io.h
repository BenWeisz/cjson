#ifndef CJSON_IO_H
#define CJSON_IO_H

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

#define CJSON_RESOURCE(x) CJSON_RESOURCE_PATH x 

unsigned char* IO_read( const char* path, size_t* buf_size );

#endif // CJSON_IO_H
