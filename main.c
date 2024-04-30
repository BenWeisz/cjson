#include <stdio.h>
#include "cjson4.h"

int main()
{
    const char* json = "{ \"numbers\": [1,2,3,4\t,5\t\t\t\t\n,6]\t\t\t\n      }";
    _CJSON_LEXER_TOKEN* tokens = _CJSON_lexer_lex_tokens(json);

    return 0;
}
