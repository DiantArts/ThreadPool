/*
** EPITECH PROJECT, 2020
** TimedFunction
** File description:
** Control the flow of a function
*/

#include "macros.hpp"
#include "Pool.hpp"

void parallelRun(FunctionList &functionList, std::mutex &mutex, size_t &listSize)
{
    DEBUG_TIME;

    while (mutex.lock(), listSize) {
        functionList.remove_if([&](const auto &element) {
            if ((element.first)()) {
                return false;
            }
            DEBUG_FUNC;
            element.second.get().setToOver();
            listSize--;
            return true;
        });
        mutex.unlock();
    }
    mutex.unlock();
}

ThreadPool::ThreadPool() noexcept
    : m_FunctionListSize(0)
    , m_Thread(nullptr)
{
    DEBUG_FUNC;
}

ThreadPool::~ThreadPool() noexcept
{
    DEBUG_FUNC;

    if (this->m_Thread) {
        std::cout << "DELEEEEEEEEEEEEEEEEEEEETE from destructor" << std::endl;
        m_Mutex.lock();
        m_FunctionListSize = 0;
        while (!m_FunctionList.empty()) {
            m_FunctionList.pop_front();
        }
        m_Mutex.unlock();
        m_Thread->join();
        delete m_Thread;
    }
}

FunctionList::iterator ThreadPool::add(std::function<bool()> func,
        TimedFunction &refTimedFunction) noexcept
{
    DEBUG_FUNC;

    std::lock_guard<std::mutex> lockGuard(this->m_Mutex);

    this->m_FunctionList.push_front(std::make_pair(func, std::reference_wrapper(refTimedFunction)));
    if (!this->m_FunctionListSize++) {
        if (this->m_Thread) {
            m_Thread->join();
            delete this->m_Thread;
        }
        this->m_Thread = new std::thread(parallelRun, std::ref(this->m_FunctionList),
                                         std::ref(this->m_Mutex),
                                         std::ref(this->m_FunctionListSize));
    }
    return this->m_FunctionList.begin();
}

void ThreadPool::
remove(FunctionList::iterator &iterator) noexcept
{
    DEBUG_FUNC;

    std::lock_guard<std::mutex> lockGuard(this->m_Mutex);

    this->m_FunctionList.remove_if([&](const auto &element) { return &element == &*iterator; });
    if (!--this->m_FunctionListSize) {
        this->m_Thread->join();
        delete this->m_Thread;
        this->m_Thread = nullptr;
    }
}

std::size_t ThreadPool::size() const noexcept
{
    // DEBUG_FUNC;

    std::lock_guard<std::mutex> lockGuard(this->m_Mutex);
    return this->m_FunctionListSize;
}
