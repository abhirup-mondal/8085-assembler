#ifndef _GUARD_STRING_CLEANER_HPP
#define _GUARD_STRING_CLEANER_HPP

#include <string>
#include <vector>

std::vector<std::string> split(std::string s,char c);

void whitespace_clean(std::string& s);
void replace_tab_by_space(std::string& s);
void whitespace_compress(std::string& s);
void whitespace_trim(std::string &s);

#endif
