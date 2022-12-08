#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "Token.h"
using namespace Token;


std::map<Token::TokenType, std::string> typetable = {
    {Qualifier, "Qualifier"},
    {Id, "Id"},
    {Return, "Return"},
    {For, "For"},
    {Break, "Break"},
    {If, "If"},
    {Else, "Else"},
    {While, "While"},
    {Continue, "Continue"},
    {Comma, "Comma"},
    {Semicolon, "Semicolon"},
    {Plus, "Plus"},
    {Minus, "Minus"},
    {Mod, "Mod"},
    {And, "And"},
    {BitAnd, "BitAnd"},
    {Or, "Or"},
    {BitOr, "BitOr"},
    {Xor, "Xor"},
    {Neg, "Neg"},
    {LeftShift, "LeftShift"},
    {RightShift, "RightShift"},
    {Star, "Star"},
    {Slash, "Slash"},
    {IntegerConstant, "IntegerConstant"},
    {DoubleConstant, "DoubleConstant"},
    {LeftParen, "LeftParen"},
    {RightParen, "RightParen"},
    {LeftSqBracket, "LeftSqBracket"},
    {RightSqBracket, "RightSqBracket"},
    {LeftBrace, "LeftBrace"},
    {RightBrace, "RightBrace"},
    {Relop, "Relop"}
};

void showSimpleToken(Token::Token* p) {
    std::cout << "<type: " << typetable[p->getType()] << ">" << std::endl;
}

void showTokenList(std::vector<Token::Token*> &v) {
    for (auto x: v) {
        showSimpleToken(x);
    }
}
