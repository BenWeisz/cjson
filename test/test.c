#include "test_lexer.h"
#include "test_verify.h"
#include "test_parse.h"
#include "test_search.h"

int main()
{
	test_lexer_run_tests();
	test_verify_run_tests();
	test_parse_run_tests();
	test_search_run_tests();
	return 0;
}
