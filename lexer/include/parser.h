#include <vector>
#include "Token.h"

#ifndef PARSER_H
#define PARSER_H

typedef std::vector<Token::Token*> TokenPtrV;

namespace Match {
    int Start(TokenPtrV &v, int idx);
    int getMaxRet();
};

#endif
