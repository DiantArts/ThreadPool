/*
** EPITECH PROJECT, 2020
** TimedFunction
** File description:
** Control the flow of a function
*/

#include <iostream>
#include "macros.hpp"
#include "Pool.hpp"
#include "TimedFunction.hpp"

static std::optional<std::chrono::duration<float>> midFunc1(const std::function<void()> &func);
static std::optional<std::chrono::duration<float>> midFunc2(const std::function<void()> &func, const std::chrono::duration<float> maxDuration, arg_t &args);
static std::optional<std::chrono::duration<float>> midFunc3(const std::function<void()> &func, uint_fast32_t maxTime, arg_t &args);
static std::optional<std::chrono::duration<float>> midFunc4(const std::function<void()> &func, const std::chrono::duration<float> maxDuration, uint_fast32_t maxTime, arg_t &args);
static std::optional<std::chrono::duration<float>> midFunc5(const std::function<void()> &func, const std::chrono::duration<float> interval, arg_t &args);
static std::optional<std::chrono::duration<float>> midFunc6(const std::function<void()> &func, const std::chrono::duration<float> interval, const std::chrono::duration<float> maxDuration, arg_t &args);
static std::optional<std::chrono::duration<float>> midFunc7(const std::function<void()> &func, const std::chrono::duration<float> interval, uint_fast32_t maxTime, arg_t &args);
static std::optional<std::chrono::duration<float>> midFunc8(const std::function<void()> &func, const std::chrono::duration<float> interval, const std::chrono::duration<float> maxDuration, uint_fast32_t maxTime, arg_t &args);

TimedFunction::TimedFunction(ThreadPool &threadPool,
                             const std::function<void()> &func,
                             const std::optional<int> &howManyTimes /* = std::nullopt */) noexcept
    : m_IsOver(false), m_Args({ 0, nullptr, nullptr })
{
    // bind the right function with right arguments
    std::function<std::optional<std::chrono::duration<float>> (void)> funcToSend;

    funcToSend = (!howManyTimes)
        ? static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc1, func))
        : static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc3, func, howManyTimes.value(), this->m_Args));

    this->m_Iterator = threadPool.add(funcToSend, *this);
}

TimedFunction::TimedFunction(ThreadPool &threadPool,
                             const std::function<void()> &func,
                             const std::chrono::duration<float> &duration,
                             const std::optional<int> &howManyTimes /* = std::nullopt */) noexcept
    : m_IsOver(false), m_Args({ 0, nullptr, nullptr })
{
    // bind the right function with right arguments
    std::function<std::optional<std::chrono::duration<float>> (void)> funcToSend;
    if (!duration.count()) {
        funcToSend = (!howManyTimes)
            ? static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc1, func))
            : static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc3, func, howManyTimes.value(), this->m_Args));
    } else {
        this->m_Args.durationClockStart = new std::chrono::steady_clock::time_point;
        *this->m_Args.durationClockStart = std::chrono::steady_clock::now();
        funcToSend = (!howManyTimes)
            ? static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc2, func, duration, this->m_Args))
            : static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc4, func, duration, howManyTimes.value(), this->m_Args));
    }

    this->m_Iterator = threadPool.add(funcToSend, *this);
}

TimedFunction::TimedFunction(ThreadPool &threadPool,
                             const std::function<void()> &func,
                             const std::chrono::duration<float> &interval,
                             const std::chrono::duration<float> &duration,
                             const std::optional<int> &howManyTimes /* = std::nullopt */) noexcept
    : m_IsOver(false), m_Args({ 0, nullptr, nullptr })
{
    // bind the right function with right arguments
    std::function<std::optional<std::chrono::duration<float>> (void)> funcToSend;
    if (!interval.count()) {
        if (duration.count()) {
            this->m_Args.durationClockStart = new std::chrono::steady_clock::time_point;
            *this->m_Args.durationClockStart = std::chrono::steady_clock::now();
            funcToSend = (!howManyTimes)
                ? static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc2, func, duration, this->m_Args))
                : static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc4, func, duration, howManyTimes.value(), this->m_Args));
        } else {
            funcToSend = (!howManyTimes)
                ? static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc1, func))
                : static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc3, func, howManyTimes.value(), this->m_Args));
        }
    } else if (duration.count()) {
        this->m_Args.durationClockStart = new std::chrono::steady_clock::time_point;
        *this->m_Args.durationClockStart = std::chrono::steady_clock::now();
        this->m_Args.intervalClockStart = new std::chrono::steady_clock::time_point;
        *this->m_Args.intervalClockStart = std::chrono::steady_clock::now();
        funcToSend = (!howManyTimes)
            ? static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc6, func, interval, duration, this->m_Args))
            : static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc8, func, interval, duration, howManyTimes.value(), this->m_Args));
    } else {
        this->m_Args.intervalClockStart = new std::chrono::steady_clock::time_point;
        *this->m_Args.intervalClockStart = std::chrono::steady_clock::now();
        funcToSend = (!howManyTimes)
            ? static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc5, func, interval, this->m_Args))
            : static_cast<std::function<std::optional<std::chrono::duration<float>> (void)>>(std::bind(midFunc7, func, interval, howManyTimes.value(), this->m_Args));
    }

    this->m_Iterator = threadPool.add(funcToSend, *this);
}

TimedFunction::~TimedFunction() noexcept
{
    if (m_Args.intervalClockStart) {
        delete m_Args.intervalClockStart;
    }
    if (m_Args.durationClockStart) {
        delete m_Args.durationClockStart;
    }
}

bool TimedFunction::isOver() const noexcept
{
    return m_IsOver;
}

// interval == 0
// duration == infinit
// howManyTimes == infinit
static std::optional<std::chrono::duration<float>> midFunc1(const std::function<void()> &func)
{
    func();
    return std::chrono::duration<float>(0);
}

// interval == 0
// duration != infinit
// howManyTimes == infinit
static std::optional<std::chrono::duration<float>> midFunc2(const std::function<void()> &func, const std::chrono::duration<float> maxDuration, arg_t &args)
{
    if (std::chrono::steady_clock::now() - *args.durationClockStart >= maxDuration) {
        return std::nullopt;
    }
    func();
    return std::chrono::duration<float>(0);
}

// interval == 0
// duration == infinit
// howManyTimes != infinit
static std::optional<std::chrono::duration<float>> midFunc3(const std::function<void()> &func, uint_fast32_t maxTime, arg_t &args)
{
    if (args.numberOfTime >= maxTime) {
        return std::nullopt;
    }
    func();
    args.numberOfTime++;
    return std::chrono::duration<float>(0);
}

// interval == 0
// duration != infinit
// howManyTimes != infinit
static std::optional<std::chrono::duration<float>> midFunc4(const std::function<void()> &func, const std::chrono::duration<float> maxDuration, uint_fast32_t maxTime, arg_t &args)
{
    if ((args.numberOfTime >= maxTime) || (std::chrono::steady_clock::now() - *args.durationClockStart >= maxDuration)) {
        return std::nullopt;
    }
    args.numberOfTime++;
    func();
    return std::chrono::duration<float>(0);
}

// interval != 0
// duration == infinit
// howManyTimes == infinit
static std::optional<std::chrono::duration<float>> midFunc5(const std::function<void()> &func, const std::chrono::duration<float> interval, arg_t &args)
{
    if (std::chrono::steady_clock::now() - *args.intervalClockStart < interval) {
        return std::chrono::steady_clock::now() - *args.intervalClockStart;
    }
    *args.durationClockStart = std::chrono::steady_clock::now();
    func();
    return interval;
}

// interval != 0
// duration != infinit
// howManyTimes == infinit
static std::optional<std::chrono::duration<float>> midFunc6(const std::function<void()> &func, const std::chrono::duration<float> interval, const std::chrono::duration<float> maxDuration, arg_t &args)
{
    if (std::chrono::steady_clock::now() - *args.intervalClockStart < interval) {
        return std::chrono::steady_clock::now() - *args.intervalClockStart;
    } else if (std::chrono::steady_clock::now() - *args.durationClockStart >= maxDuration) {
        return std::nullopt;
    }
    *args.intervalClockStart = std::chrono::steady_clock::now();
    func();
    return interval;
}

// interval != 0
// duration == infinit
// howManyTimes != infinit
static std::optional<std::chrono::duration<float>> midFunc7(const std::function<void()> &func, const std::chrono::duration<float> interval, uint_fast32_t maxTime, arg_t &args)
{
    if (std::chrono::steady_clock::now() - *args.intervalClockStart < interval) {
        return std::chrono::steady_clock::now() - *args.intervalClockStart;
    } if (args.numberOfTime >= maxTime) {
        return std::nullopt;
    }
    *args.intervalClockStart = std::chrono::steady_clock::now();
    args.numberOfTime++;
    func();
    return interval;
}

// interval != 0
// duration != infinit
// howManyTimes != infinit
static std::optional<std::chrono::duration<float>> midFunc8(const std::function<void()> &func, const std::chrono::duration<float> interval, const std::chrono::duration<float> maxDuration, uint_fast32_t maxTime, arg_t &args)
{
    if (std::chrono::steady_clock::now() - *args.intervalClockStart < interval) {
        return std::chrono::steady_clock::now() - *args.intervalClockStart;
    } else if ((args.numberOfTime >= maxTime) || (std::chrono::steady_clock::now() - *args.durationClockStart >= maxDuration)) {
        return std::nullopt;
    }
    *args.intervalClockStart = std::chrono::steady_clock::now();
    args.numberOfTime++;
    func();
    return interval;
}
