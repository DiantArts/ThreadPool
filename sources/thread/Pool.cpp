/*
** EPITECH PROJECT, 2020
** TimedFunction
** File description:
** Control the flow of a function
*/

#include "TimedFunction.hpp"
#include "macros.hpp"
#include "Pool.hpp"

uint_least16_t ThreadPool::nextId = 0;

void ThreadPool::parallelRun(ThreadPool &threadPool)
{
    auto safeIsSupposedToRun = true;
    auto timeToWaitForNextCall = std::chrono::duration<float>(0);

    while (true) {
        {
            std::unique_lock<std::mutex> uniqueLock(threadPool.m_Mutex);
            threadPool.m_Cv.wait_for(uniqueLock, timeToWaitForNextCall,
            [&threadPool, &safeIsSupposedToRun] {
                if (!threadPool.m_IsSupposedToRun) {
                    safeIsSupposedToRun = threadPool.m_IsSupposedToRun;
                    return true;
                }
                return false;
            });
            if (!safeIsSupposedToRun) {
                break;
            }
        }

        {
            std::unique_lock<std::mutex> uniqueLock(threadPool.m_Mutex);
            threadPool.m_Cv.wait(uniqueLock, [&threadPool, &safeIsSupposedToRun] {
                if (!threadPool.m_FunctionList.empty()) {
                    return true;
                } else if (!threadPool.m_IsSupposedToRun) {
                    safeIsSupposedToRun = threadPool.m_IsSupposedToRun;
                    return true;
                }
                return false;
            });
            if (!safeIsSupposedToRun) {
                break;
            }
        }

        timeToWaitForNextCall = std::chrono::duration<float>(0);
        threadPool.m_Mutex.lock();
        threadPool.m_FunctionList.remove_if([&](const auto &element) {
            auto returnValue = (element.first)();
            if (returnValue == std::nullopt) {
                threadPool.m_Cv.notify_all();
                return true;
            }
            if (timeToWaitForNextCall < returnValue.value()) {
                timeToWaitForNextCall = returnValue.value();
            }
            return false;
        });
        threadPool.m_Mutex.unlock();
    }
    DEBUG_MSG("exit thread");
}

ThreadPool::ThreadPool() noexcept
    : id(nextId++), m_IsSupposedToRun(true)
    , m_Thread(ThreadPool::parallelRun, std::ref(*this))
{}

ThreadPool::~ThreadPool()
{
    this->m_Mutex.lock();
    this->m_IsSupposedToRun = false;
    while (!m_FunctionList.empty()) {
        this->m_FunctionList.pop_front();
    }
    this->m_Cv.notify_all();
    this->m_Mutex.unlock();
    this->m_Thread.join();
}

FunctionList::iterator ThreadPool::add(std::function<std::optional<std::chrono::duration<float>> ()> func,
        TimedFunction &timedFunction)
{
    this->m_Mutex.lock();
    this->m_FunctionList.push_front(std::make_pair(func, std::reference_wrapper(timedFunction)));
    auto it = this->m_FunctionList.begin();
    this->m_Mutex.unlock();
    this->m_Cv.notify_all();
    return it;
}

void ThreadPool::remove(FunctionList::iterator &iterator)
{
    this->m_Mutex.lock();
    this->m_FunctionList.erase(iterator);
    this->m_Mutex.unlock();
    this->m_Cv.notify_all();
}

std::size_t ThreadPool::size() const
{
    std::lock_guard<std::mutex> lockGuard(this->m_Mutex);
    return this->m_FunctionList.size();
}

bool ThreadPool::empty() const
{
    std::lock_guard<std::mutex> lockGuard(this->m_Mutex);
    return this->m_FunctionList.empty();
}

void ThreadPool::join()
{
    std::unique_lock<std::mutex> uniqueLock(this->m_Mutex);
    this->m_Cv.wait(uniqueLock, [&] { return this->m_FunctionList.empty(); });
}
