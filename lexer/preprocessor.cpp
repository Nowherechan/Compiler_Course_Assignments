#include <iostream>
#include <fstream>
#include <string.h>
#include "preprocessor.h"
#include "lexer_dfa.h"

int preprocess(const char *file_name) {
    std::fstream src (file_name, std::ios::in);
    std::fstream out ("./preprocess_output", std::ios::out);
    std::string buf;
    unsigned int comment_err_start_line_num = 0, line_num = 0;
    bool is_comment = false;
    while (1) {
        line_num++;
        getline(src, buf);
        preprocess_line(buf, out, is_comment, line_num, comment_err_start_line_num);
        if (src.eof()) {
            src.close();
            out.close();
            break;
        }
    }
    if (is_comment) {
        std::cerr << "\033[1m" << "line "<< comment_err_start_line_num << 
            ": \033[31merror: \033[0munterminated comment" << std::endl;
        return 1;
    }
    return 0;
}

int preprocess_line(std::string &buf, std::fstream &out, bool &is_comment, const unsigned int line_num, unsigned int &err_st_line) {
    preprocess_mode mode = is_comment ? comment : uncomment;
    bool end_line_comment = false;
    for (auto ch: buf) {
        switch (mode)
        {
        case uncomment:
            if (ch == '/') {
                mode = unc_slash;
            } else {
                out << ch;
                if (rec_one_char(ch))
                    rec_one_char(ch); // TODO
            }
            break;
        
        case unc_slash:
            if (ch == '*') {
                out << ' ' << ' ';
                if (rec_one_char(' '))
                    rec_one_char(' ');
                if (rec_one_char(' '))
                    rec_one_char(' '); // TODO
                mode = comment;
                is_comment = true;
                err_st_line = line_num;
            } else if (ch == '/'){
                end_line_comment = true;
            } else {
                out << '/' << ch;
                if (rec_one_char('/'))
                    rec_one_char('/'); 
                if (rec_one_char(ch))
                    rec_one_char(ch); // TODO
                mode = uncomment;
            }
            break;

        case comment:
            if (ch == '*') {
                mode = com_star;
            } else {
                out << ' ';
                if (rec_one_char(' '))
                    rec_one_char(' '); // TODO
            }
            break;
        
        case com_star:
            out << ' ' << ' ';
            if (rec_one_char(' '))
                rec_one_char(' ');
            if (rec_one_char(' '))
                rec_one_char(' '); // TODO
            if (ch =='/') {
                mode = uncomment;
                is_comment = false;
            } else if (ch == '*') {
                // do nothing
            } else {
                mode = comment;
            }
            break;
            
        default:
            break;
        }

        if (end_line_comment) {
            break;
        }
    }
    out << '\n';
    if (rec_one_char('\n'))
        rec_one_char('\n'); // TODO
    return 0;
}