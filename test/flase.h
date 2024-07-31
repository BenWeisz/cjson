#ifndef FLASE_H
#define FLASE_H

#ifndef FLASE_PRINT
#include <stdio.h>
#define FLASE_PRINT printf
#endif

#define FLASE_DEFINE_TEST( name ) void flase_test_##name( unsigned int* an_ptr, unsigned int* pn_ptr )

#define FLASE_RUN_TEST( name ) \
	({ unsigned int flase_test_pn_##name = 0; \
		unsigned int flase_test_an_##name = 0; \
		do { \
			FLASE_PRINT( "FLASE Run Test: " #name ); \
			flase_test_##name( &flase_test_an_##name, &flase_test_pn_##name ); \
			if ( flase_test_pn_##name == flase_test_an_##name ) \
			{ \
				FLASE_PRINT( " | \e[1;32mPASS\e[0m [\e[1;32m%d/%d\e[0m]\n", flase_test_pn_##name, flase_test_an_##name ); \
			} \
			else \
			{ \
				FLASE_PRINT( "FLASE Error Report: " #name " | "); \
				FLASE_PRINT( "\e[1;31mFAIL\e[0m [\e[1;31m%d/%d\e[0m]\n", flase_test_pn_##name, flase_test_an_##name ); \
			} \
		} while(0); \
	})

#define FLASE_ASSERT( cond, msg ) \
	({  *an_ptr += 1; \
		do { \
			if ( !( cond ) ) \
			{ \
				FLASE_PRINT( "\n\t\e[1;31mFailed:\e[0m %s, %s:%d\n", (msg), __FILE__, __LINE__ ); \
			} \
			else \
			{ \
				*pn_ptr += 1; \
			} \
		} while(0); \
	})

#endif // FLASE_H
