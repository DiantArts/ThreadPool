/*
** EPITECH PROJECT, 2020
** TimedFunction
** File description:
** Control the flow of a function over time
*/

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <chrono>
#include <optional>
#include <functional>
#include <future>
#include <atomic>
#include <list>
// #include "TimedFunction.hpp"

class TimedFunction;

using FunctionList = std::list<std::pair<
        std::function<std::optional<std::chrono::duration<float>> ()>,
        std::reference_wrapper<TimedFunction>
    >>;

class ThreadPool {
public:
    explicit ThreadPool() noexcept;
    ~ThreadPool();

    FunctionList::iterator add(std::function<std::optional<std::chrono::duration<float>> ()> func,
                               TimedFunction &refTimedFunction);
    void remove(FunctionList::iterator &it);

    std::size_t size() const;
    bool empty() const;

    void join();
    static void parallelRun(ThreadPool &threadPool);

public:
    static uint_least16_t nextId;
    const uint_least16_t id;

private:
    FunctionList m_FunctionList;

    bool m_IsSupposedToRun;

    mutable std::condition_variable m_Cv;
    mutable std::mutex m_Mutex;

    std::thread m_Thread;
};

#endif // THREAD_POOL_HPP
