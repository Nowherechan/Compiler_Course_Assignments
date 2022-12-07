#include <string.h>
#include <map>
#include "lexer_dfa.h"


static DfaState state = Start;
static char buffer[BUFFER_SIZE];
static unsigned int buffer_end_idx=0;
static std::map<char, SignType> SigleSignTable = {
    {'+', s_Add}, {'-', s_Minus}, {'*', s_Star}, {'/', s_Slash}, 
    {'%', s_Mod}, {'^', s_Xor},
    {'(', s_LeftParen}, {')', s_RightParen},
    {'[', s_LeftSqBracket}, {']', s_RightSqBracket},
    {'{', s_LeftBrace}, {'}', s_RightBrace},
    {',', s_Comma}, {';', s_Semicolon}
};
static std::map<char, RelopType> RelopTable;

int bpush(char ch) {
    if (buffer_end_idx = BUFFER_SIZE) {
        return 1;
    }

    buffer[buffer_end_idx] = ch;
    buffer_end_idx++;
}

inline bool char_isNumber(char ch) {
    return ch >= '0' && ch <= '9';
}

/* subline included */ 
inline bool char_isLetter(char ch) { 
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_');
}

void SignRecognized(SignType signtype) {
    // TODO
}

void NumberRecognized() {
    // TODO
}

void DoubleRecognized() {
    // TODO
}

void StringRecognized() {
    // TODO
}

void SignRecognized(SignType sightype) {
    // TODO
}

void RelopRecognized(RelopType reloptype) {
    // TODO
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
    
    case bitOr:
        state = Start;
        if (ch == '|') {
            SignRecognized(s_Or);
        } else {
            SignRecognized(s_bitOr);
            return 1;
        }

    case bitAnd:
        state = Start;
        if (ch == '&') {
            SignRecognized(s_And);
        } else {
            SignRecognized(s_bitOr);
            return 1;
        }

    case Assign:
        state = Start;
        if (ch == '=') {
            RelopRecognized(r_Eq);
        } else {
            RelopRecognized(r_Assign);
            return 1;
        }
    
    case Neg:
        state = Start;
        if (ch == '=') {
            RelopRecognized(r_Neq);
        } else {
            SignRecognized(s_bitNeg); // Special
            return 1;
        }

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
        
    default:
        break;
    }

    return 0;
}