#include <vector>
#include <iostream>
#include "parser.h"

using namespace Match;

static int max_ret = 0;

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



int Match::Start(TokenPtrV &v, int idx) {
    return CodeLines(v, idx);
}

int Match::getMaxRet() {
    return max_ret;
}

void Match::reportResult(TokenPtrV &v, int ret) {
    int n = v.size();
    if (ret == n) {
        std::cout << "Match Success!" << std::endl;
        return;
    }

    if (max_ret == n) {
        std::cerr << "\033[1m" << "Token "<< max_ret-1 << 
            ": \033[31merror: \033[0mmatch failed" << std::endl;
    } else {
        std::cerr << "\033[1m" << "Token "<< max_ret << 
            ": \033[31merror: \033[0mmatch failed" << std::endl;
    }
}

int CodeBlock(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::LeftBrace)
        return -1;
    ret++;
    ret = CodeLines(v, ret);
    if ((int)v.size() <= ret || v[ret]->getType() != Token::RightBrace)
        return -1;
    ret++;
    return ret;
}

int CodeLines(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx; 
    if ((int)v.size() <= ret)
        return ret;

    if ((int)v.size() > ret)
        ret = Statement(v, ret);
    if (ret < 0) 
        return ret;
    
    // always success
    int new_ret = CodeLines(v, ret);
    ret = new_ret > ret ? new_ret : ret;

    return ret;
}

int Statement(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

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

    max_ret = max_ret > idx ? max_ret : idx;

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

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = expression(v, idx);

    if (ret < 0)
        return -1;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Semicolon) {
        return -1;
    }
    ret++;

    return ret;
}

int ContinueStmt(TokenPtrV &v, int idx) {
    
    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Continue) {
        return -1;
    }
    ret++;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Semicolon) {
        return -1;
    }
    ret++;

    return ret;
}

int ReturnStmt(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Return) {
        return -1;
    }
    ret++;

    ret = expression(v, ret);
    if (ret < 0) 
        return -1;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Semicolon) {
        return -1;
    }
    ret++;

    return ret;
}

int DeclareStmt(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Qualifier) {
        return -1;
    }
    ret++;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Id) {
        return -1;
    }
    ret++;

    // end with ';'
    if ((int)v.size() > ret && v[ret]->getType() == Token::Semicolon) {
        return ret+1;
    }
    
    // not ';'
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Relop) {
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
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Semicolon) {
        return -1;
    }
    ret++;

    return ret;
}

int IfStmt(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::If) {
        return -1;
    }
    ret++;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::LeftParen) {
        return -1;
    }
    ret++;

    ret = expression(v, ret);
    if (ret < 0) 
        return -1;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    ret = CodeBlock(v, ret);
    if (ret < 0) 
        return -1;

    // there is no 'else'
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Else) {
        return ret;
    }
    ret++;

    // else found
    ret = CodeBlock(v, ret);
    return ret;
}

int WhileStmt(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::While) {
        return -1;
    }
    ret++;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::LeftParen) {
        return -1;
    }
    ret++;

    ret = expression(v, ret);
    if (ret < 0) 
        return -1;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    return CodeBlock(v, ret);
}

int FuncdefStmt(TokenPtrV &v, int idx) {
    
    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Qualifier) {
        return -1;
    }
    ret++;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Id) {
        return -1;
    }
    ret++;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::LeftParen) {
        return -1;
    }
    ret++;

    ret = Arglist(v, ret);
    if (ret < 0) 
        return -1;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    return CodeBlock(v, ret);
}

int Arglist(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Qualifier) {
        return ret;
    } // Might be empty
    ret++;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Id) {
        return -1;
    }
    ret++;

    // comma following
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Comma) {
        return ret;
    }
    ret++;

    int new_ret = Arglist(v, ret);
    ret = new_ret > ret ? new_ret : ret;

    return ret;
}

// EXPRESSIONS !

int prim_exp(TokenPtrV &v, int idx) {
    
    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;
    
    ret = atom(v, ret);
    if (ret < 0) {
        return -1;
    }

    if ((int)v.size() < ret) {
        return -1;
    }

    if ((int)v.size() == ret) {
        return ret;
    }

    if (v[ret]->getType() == Token::Dot) {
        ret++;
        if ((int)v.size() <= ret || v[ret]->getType() != Token::Id) {
            return -1;
        }
        ret++;
        return ret;

    } else if (v[ret]->getType() == Token::LeftParen) {
        ret++;
        ret = expr_list(v, ret);
        if (ret < 0) 
            return -1;
        if ((int)v.size() <= ret || v[ret]->getType() != Token::RightParen) {
            return -1;
        }
        ret++;
        return ret;

    } else if (v[ret]->getType() == Token::LeftSqBracket) {
        ret++;
        ret = expression(v, ret);
        if (ret < 0) 
            return -1;
        if ((int)v.size() <= ret || v[ret]->getType() != Token::RightSqBracket) {
            return -1;
        }
        ret++;
        return ret;
    } else {
        return ret;
    }
}

int expr_list(TokenPtrV &v, int idx) {
    
    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    ret = expression(v, ret);
    if (ret < 0) 
        return -1;

    if ((int)v.size() <= ret || v[ret]->getType() != Token::Comma) {
        return ret;
    }
    ret++;

    // always match
    int new_ret = expr_list(v, ret);
    ret = new_ret > ret ? new_ret : ret;

    return ret;
}
    
int atom(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = idx;

    if ((int)v.size() <= ret) 
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
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::RightParen) {
        return -1;
    }
    ret++;

    return ret;
}

// Calculate
int expression(TokenPtrV &v, int idx) {
    return or_exp(v, idx);
}

int or_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = and_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Or) {
        return ret;
    }

    ret++;

    return or_exp(v, ret);
}

int and_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = neg_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::And) {
        return ret;
    }

    ret++;

    return and_exp(v, ret);
}

int neg_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    if ((int)v.size() <= idx) {
        return -1;
    }

    int ret = idx;
    if (v[idx]->getType() == Token::Neg) {
        return neg_exp(v, ret+1);
    }

    return relop_exp(v, ret);
}

int relop_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = bitor_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Relop) {
        return ret;
    }

    ret++;

    return relop_exp(v, ret);
}

int bitor_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = bitxor_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::BitOr) {
        return ret;
    }

    ret++;

    return bitor_exp(v, ret);
}

int bitxor_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = bitand_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::Xor) {
        return ret;
    }

    ret++;

    return bitxor_exp(v, ret);
}

int bitand_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = bitsft_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || v[ret]->getType() != Token::BitAnd) {
        return ret;
    }

    ret++;

    return bitand_exp(v, ret);
}

int bitsft_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = add_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || 
        (v[ret]->getType() != Token::LeftShift && 
         v[ret]->getType() != Token::RightShift)) {
        return ret;
    }

    ret++;

    return bitsft_exp(v, ret);
}

int add_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = mult_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || 
        (v[ret]->getType() != Token::Plus && 
         v[ret]->getType() != Token::Minus)) {
        return ret;
    }

    ret++;

    return add_exp(v, ret);
}

int mult_exp(TokenPtrV &v, int idx) {

    max_ret = max_ret > idx ? max_ret : idx;

    int ret = prim_exp(v, idx);
    if (ret < 0) 
        return -1;
    
    if ((int)v.size() <= ret || 
        (v[ret]->getType() != Token::Star &&
         v[ret]->getType() != Token::Slash &&
         v[ret]->getType() != Token::Mod)) {
        return ret;
    }

    ret++;

    return mult_exp(v, ret);
}