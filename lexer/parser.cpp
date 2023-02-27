#include <vector>
#include "parser.h"

using namespace Match;

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



int Start(TokenPtrV &v, int idx) {
    return CodeLines(v, idx);
}

int CodeBlock(TokenPtrV &v, int idx) {
    int ret = idx;

    if (v.size() <= ret || v[ret]->getType() != Token::LeftBrace)
        return -1;
    ret++;
    ret = CodeLines(v, ret);
    if (v.size() <= ret || v[ret]->getType() != Token::LeftBrace)
        return -1;
    ret++;
    return ret;
}

int CodeLines(TokenPtrV &v, int idx) {
    
    int ret = idx; 
    if (v.size() > ret)
        ret = Statement(v, ret);
    if (ret < 0) 
        return ret;
    
    // always success
    int new_ret = ret;
    while (new_ret > 0) {
        ret = new_ret;
        new_ret = CodeLines(v, new_ret);
    }

    return ret;
}
    