#include "cjson4.h"

int main()
{
    const char* json = "{ \"numbers\": [6.0   \t\t\t\t\n, true, false, null, \"strstr\"]\t\t\t\n    }";
    // const char* bad_json = "{ \"asdf\": 1\t\t\t\t\t\n]";
    _CJSON_LEXER_TOKEN* tokens = _CJSON_lexer_lex_tokens(json);

    const int status = _CJSON_lexer_verify_and_augment_tokens(tokens);
    printf("%d\n", status);

    return 0;
}
