
#include "../../includes/utils.hpp"

std::string strTrim(const std::string& str)
{
    size_t begin = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (begin == std::string::npos)
        return ("");
    return (str.substr(begin, end - begin + 1));
}
