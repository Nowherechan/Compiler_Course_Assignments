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

void rec_one_char(char ch) {
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
    
    // TODO: Other Cases
    default:
        break;
    }
}