#include "../include/search.h"

/* Binary search for the first CJSON node with a parent value of <parent> */
long CJSON_search_find_first_with_parent(
	CJSON_NODE* nodes,
	long low,
	long high,
	long parent )
{	
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

CJSON_NODE* _CJSON_search( CJSON_NODE* nodes, const long n, const char* keys[] )
{
	long key_i = 0;
	long node_i = 0;
	while ( key_i < n )
	{
		const char* key = keys[ key_i++ ];
		CJSON_NODE* node = &( nodes[ node_i ] );

		if ( node->type == CJSON_NODE_TYPE_OBJ )
		{
			long key_node_i = CJSON_search_find_first_with_parent( nodes, node_i, nodes[0].rev_i + 1 - 1, node->i );
			if ( key_node_i == -1 )
			{
				return CJSON_NULL;
			}
			
			CJSON_NODE* key_node = &( nodes[ key_node_i ] );
			while ( key_node->parent == node->i )
			{
				if ( strncmp( key_node->buf, key, strlen( key ) ) == 0 ) break;
				
				key_node_i++;
				key_node = &( nodes[key_node_i] );
			}
			if ( key_node->parent != node->i )
			{
				return CJSON_NULL;
			}

			node_i = CJSON_search_find_first_with_parent( nodes, key_node_i, nodes[0].rev_i + 1 - 1, key_node->i );
		}
		else if ( node->type == CJSON_NODE_TYPE_ARR )
		{
			if ( !CJSON_IS_NUM( key ) )
			{
				return CJSON_NULL;
			}

			long ind = strtol( key, CJSON_NULL, 10 );
			long element_node_i = CJSON_search_find_first_with_parent( nodes, node_i, nodes[0].rev_i + 1 - 1, node->i );
			node_i = element_node_i + ind;

			 // nodes[0].rev_i + 1 is the pseudo length of nodes
			if ( node_i >= nodes[0].rev_i + 1 || node_i < 0 )
			{
				return CJSON_NULL;
			}
			else if ( nodes[ node_i ].parent != node->i )
			{
				return CJSON_NULL;
			}
		}
		else if ( node->type == CJSON_NODE_TYPE_KEY || node->type == CJSON_NODE_TYPE_VALUE )
		{
			return CJSON_NULL;
		}
	}

	return &( nodes[ node_i ] );
}
