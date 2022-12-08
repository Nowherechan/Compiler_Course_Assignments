#include <vector>
#include "Token.h"

#ifndef LEXER_DFA_H
#define LEXER_DFA_H

enum DfaState {
    Start,

    Number,
    Double,
    String,
    bitOr,
    bitAnd,
    Assign,
    Neg,
    Less,
    Greater,
    Dot
};

enum SignType {
    s_Add,
    s_Minus,
    s_Star,
    s_Slash,
    s_Mod,
    s_And,
    s_bitAnd,
    s_Or,
    s_bitOr,
    s_Xor,
    s_bitNeg,
    s_LeftShift,
    s_RightShift,
    s_Dot,
    s_LeftParen,
    s_RightParen,
    s_LeftSqBracket,
    s_RightSqBracket,
    s_LeftBrace,
    s_RightBrace,
    s_Comma,
    s_Semicolon
};

enum RelopType {
    r_Assign,
    r_Eq,
    r_Neq,
    r_Lt,
    r_Gt,
    r_Leq,
    r_Geq
};


const unsigned int BUFFER_SIZE = 1024;

std::vector<Token::Token*> &get_tokenlist_ref();

int rec_one_char(char ch);

#endif