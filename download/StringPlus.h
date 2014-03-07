#ifndef STRING_PLUS_H
#define STRING_PLUS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::string ReadFileIntoString(const char *filename);
int FindStr2Str(const std::string &srcStr, size_t pos, const std::string &startStr, const std::string &endStr, std::string &outStr);
int FindStr2StrEx(const std::string &srcStr, size_t pos, const std::string &startStr, const std::string &endStr, std::string &outStr);
void DeleteSpaces(std::string &str);
void DeleteSubStr(std::string &srcStr, const std::string &subStr);
void DeleteTag(std::string &tagStr, const std::string &tag, std::string &dataStr);
void DeleteLeadingTrailingSpace(std::string &str);
int SubStrCount(const std::string &src, const std::string &target);
std::string UTF8ToANSI(const std::string& str);

#endif // !STRING_PLUS_H


