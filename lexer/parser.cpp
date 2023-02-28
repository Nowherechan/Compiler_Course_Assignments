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

int Statement(TokenPtrV &v, int idx) {
    
    int ret = idx;

    ret = SimpleStmt(v, idx);
    if (ret > 0) 
        return ret;

    ret = IfStmt(v, idx);
    if (ret > 0)
        return ret;
    
    ret = WhileStmt(v, idx);
    if (ret > 0)
        return ret;
    
    ret = FuncdefStmt(v, idx);
    if (ret > 0)
        return ret;

    return -1;
}

int SimpleStmt(TokenPtrV &v, int idx) {

    int ret = idx;

    ret = ExpressionStmt(v, idx);
    if (ret > 0)
        return ret;

    ret = ContinueStmt(v, idx);
    if (ret > 0)
        return ret;

    ret = ReturnStmt(v, idx);
    if (ret > 0)
        return ret;

    ret = DeclareStmt(v, idx);
    if (ret > 0)
        return ret;
    
    return -1;
}

int ExpressionStmt(TokenPtrV &v, int idx) {

    int ret = expression(v, idx);

    if (ret < 0)
        return -1;
    
    if (v.size() <= ret || v[ret]->getType() != Token::Semicolon) {
        return -1;
    }
    ret++;

    return ret;
}

int ContinueStmt(TokenPtrV &v, int idx) {
    
    int ret = idx;
    
    if (v.size() <= ret || v[ret]->getType() != Token::Continue) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::Semicolon) {
        return -1;
    }
    ret++;

    return ret;
}

int ReturnStmt(TokenPtrV &v, int idx) {

    int ret = idx;
    
    if (v.size() <= ret || v[ret]->getType() != Token::Return) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::Return) {
        return -1;
    }
    ret++;

    return ret;
}

int DeclareStmt(TokenPtrV &v, int idx) {

    int ret = idx;

    if (v.size() <= ret || v[ret]->getType() != Token::Qualifier) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::Id) {
        return -1;
    }
    ret++;

    // end with ';'
    if (v.size() > ret && v[ret]->getType() == Token::Semicolon) {
        return ret+1;
    }
    
    // not ';'
    if (v.size() <= ret || v[ret]->getType() != Token::Relop) {
        return -1;
    }

    auto pp = (Token::RelopToken *)(v[ret]);
    if (pp->getRelopType() != Token::Assign) {
        return -1;
    }
            
    ret++;

    ret = expression(v, ret);

    if (ret < 0)
        return -1;
    
    if (v.size() <= ret || v[ret]->getType() != Token::Semicolon) {
        return -1;
    }
    ret++;

    return ret;
}

int IfStmt(TokenPtrV &v, int idx) {

    int ret = idx;

    if (v.size() <= ret || v[ret]->getType() != Token::If) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::LeftParen) {
        return -1;
    }
    ret++;

    ret = expression(v, ret);
    if (ret < 0) 
        return -1;

    if (v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    ret = CodeBlock(v, ret);
    if (ret < 0) 
        return -1;

    // there is no 'else'
    if (v.size() <= ret || v[ret]->getType() != Token::Else) {
        return ret;
    }
    ret++;

    // else found
    ret = CodeBlock(v, ret);
    return ret;
}

int WhileStmt(TokenPtrV &v, int idx) {

    int ret = idx;

    if (v.size() <= ret || v[ret]->getType() != Token::While) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::LeftParen) {
        return -1;
    }
    ret++;

    ret = expression(v, ret);
    if (ret < 0) 
        return -1;

    if (v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    return CodeBlock(v, ret);
}

int FuncdefStmt(TokenPtrV &v, int idx) {
    
    int ret = idx;

    if (v.size() <= ret || v[ret]->getType() != Token::Qualifier) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::Id) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::LeftParen) {
        return -1;
    }
    ret++;

    ret = Arglist(v, ret);
    if (ret < 0) 
        return -1;

    if (v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    return CodeBlock(v, ret);
}

int Arglist(TokenPtrV &v, int idx) {

    int ret = idx;

    if (v.size() <= ret || v[ret]->getType() != Token::Qualifier) {
        return -1;
    }
    ret++;

    if (v.size() <= ret || v[ret]->getType() != Token::Id) {
        return -1;
    }
    ret++;

    // comma following
    if (v.size() > ret && v[ret]->getType() == Token::Comma) {
        return Arglist(v, ret);
    }

    return ret;
}

// EXPRESSIONS !

int prim_exp(TokenPtrV &v, int idx) {
    
    int ret = idx;
    
    ret = atom(v, ret);
    if (ret < 0) {
        return -1;
    }

    if (v.size() < ret) {
        return -1;
    }

    if (v.size() == ret) {
        return ret;
    }

    if (v[ret]->getType() == Token::Dot) {
        ret++;
        if (v.size() <= ret || v[ret]->getType() != Token::Id) {
            return -1;
        }
        ret++;
        return ret;

    } else if (v[ret]->getType() == Token::LeftParen) {
        ret++;
        ret = expr_list(v, ret);
        if (ret < 0) 
            return -1;
        if (v.size() <= ret || v[ret]->getType() != Token::RightParen) {
            return -1;
        }
        ret++;
        return ret;

    } else if (v[ret]->getType() == Token::LeftSqBracket) {
        ret++;
        ret = expression(v, ret);
        if (ret < 0) 
            return -1;
        if (v.size() <= ret || v[ret]->getType() != Token::RightSqBracket) {
            return -1;
        }
        ret++;
        return ret;
    } else {
        return -1;
    }
}

int expr_list(TokenPtrV &v, int idx) {
    
    int ret = idx;

    ret = expression(v, ret);
    if (ret < 0) 
        return -1;

    // always match
    int new_ret = ret;
    while (new_ret >= 0) {
        ret = new_ret;
        new_ret = expr_list(v, ret);
    }

    return ret;
}
    
int atom(TokenPtrV &v, int idx) {

    int ret = idx;

    if (v.size() <= ret) 
        return -1;
    
    if (v[ret]->getType() == Token::Id ||
        v[ret]->getType() == Token::DoubleConstant ||
        v[ret]->getType() == Token::IntegerConstant ) {
        ret++;
        return ret;
    }
    
    if (v[ret]->getType() != Token::LeftParen) {
        return -1;
    }

    ret++;
    ret = expression(v, ret);
    if (ret < 0) 
        return -1;
    
    if (v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    return ret;
}

// Calculate