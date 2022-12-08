#include "Token.h"

Token::Token(unsigned int li, unsigned int col, TokenType tokentype): 
    line(li), 
    column(col), 
    token_type(tokentype) {

}

Token::~Token()
{
}

QualifierToken::QualifierToken(unsigned int lin, unsigned int col, QualifierType qualifiertype):
    Token(lin, col, Qualifier),
    qualifier_type(qualifiertype) {
    
}

QualifierType QualifierToken::getQualifierType() {
    return this->qualifier_type;
}

RelopToken::RelopToken(unsigned int lin, unsigned int col, RelopType reloptype):
    Token(lin, col, Relop),
    relop_type(reloptype) {

}

RelopType RelopToken::getRelopType() {
    return this->relop_type;
}

IdToken::IdToken(unsigned int lin, unsigned int col, unsigned int _id):
    Token(lin, col, Id),
    id(_id) {

}

unsigned int IdToken::getId() {
    return this->id;
}

IntConstToken::IntConstToken(unsigned int lin, unsigned int col, int val):
    Token(lin, col, IntegerConstant),
    value(val) {

}

int IntConstToken::getValue() {
    return this->value;
}

DoubleConstToken::DoubleConstToken(unsigned int lin, unsigned int col, double val):
    Token(lin, col, DoubleConstant),
    value(val) {

}

double DoubleConstToken::getValue() {
    return this->value;
}