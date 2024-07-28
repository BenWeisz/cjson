#ifndef CJSON_TYPES_H
#define CJSON_TYPES_H

#define CJSON_NODE_TYPE_OBJ		0x00
#define CJSON_NODE_TYPE_ARR		0x01
#define CJSON_NODE_TYPE_KEY		0x02
#define CJSON_NODE_TYPE_VALUE	0x03

struct CJSON_NODE
{
	unsigned char type;
	char* buf;
	long parent;
};

struct CJSON
{
	struct CJSON_NODE* nodes;
	long num_nodes;	
};

#endif // CJSON_TYPES_H
