#ifndef PROGRAM_OPTIONS_HPP
#define PROGRAM_OPTIONS_HPP

#include <algorithm>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Toolbelt
{
/*!
 *
 * Arguments class
 *
 * Parse the options given as arguments and offer getters
 *
 */
class Arguments
{
    const char dash = '-';
    const char equal = '=';

    using arg_map = std::unordered_map<std::string_view, std::optional<std::string_view>>;

public:

    /*!
     *
     * Constructor accepting the usual arguments
     *
     */
    inline Arguments(const int argc, char** argv) noexcept;

    /*!
     *
     * Get an optional from an option name
     *
     */
    template<class T> inline std::optional<T> get(const std::string_view& option) const noexcept;

    /*!
     *
     * Get the value from an option name, with a default
     *
     */
    template<class T> inline T get(const std::string_view& option, T&& defaultValue) const noexcept;

private:

    /*!
     *
     * Handle an option
     *
     * Either pass to handleOption or to handleValue
     *
     */
    inline void handle(const std::string_view& option) noexcept;

    /*!
     *
     * Handle an option
     *
     */
    inline void handleOption(const std::string_view& option) noexcept;

    /*!
     *
     * Handle an option's value
     *
     */
    inline void handleValue(const std::optional<std::string_view>& value) noexcept;

    /*!
     *
     * Internal getter from the map
     *
     */
    template<class T> inline std::optional<T> get(const arg_map& options, const std::string_view& option) const noexcept;

    /*!
     *
     * Get an option's value from the map
     *
     */
    inline std::optional<std::string_view> getValue(const arg_map& options, const std::string_view& option) const noexcept;

    //! The current option
    std::optional<std::string_view> m_currentOption;

    //! The options' map
    arg_map m_options;
};

Arguments::
Arguments(const int argc, char** argv) noexcept
{
    for (int i{ 1 }; i < argc; ++i)
    {
        handle(argv[i]);
    }
}

template<class T>
std::optional<T>
Arguments::
get(const std::string_view& option) const noexcept
{
    return get<T>(m_options, option);
}

template<class T>
T
Arguments::
get(const std::string_view& option, T&& defaultValue) const noexcept
{
    return get<T>(option).value_or(defaultValue);
}

void
Arguments::
handle(const std::string_view& obj) noexcept
{
    // if starts with '-', treat as option; otherwise, it is a value
    obj.at(0) == dash ? handleOption(obj) : handleValue(obj);
}

void
Arguments::
handleOption(const std::string_view& option) noexcept
{
    if (m_currentOption)
    {
        handleValue(std::nullopt);
    }
    m_currentOption = option;
    m_currentOption->remove_prefix(m_currentOption->find_first_not_of(dash));

    if (const auto it{ m_currentOption->find_first_of(equal)}; it != std::string_view::npos)
    {
        auto value = m_currentOption.value();
        value.remove_prefix(it + 1);
        // Update the current option
        m_currentOption->remove_suffix(m_currentOption->size() - it);
        // Handle its value
        handleValue(value);
    }
}

void
Arguments::
handleValue(const std::optional<std::string_view>& value) noexcept
{
    m_options.emplace(m_currentOption.value(), value);
    m_currentOption.reset();
}

template<class T>
std::optional<T>
Arguments::
get(const arg_map& options, const std::string_view& option) const noexcept
{
    if (const auto v{ getValue(options, option) })
    {
        if (T value; std::istringstream(std::string(*v)) >> value)
        {
            return value;
        }
    }
    return std::nullopt;
}

std::optional<std::string_view>
Arguments::
getValue(const arg_map& options, const std::string_view& option) const noexcept
{
    const auto it{ options.find(option) };
    return it != std::cend(options) ? std::make_optional(*it->second) : std::nullopt;
}

}

#endif
