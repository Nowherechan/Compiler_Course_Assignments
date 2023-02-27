#include <vector>
#include "Token.h"

#ifndef PARSER_H
#define PARSER_H

typedef std::vector<Token::Token*> TokenPtrV;

namespace Match {
    int Start(TokenPtrV &v, int idx);
    int CodeLines(TokenPtrV &v, int idx);
    int CodeBlock(TokenPtrV &v, int idx);
    int Statement(TokenPtrV &v, int idx);
    int SimpleStmt(TokenPtrV &v, int idx);
    int ExpressionStmt(TokenPtrV &v, int idx);
    int ContinueStmt(TokenPtrV &v, int idx);
    int ReturnStmt(TokenPtrV &v, int idx);
    int DeclareStmt(TokenPtrV &v, int idx);
    int IfStmt(TokenPtrV &v, int idx);
    int WhileStmt(TokenPtrV &v, int idx);
    int FuncdefStmt(TokenPtrV &v, int idx);
    int Arglist(TokenPtrV &v, int idx);

    // primary expressions
    int prim_exp(TokenPtrV &v, int idx);
    int expr_list(TokenPtrV &v, int idx);
    int atom(TokenPtrV &v, int idx);
    int const_exp(TokenPtrV &v, int idx); // const to const_exp
    
    // expressions
    int expression(TokenPtrV &v, int idx);
    int or_exp(TokenPtrV &v, int idx);
    int and_exp(TokenPtrV &v, int idx);
    int neg_exp(TokenPtrV &v, int idx);
    int relop_exp(TokenPtrV &v, int idx);
    int bitor_exp(TokenPtrV &v, int idx);
    int bitxor_exp(TokenPtrV &v, int idx);
    int bitand_exp(TokenPtrV &v, int idx);
    int bitsft_exp(TokenPtrV &v, int idx);
    int add_exp(TokenPtrV &v, int idx);
    int mult_exp(TokenPtrV &v, int idx);
};

#endif
