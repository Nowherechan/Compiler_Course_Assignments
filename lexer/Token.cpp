#include "Token.h"

Token::Token::Token(unsigned int lin, unsigned int col, TokenType tokentype): 
    line(lin), 
    column(col), 
    token_type(tokentype) {

}

Token::TokenType Token::Token::getType() {
    return this->token_type;
}

Token::Token::~Token()
{
}

Token::QualifierToken::QualifierToken(unsigned int lin, unsigned int col, QualifierType qualifiertype):
    Token(lin, col, Qualifier),
    qualifier_type(qualifiertype) {
    
}

Token::QualifierType Token::QualifierToken::getQualifierType() {
    return this->qualifier_type;
}

Token::RelopToken::RelopToken(unsigned int lin, unsigned int col, RelopType reloptype):
    Token(lin, col, Relop),
    relop_type(reloptype) {

}

Token::RelopType Token::RelopToken::getRelopType() {
    return this->relop_type;
}

Token::IdToken::IdToken(unsigned int lin, unsigned int col, unsigned int _id):
    Token(lin, col, Id),
    id(_id) {

}

unsigned int Token::IdToken::getId() {
    return this->id;
}

Token::IntConstToken::IntConstToken(unsigned int lin, unsigned int col, int val):
    Token(lin, col, IntegerConstant),
    value(val) {

}

int Token::IntConstToken::getValue() {
    return this->value;
}

Token::DoubleConstToken::DoubleConstToken(unsigned int lin, unsigned int col, double val):
    Token(lin, col, DoubleConstant),
    value(val) {

}

double Token::DoubleConstToken::getValue() {
    return this->value;
}