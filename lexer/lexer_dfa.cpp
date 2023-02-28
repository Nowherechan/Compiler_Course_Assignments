#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include "lexer_dfa.h"
#include "Token.h"


static DfaState state = Start;
static char buffer[BUFFER_SIZE];
static unsigned int buffer_end_idx=0;
static std::vector<Token::Token*> token_list;
static std::map<char, SignType> SigleSignTable = {
    {'+', s_Add}, {'-', s_Minus}, {'*', s_Star}, {'/', s_Slash}, 
    {'%', s_Mod}, {'^', s_Xor},
    {'(', s_LeftParen}, {')', s_RightParen},
    {'[', s_LeftSqBracket}, {']', s_RightSqBracket},
    {'{', s_LeftBrace}, {'}', s_RightBrace},
    {',', s_Comma}, {';', s_Semicolon}
};
static std::map<char, RelopType> RelopTable;
static std::map<enum SignType, enum Token::TokenType> SignToTokenTable = {
    {s_Add, Token::Plus}, {s_Minus, Token::Minus}, {s_Star, Token::Star}, {s_Slash, Token::Slash},
    {s_Mod, Token::Mod}, 
    {s_And, Token::And}, {s_bitAnd, Token::BitAnd}, {s_Or, Token::Or}, {s_bitOr, Token::BitOr},
    {s_Xor, Token::Xor}, {s_bitNeg, Token::Neg}, {s_LeftShift, Token::LeftShift}, {s_RightShift, Token::RightShift},
    {s_LeftParen, Token::LeftParen}, {s_RightParen, Token::RightParen}, {s_Dot, Token::Dot},
    {s_LeftSqBracket, Token::LeftSqBracket}, {s_RightSqBracket, Token::RightSqBracket},
    {s_LeftBrace, Token::LeftBrace}, {s_RightBrace, Token::RightBrace}, {s_Comma, Token::Comma}, {s_Semicolon, Token::Semicolon}
};
static std::map<RelopType, Token::RelopType> RelopToTokenRelopType = {
    {r_Assign, Token::Assign}, 
    {r_Eq, Token::Eq}, {r_Neq, Token::Neq},
    {r_Lt, Token::Lt}, {r_Gt, Token::Gt},
    {r_Leq, Token::Leq}, {r_Geq, Token::Geq}  
};
static std::map<std::string, Token::TokenType> KeyWordTable = {
    {"return", Token::Return}, {"for", Token::For},
    {"break", Token::Break}, {"if", Token::If},
    {"else", Token::Else},
    {"while", Token::While}, {"continue", Token::Continue}
};
static std::map<std::string, Token::QualifierType> QualifierTable = {
    {"int", Token::Int}, {"double", Token::Double}, {"void", Token::Void}
};
static std::map<std::string, unsigned int> IdTable;
static unsigned int IdTable_Cnt = 0;

int bpush(char ch) {
    if (buffer_end_idx == BUFFER_SIZE) {
        return 1;
    }

    buffer[buffer_end_idx] = ch;
    buffer_end_idx++;
    return 0;
}

void idtable_push(std::string str) {
    IdTable[str] = IdTable_Cnt;
    IdTable_Cnt++;
    // TODO
}

inline bool char_isNumber(char ch) {
    return ch >= '0' && ch <= '9';
}

/* subline included */ 
inline bool char_isLetter(char ch) { 
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_');
}

std::pair<unsigned int, unsigned int> get_position() {
    return {0, 0};
    // TODO: first - line, second - col
}

std::vector<Token::Token*> &get_tokenlist_ref() {
    std::vector<Token::Token*> &ref = token_list;
    return ref;
}

void NumberRecognized() {
    bpush('\0');
    int value = std::stoi(buffer, nullptr, 0);
    std::pair<unsigned int, unsigned int> position = get_position();
    Token::IntConstToken *new_int_token = new Token::IntConstToken(position.first, position.second, value);
    Token::Token *new_token = (Token::Token*)new_int_token;
    token_list.push_back(new_token);

    buffer_end_idx = 0;
}

void DoubleRecognized() {
    bpush('\0');
    double value = strtod(buffer, nullptr);
    std::pair<unsigned int, unsigned int> position = get_position();
    Token::DoubleConstToken *new_double_token = new Token::DoubleConstToken(position.first, position.second, value);
    Token::Token *new_token = (Token::Token*)new_double_token;
    token_list.push_back(new_token);

    buffer_end_idx = 0;
}

void StringRecognized() {
    bpush('\0');
    std::string str = buffer;
    std::pair<unsigned int, unsigned int> position = get_position();
    Token::Token* new_token;

    if (KeyWordTable.find(str) != KeyWordTable.end()) {

        Token::TokenType tokentype = KeyWordTable[str];
        new_token = new Token::Token(position.first, position.second, tokentype);

    } else if (QualifierTable.find(str) != QualifierTable.end()) {

        Token::QualifierType qualifier_type = QualifierTable[str];
        Token::QualifierToken* new_qualifier_token = new Token::QualifierToken(position.first, position.second, qualifier_type);
        new_token = (Token::Token*)new_qualifier_token;

    } else if (IdTable.find(str) != IdTable.end()) {

        unsigned int id = IdTable[str];
        Token::IdToken* new_id_token = new Token::IdToken(position.first, position.second, id);
        new_token = (Token::Token*)new_id_token;

    } else {

        idtable_push(str);
        unsigned int id = IdTable[str];
        Token::IdToken* new_id_token = new Token::IdToken(position.first, position.second, id);
        new_token = (Token::Token*)new_id_token;

    }

    token_list.push_back(new_token);

    buffer_end_idx = 0;
}

void SignRecognized(SignType signtype) {
    Token::TokenType tokentype = SignToTokenTable[signtype];
    std::pair<unsigned int, unsigned int> position = get_position();
    Token::Token *new_token = new Token::Token(position.first, position.second, tokentype);
    token_list.push_back(new_token);
}

void RelopRecognized(RelopType reloptype) {
    Token::RelopType _reloptype = RelopToTokenRelopType[reloptype];
    std::pair<unsigned int, unsigned int> position = get_position();
    Token::RelopToken *new_relop_token = new Token::RelopToken(position.first, position.second, _reloptype);
    Token::Token *new_token = (Token::Token*)new_relop_token;
    token_list.push_back(new_token);
}

int rec_one_char(char ch) {
    switch (state)
    {
    case Start:
        if (char_isNumber(ch)) {
            state = Number;
            bpush(ch);
        } else if (char_isLetter(ch)) {
            state = String;
            bpush(ch);
        } else if (SigleSignTable.find(ch) != SigleSignTable.end()) {
            SignRecognized(SigleSignTable[ch]);
        } else {
            switch (ch)
            {
            case '|':
                state = bitOr;
                break;
            case '&':
                state = bitAnd;
                break;
            case '=':
                state = Assign;
                break;
            case '.':
                state = Dot;
                break;
            case '!':
                state = Neg;
                break;
            case '<':
                state = Less;
                break;
            case '>':
                state = Greater;
                break;
            default:
                state = Start;
                break;
            }
        }
        break;
    
    case Number:
        if (char_isNumber(ch)) {
            bpush(ch);
        } else if (ch == '.') {
            state = Double;
            bpush(ch);
        } else {
            NumberRecognized();
            state = Start;
            return 1;
        }
        break;

    case Double:
        if (char_isNumber(ch)) {
            bpush(ch);
        } else {
            DoubleRecognized();
            state = Start;
            return 1;
        }
        break;

    case String:
        if (char_isNumber(ch) || char_isLetter(ch)) {
            bpush(ch);
        } else {
            StringRecognized();
            state = Start;
            return 1;
        }
        break;
    
    case bitOr:
        state = Start;
        if (ch == '|') {
            SignRecognized(s_Or);
        } else {
            SignRecognized(s_bitOr);
            return 1;
        }
        break;

    case bitAnd:
        state = Start;
        if (ch == '&') {
            SignRecognized(s_And);
        } else {
            SignRecognized(s_bitAnd);
            return 1;
        }
        break;

    case Assign:
        state = Start;
        if (ch == '=') {
            RelopRecognized(r_Eq);
        } else {
            RelopRecognized(r_Assign);
            return 1;
        }
        break;
    
    case Neg:
        state = Start;
        if (ch == '=') {
            RelopRecognized(r_Neq);
        } else {
            SignRecognized(s_bitNeg); // Special
            return 1;
        }
        break;

    case Less:
        state = Start;
        if (ch == '=') {
            RelopRecognized(r_Leq);
        } else if (ch == '<') {
            SignRecognized(s_LeftShift);
        } else {
            RelopRecognized(r_Lt);
            return 1;
        }
        break;
    
    case Greater:
        state = Start;
        if (ch == '=') {
            RelopRecognized(r_Geq);
        } else if (ch == '>') {
            SignRecognized(s_RightShift);
        } else {
            RelopRecognized(r_Gt);
            return 1;
        }
        break;

    case Dot:
        if (char_isNumber(ch)) {
            state = Double;
            bpush('0');
            bpush('.');
            bpush(ch);
        } else {
            state = Start;
            SignRecognized(s_Dot);
            return 1;
        }
        break;

    default:
        break;
    }

    return 0;
}