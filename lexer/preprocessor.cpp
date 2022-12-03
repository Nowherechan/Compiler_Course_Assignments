#include <iostream>
#include <fstream>
#include <string.h>
#include "preprocessor.h"

int preprocess(const char *file_name) {
    std::fstream src (file_name, std::ios::in);
    std::fstream out ("./preprocess_output", std::ios::out);
    std::string buf;
    unsigned int line_num = 1;
    bool is_comment = false;
    while (1) {
        getline(src, buf);
        preprocess_line(buf, out, is_comment);
        if (src.eof()) {
            src.close();
            out.close();
            break;
        }
    }
    return 0;
}

int preprocess_line(std::string &buf, std::fstream &out, bool &is_comment) {
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
            }
            break;
        
        case unc_slash:
            if (ch == '*') {
                out << ' ' << ' ';
                mode = comment;
                is_comment = true;
            } else if (ch == '/'){
                end_line_comment = true;
            } else {
                out << '/' << ch;
                mode = uncomment;
            }
            break;

        case comment:
            if (ch == '*') {
                mode = com_star;
            } else {
                out << ' ';
            }
            break;
        
        case com_star:
            out << ' ' << ' ';
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
    return 0;
}
