/*
** EPITECH PROJECT, 2020
** TimedFunction
** File description:
** Control the flow of a function over time
*/

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <functional>
#include <future>
#include <atomic>
#include <forward_list>
#include "TimedFunction.hpp"

using FunctionList = std::forward_list<std::pair<std::function<bool ()>,
                                                 std::reference_wrapper<TimedFunction>>>;

void parallelRun(FunctionList &functionList, std::mutex &mutex, size_t &listSize);

class ThreadPool {
public:
    explicit ThreadPool() noexcept;
    ~ThreadPool() noexcept;

    FunctionList::iterator add(std::function<bool()> func,
                               TimedFunction &refTimedFunction) noexcept;
    void remove(FunctionList::iterator &it) noexcept;
    std::size_t size() const noexcept;

private:
    FunctionList m_FunctionList;
    size_t m_FunctionListSize;

    std::atomic<bool> m_ThreadIsSupposedToRun;
    std::condition_variable cv;
    mutable std::mutex m_Mutex;
    std::thread *m_Thread;
};

#endif // THREAD_POOL_HPP
