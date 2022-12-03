#include <fstream>
#include <string.h>

int preprocess(const char *file_name);

int preprocess_line(std::string &buf, std::fstream &out, bool &is_comment, const unsigned int line_num, unsigned int &err_st_line);

enum preprocess_mode {
    uncomment,
    unc_slash,
    comment,
    com_star
};