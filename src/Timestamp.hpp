#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#include <chrono>
#include <string>
#include <iomanip>

namespace Toolbelt
{
/*!
 *
 * Timestamp class
 *
 * This is a static class wrapping up functionalities to
 * get a timestamp and convert timestamps to ISO-8601
 * dates.
 *
 */
class Timestamp final
{
public:

    /*!
    *
    * Return the timestamp
    *
    */
    static inline time_t now() noexcept
    {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

    /*!
    *
    * Convert the timestamp to a string
    *
    * The format is ISO-8601 compliant: YYYY-MM-DDTHH-MM-SSZ
    *
    */
    static inline std::string toString(const time_t timestamp) noexcept
    {
        std::stringstream output;
        output << std::put_time(std::localtime(&timestamp), "%FT%TZ");
        return output.str();
    }
};

}

#endif
