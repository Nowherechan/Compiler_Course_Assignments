#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "Token.h"
using namespace Token;


std::map<Token::TokenType, std::string> type_table = {
    {Qualifier, "Qualifier"},
    {Id, "Id"},
    {Return, "Return"},
    {For, "For"},
    {Break, "Break"},
    {If, "If"},
    {Else, "Else"},
    {While, "While"},
    {Continue, "Continue"},
    {Comma, "\',\'"},
    {Semicolon, "\';\'"},
    {Plus, "\'+\'"},
    {Minus, "\'-\'"},
    {Mod, "\'%\'"},
    {And, "\'&&\'"},
    {BitAnd, "\'&\'"},
    {Or, "\'||\'"},
    {BitOr, "\'|\'"},
    {Xor, "\'^\'"},
    {Neg, "\'!\'"},
    {LeftShift, "\'<<\'"},
    {RightShift, "\'>>\'"},
    {Star, "\'*\'"},
    {Slash, "\'/\'"},
    {IntegerConstant, "IntegerConst"},
    {DoubleConstant, "DoubleConst"},
    {LeftParen, "\'(\'"},
    {RightParen, "\')\'"},
    {LeftSqBracket, "\'[\'"},
    {RightSqBracket, "\']\'"},
    {LeftBrace, "\'{\'"},
    {RightBrace, "\'}\'"},
    {Relop, "Relop"}
};

std::map<Token::QualifierType, std::string> qualifier_table = {
    {Int, "int"}, {Double, "double"}, {Void, "void"}
};

std::map<Token::RelopType, std::string> relop_table = {
    {Assign, "assign"}, {Eq, "equal to"}, {Neq, "not equal to"},
    {Gt, "greater than"}, {Geq, "greater or equal"}, {Lt, "less than"}, {Leq, "less or equal"}
};

void showSimpleToken(Token::Token* p) {
    std::cout << "{ type: " 
              << type_table[p->getType()] 
              << " }" << std::endl;
}

void showQualifierToken(Token::Token* p) {
    auto pp = (Token::QualifierToken *)p;
    std::cout << "{ type: Qualifier,\n  q_type: " 
              << qualifier_table[pp->getQualifierType()] 
              << " }" << std::endl;
}

void showRelopToken(Token::Token* p) {
    auto pp = (Token::RelopToken *)p;
    std::cout << "{ type: Relop,\n  r_type: " 
              << relop_table[pp->getRelopType()] 
              << " }" << std::endl;
}

void showIntConstToken(Token::Token* p) {
    auto pp = (Token::IntConstToken *)p;
    std::cout << "{ type: IntConst,\n  value: " 
              << pp->getValue()
              << " }" << std::endl;
}

void showDoubleConstToken(Token::Token* p) {
    auto pp = (Token::DoubleConstToken *)p;
    std::cout << "{ type: DoubleConst,\n  value: " 
              << pp->getValue()
              << " }" << std::endl;
}

void showIdToken(Token::Token* p) {
    auto pp = (Token::IdToken *)p;
    std::cout << "{ type: Identifier,\n  id: " 
              << pp->getId()
              << " }" << std::endl;
}

void showTokenList(std::vector<Token::Token*> &v) {
    for (auto p: v) {
        auto token_type = p->getType();
        switch (token_type)
        {
        case Qualifier:
            showQualifierToken(p);
            break;
        
        case Relop:
            showRelopToken(p);
            break;
        
        case IntegerConstant:
            showIntConstToken(p);
            break;
        
        case DoubleConstant:
            showDoubleConstToken(p);
            break;
        
        case Id:
            showIdToken(p);
            break;
        
        default:
            showSimpleToken(p);
            break;
        }
        std::cout << std::endl;
    }
}
