#include "../include/search.h"

/* Binary search for the first CJSON node with a parent value of <parent> */
long CJSON_search_find_first_with_parent(
	const CJSON* cjson,
	long parent )
{
	long low = parent;
	long high = cjson->num_nodes - 1;

	CJSON_NODE* nodes = cjson->nodes;
	
	while ( low <= high )
	{
		long mid = ( low + high ) / 2;
		long mid_v = nodes[ mid ].parent;

		if ( mid_v < parent )
		{
			low = mid + 1;
		}
		else if ( mid > 0 && nodes[ mid - 1 ].parent >= parent )
		{
			high = mid - 1;
		}
		else if ( mid_v == parent )
		{
			return mid;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

CJSON_NODE* _CJSON_search( const CJSON* cjson, const long n, const char* keys[] )
{
	long key_i = 0;
	long node_i = 0;
	CJSON_NODE* nodes = cjson->nodes;
	while ( key_i < n )
	{
		const char* key = keys[ key_i++ ];
		CJSON_NODE* node = &( nodes[ node_i ] );

		if ( node->type == CJSON_NODE_TYPE_OBJ )
		{
			long parent_i = node_i;
			long key_node_i = CJSON_search_find_first_with_parent( cjson, node_i );
			if ( key_node_i == -1 )
			{
				return NULL;
			}
			
			CJSON_NODE* key_node = &( nodes[ key_node_i ] );
			while ( key_node->parent == parent_i )
			{
				if ( strncmp( key_node->buf, key, strlen( key ) ) == 0 )
				{
					node_i = key_node_i;
					node_i = CJSON_search_find_first_with_parent( cjson, node_i );
					if ( node_i == -1 )
					{
						return NULL;
					}
					break;
				}
				key_node_i++;
				key_node = &( nodes[ key_node_i ] );
			}
			if ( key_node->parent != parent_i )
			{
				return NULL;
			}
		}
		else if ( node->type == CJSON_NODE_TYPE_ARR )
		{
			if ( !CJSON_IS_NUM( key ) )
			{
				return NULL;
			}

			long ind = strtol( key, NULL, 10 );
			long parent_i = node_i;
			long element_node_i = CJSON_search_find_first_with_parent( cjson, node_i  );
			node_i = element_node_i + ind;
			if ( node_i >= cjson->num_nodes || node_i < 0 )
			{
				return NULL;
			}
			else if ( nodes[ node_i ].parent != parent_i )
			{
				return NULL;
			}
		}
		else if ( node->type == CJSON_NODE_TYPE_KEY || node->type == CJSON_NODE_TYPE_VALUE )
		{
			return NULL;
		}
	}

	return &( nodes[ node_i ] );
}
