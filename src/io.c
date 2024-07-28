#include "../include/io.h"

unsigned char* IO_read( const char* path, size_t* buf_size )
{
	*buf_size = 0;
	
	FILE* file = fopen( path, "rb" );
	if ( file == NULL )
	{
		CJSON_ERROR( "Failed to open file: %s\n", path );
		return NULL;
	}

	// Determine file size
	int r = fseek( file, 0, SEEK_END );
	if ( r != 0 )
	{
		CJSON_ERROR( "Failed to seek to end of file: %s\n", path );
		fclose( file );
		return NULL;
	}

	long file_size = ftell( file );
	if ( file_size == -1 )
	{
		CJSON_ERROR( "Failed to read file size: %s\n", path );
		fclose( file );
		return NULL;
	}

	r = fseek( file, 0, SEEK_SET );
	if ( r != 0 )
	{
		CJSON_ERROR( "Failed to return to front of file: %s\n", path );
		fclose( file );
		return NULL;
	}

	unsigned char* buf = ( unsigned char* )malloc( sizeof( unsigned char ) * file_size );
	if ( buf == NULL )
	{
		CJSON_ERROR( "Failed to allocate enough room for buffer: %s\n", path );
		fclose( file );
		return NULL;
	}

	size_t bytes_read = fread( buf, sizeof( unsigned char ), file_size, file );
	if ( bytes_read == 0 )
	{
		CJSON_ERROR( "Failed to read full length of file, file size: %ld, bytes read: %lu\n", file_size, bytes_read );
		fclose( file );
		free( buf );
		return NULL;
	}

	*buf_size = bytes_read;
	fclose( file );
	
	return buf;
}
