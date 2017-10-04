#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <string>
#include <unordered_map>

namespace Toolbelt
{
/*!
 *
 * Timer class
 *
 * This class is templatized and expects a ratio.
 * It allows for the classic start/stop/duration
 * and adds pausing and recording features.
 *
 */
template <class Precision>
class Timer final
{

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

public:

    /*!
     *
     * Start the timer
     *
     */
    inline void start() noexcept;

    /*!
     *
     * Pause the timer
     *
     * Does not stop the timer, just store the accumulated time
     * between the last start/resume and now.
     * Shall NOT be used in place of stop().
     *
     */
    inline void pause() noexcept;

    /*!
     *
     * Resume the timer after being paused
     *
     * Must be called only after pause().
     * Shall NOT be used in place of start().
     *
     */
    inline void resume() noexcept;

    /*!
     *
     * Stop the timer
     *
     * Must be called before duration().
     * Must NOT be called before lap().
     *
     */
    inline void stop() noexcept;

    /*!
     *
     * Returns the duration
     *
     * Will call stop().
     * It will return the time accumulated between start and end,
     * taking into account the pauses.
     *
     */
    inline double duration() noexcept;

    /*!
     *
     * Records a duration
     *
     * Will call stop().
     * It will store the time accumulated between start and end,
     * taking into account the pauses.
     *
     */
    inline void lap(const std::string& key) noexcept;

    /*!
     *
     * Returns the timings recorded via lap()
     *
     */
    inline std::unordered_map<std::string, double> timings() const noexcept;

    /*!
     *
     * Reset the timer
     *
     * It will erase the records, reset the information about
     * the pauses and stop the timer
     *
     */
    inline void reset() noexcept;

private:

    //! Finite states
    enum class State
    {
        Running,
        Paused,
        Stopped
    };

    //! The state
    State m_state{ State::Stopped };

    //! Alias
    inline TimePoint now() const noexcept{ return std::chrono::high_resolution_clock::now(); }

    //! The begin time
    TimePoint m_begin;

    //! The end time
    TimePoint m_end;

    //! The time accumulated, necessary to support pauses
    double m_accumulatedDuration;

    //! The timings recorded via record()
    std::unordered_map<std::string, double> m_timings;
};

template <class Precision>
void
Timer<Precision>::
start() noexcept
{
    m_state = State::Running;
    m_accumulatedDuration = 0.0;
    m_begin = now();
}

template <class Precision>
void
Timer<Precision>::
pause() noexcept
{
    if (m_state == State::Running)
    {
        m_state = State::Paused;
        m_accumulatedDuration += std::chrono::duration<double, Precision>(now() - m_begin).count();
    }
}

template <class Precision>
void
Timer<Precision>::
resume() noexcept
{
    if (m_state == State::Paused)
    {
        m_state = State::Running;
        m_begin = now();
    }
}

template <class Precision>
void
Timer<Precision>::
stop() noexcept
{
    if (m_state != State::Stopped)
    {
        m_end = m_state == State::Paused ? m_begin : now();
        m_state = State::Stopped;
    }
}

template <class Precision>
double
Timer<Precision>::
duration() noexcept
{
    stop();
    return m_accumulatedDuration + std::chrono::duration<double, Precision>(m_end - m_begin).count();
}

template <class Precision>
void
Timer<Precision>::
lap(const std::string& key) noexcept
{
    stop();
    m_timings[key] = duration();
    start();
}

template <class Precision>
std::unordered_map<std::string, double>
Timer<Precision>::
timings() const noexcept
{
    return m_timings;
}

template <class Precision>
void
Timer<Precision>::
reset() noexcept
{
    stop();
    m_accumulatedDuration = 0.0;
    m_timings.clear();
}

}

#endif
