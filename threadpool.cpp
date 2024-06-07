/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/
#include "threadpool.h"

#include <mutex>

namespace rfbase
{

ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency())
{
}

ThreadPool::ThreadPool(unsigned int numThreads) : thread_count_(numThreads), stop_(false)
{
    raise(numThreads);
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::raise(unsigned int numThreads)
{
    threads_.reserve(numThreads);
    for (unsigned int i = 0; i < numThreads; ++i)
    {
        std::thread th([this] { taskLoop(); });
        threads_.emplace_back(std::move(th));
    }
}

void ThreadPool::taskLoop()
{
    while (true)
    {
        Task task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });

            if (stop_ && tasks_.empty())
            {
                return;
            }
            {
                std::scoped_lock<std::mutex> s_lock(done_mutex_);
                ++thread_count_;
            }
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
        {
            std::unique_lock<std::mutex> lock(done_mutex_);
            --thread_count_;
        }
        done_cv_.notify_all();
    }
}

void ThreadPool::stop()
{
    {
        std::scoped_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    queue_cv_.notify_all();

    for (auto &th : threads_)
    {
        th.join();
    }
}

void ThreadPool::waitAll()
{
    std::unique_lock<std::mutex> lock(done_mutex_);
    done_cv_.wait(lock, [this] { return thread_count_ == 0 && tasks_.empty(); });
}

void ThreadPool::enqueueTask(Task task)
{
    {
        std::scoped_lock<std::mutex> lock(queue_mutex_);
        tasks_.emplace(std::move(task));
    }
    queue_cv_.notify_one();
}
} // namespace rfbase
