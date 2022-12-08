#include <vector>
#include "Token.h"
#include "lexer_dfa.h"

#ifndef SHOWTOKENS_H
#define SHOWTOKENS_H

void showSimpleToken(Token::Token* p);

void showTokenList(std::vector<Token::Token*> &v);

#endif