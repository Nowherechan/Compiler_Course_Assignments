enum TokenType {

    Qualifier,
    Id,
    
    Return,
    For,
    Return,
    Break,
    If,
    Else,
    While,
    Continue,

    Comma,
    Semicolon,
    Plus,
    Minus,
    Mod,
    And,
    BitAnd,
    Or,
    BitOr,
    Xor,
    Neg,
    LeftShift,
    RightShift,
    Star,
    Slash,

    IntegerConstant,
    DoubleConstant,

    LeftParen,
    RightParen,
    LeftSqBracket,
    RightSqBracket,
    LeftBrace,
    RightBrace,

    Relop
};

enum QualifierType {
    Int,
    Double,
    Void
};

enum RelopType {
    Assign,
    Eq,
    Neq,
    Gt,
    Geq,
    Lt,
    Leq
};

class Token {
private:
    TokenType token_type;

    unsigned int line;
    unsigned int column;
public:
    Token(unsigned int li, unsigned int col, TokenType tokentype);
    ~Token();
};

class QualifierToken: Token {
private:
    QualifierType qualifier_type;
public:
    QualifierType getQualifierType();
    QualifierToken(unsigned int lin, unsigned int col, QualifierType qualifiertype);
};

class RelopToken: Token {
private:
    RelopType relop_type;
public:
    RelopType getRelopType();
    RelopToken(unsigned int lin, unsigned int col, RelopType reloptype);
};

class IdToken: Token {
private:
    unsigned int id;
public:
    unsigned int getId();
    IdToken(unsigned int lin, unsigned int col, unsigned int _id);
};

class IntConstToken: Token {
private:
    int value;
public:
    int getValue();
    IntConstToken(unsigned int lin, unsigned int col, int val);
};

class DoubleConstToken: Token {
private:
    double value;
public:
    double getValue();
    DoubleConstToken(unsigned int lin, unsigned int col, double val);
};