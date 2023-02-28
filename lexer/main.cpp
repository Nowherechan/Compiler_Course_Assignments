#include <vector>
#include <iostream>
#include "parser.h"
#include "preprocessor.h"

int main() {
    preprocess("../testfile");
    std::vector<Token::Token*> &v = get_tokenlist_ref();
    showTokenList(v);
    std::cout << Match::Start(v, 0) << std::endl;
    return 0;
}