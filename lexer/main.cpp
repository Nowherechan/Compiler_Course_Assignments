#include <vector>
#include <iostream>
#include "parser.h"
#include "preprocessor.h"

typedef std::vector<Token::Token*> TokenPtrV;

int main() {
    preprocess("../testfile");
    TokenPtrV &v = get_tokenlist_ref();
    showTokenList(v);
    int parser_ret = Match::Start(v, 0);
    Match::reportResult(v, parser_ret);
    return 0;
}