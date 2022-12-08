#include <vector>
#include "preprocessor.h"

int main() {
    preprocess("../testfile");
    std::vector<Token::Token*> &v = get_tokenlist_ref();
    showTokenList(v);
    return 0;
}