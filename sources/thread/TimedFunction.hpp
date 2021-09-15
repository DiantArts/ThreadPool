/*
** EPITECH PROJECT, 2020
** TimedFunction
** File description:
** Control the flow of a function over time
*/

#ifndef TIMED_FUNCTION_HPP
#define TIMED_FUNCTION_HPP

#include <list>
#include <functional>
#include <optional>
#include <chrono>
#include <atomic>
#include <deque>
#include "Pool.hpp"

struct arg_t {
    uint_fast32_t numberOfTime;
    std::chrono::steady_clock::time_point *intervalClockStart;
    std::chrono::steady_clock::time_point *durationClockStart;
};

class ThreadPool;

class TimedFunction {
public:
    explicit TimedFunction(ThreadPool &threadPool,
                           const std::function<void ()> &fn,
                           const std::optional<int> &howManyTimes = std::nullopt) noexcept;
    explicit TimedFunction(ThreadPool &threadPool,
                           const std::function<void ()> &fn,
                           const std::chrono::duration<float> &interval,
                           const std::optional<int> &howManyTimes = std::nullopt) noexcept;
    explicit TimedFunction(ThreadPool &threadPool,
                           const std::function<void ()> &fn,
                           const std::chrono::duration<float> &interval,
                           const std::chrono::duration<float> &duration,
                           const std::optional<int> &howManyTimes = std::nullopt) noexcept;
    ~TimedFunction() noexcept;

    bool isOver() const noexcept;
    void join() const noexcept;
    void terminate() noexcept;

private:
    friend void ThreadPool::parallelRun(ThreadPool &threadPool);
    bool m_IsOver;
    arg_t m_Args;

    std::list<std::pair<std::function<std::optional<std::chrono::duration<float>> (void)>,
                        std::reference_wrapper<TimedFunction>>>::iterator m_Iterator;
};

#endif // TIMED_FUNCTION_HPP
