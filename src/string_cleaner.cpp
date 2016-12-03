#include <algorithm>
#include <sstream>

#include "string_cleaner.hpp"

std::vector<std::string> split(std::string s,char c = ' ') {
    std::stringstream aux(s);
    std::string token;
    std::vector<std::string> res;

    while(std::getline(aux,token,c)) {
        res.push_back(token);
    }

    return res;
}

void replace_tab_by_space(std::string& s) {
     std::replace( s.begin(), s.end(), '\t', ' ');
}

void whitespace_clean(std::string& s) {
    replace_tab_by_space(s);
    whitespace_compress(s);
    whitespace_trim(s);
}

void whitespace_compress(std::string& s) {
    auto remove_condition =
    [](char l, char r) {
        return std::isspace(l) && std::isspace(r);
    };

    auto new_string_end = std::unique(s.begin(), s.end(), remove_condition);
    s.erase(new_string_end, s.end());
}


void whitespace_trim(std::string &s) {
    auto is_whitespace =
    [](int c) {
        return std::isspace(c);
    };

    auto whitespace_front = std::find_if_not(s.begin(), s.end(), is_whitespace);

    auto aux_end = std::string::reverse_iterator(whitespace_front);
    auto whitespace_back =
    std::find_if_not(s.rbegin(), aux_end, is_whitespace).base();

    s = std::string(whitespace_front, whitespace_back);
}
