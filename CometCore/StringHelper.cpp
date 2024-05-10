#include "StringHelper.hpp"

std::string& ltrim(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    return str;
}

std::string& rtrim(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), str.end());
    return str;
}

std::string& trim(std::string& str)
{
    return ltrim(rtrim(str));
}
