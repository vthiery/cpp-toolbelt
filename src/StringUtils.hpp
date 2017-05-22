#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <list>
#include <string>

namespace Toolbelt
{

namespace StringUtils
{

/*!
 *
 * Merge strings from a container together, using
 * the input delimiter
 *
 */
template <class T>
std::string 
merge(const T& input, const std::string& delimiter) noexcept
{
    std::string output;
    for (auto it{ std::cbegin(input) }; it != std::cend(input); ++it) 
    {
        if (it != std::cbegin(input)) 
        {
            output.append(delimiter);
        }
        output.append(*it);
    }
    return output;
}

/*!
 *
 * Split a string strings using the input delimiter
 * and output the list of sub-strings
 *
 */
std::list<std::string>
split(const std::string& input, const std::string& delimiter) noexcept
{
    size_t from{ 0 };
    size_t to{ 0 };

    std::list<std::string> output;
    while (to != std::string::npos) 
    {
        to = input.find(delimiter, from);
        if (from < input.size() && from != to) 
        {
            output.push_back(input.substr(from, to - from));
        }
        from = to + delimiter.size();
    }
    return output;
}

/*!
 *
 * Check whether a string starts with a given prefix
 *
 */
inline bool 
startsWith(const std::string& input, const std::string& prefix) 
{
    return !(input.compare(0, prefix.size(), prefix));
}

/*!
 *
 * Check whether a string ends with a given suffix
 *
 */
inline bool 
endsWith(const std::string& input, const std::string& suffix) 
{
    const size_t suffixLength{ suffix.length() };
    const size_t inputLength{ input.length() };
    return suffixLength <= inputLength &&
            !input.compare(inputLength - suffixLength, suffixLength, suffix);
}

}

}

#endif
