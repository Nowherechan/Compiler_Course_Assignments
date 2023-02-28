#include <vector>
#include <iostream>
#include "parser.h"
#include "preprocessor.h"

typedef std::vector<Token::Token*> TokenPtrV;

int main() {
    preprocess("../testfile");
    TokenPtrV &v = get_tokenlist_ref();
    showTokenList(v);
    std::cout << "Match:\t" << Match::Start(v, 0) << std::endl;
    std::cout << "All:\t" << v.size() << std::endl;
    return 0;
}