/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/
#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace rfbase
{
class ThreadPool
{
public:
    explicit ThreadPool();
    explicit ThreadPool(unsigned int numThreads);
    ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    template <typename Func, typename... Args>
    std::future<std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>
    async(Func &&func, Args &&...args)
    {
        using RetType = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>;
        auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        return asyncImpl<RetType>(std::move(task));
    }

    template <typename Func, typename... Args>
    void submit(Func &&func, Args &&...args)
    {
        auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        enqueueTask(std::move(task));
    }

    void waitAll();

private:
    template <typename Result>
    static std::pair<std::function<void()>, std::future<Result>>
    createAsyncTask(std::function<Result()> task)
    {
        std::shared_ptr<std::promise<Result>> promise = std::make_shared<std::promise<Result>>();
        auto future = promise->get_future();

        return std::make_pair([promise = std::move(promise), task = std::move(task)]()
                              { promise->set_value(task()); },
                              std::move(future));
    }

    static std::pair<std::function<void()>, std::future<void>>
    createAsyncTask(std::function<void()> task)
    {
        std::shared_ptr<std::promise<void>> promise = std::make_shared<std::promise<void>>();
        auto future = promise->get_future();
        return std::make_pair(
            [promise = std::move(promise), task = std::move(task)]()
            {
                task();
                promise->set_value();
            },
            std::move(future));
    };

    template <typename Result>
    std::future<Result> asyncImpl(std::function<Result()> task)
    {
        auto [taskWrapper, future] = createAsyncTask(std::move(task));
        enqueueTask(std::move(taskWrapper));
        return std::move(future);
    }

private:
    using Task = std::function<void()>;
    void raise(unsigned int numThreads);
    void taskLoop();
    void stop();
    void enqueueTask(Task task);

private:
    size_t thread_count_ = 0;
    std::vector<std::thread> threads_;

    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::mutex done_mutex_;
    std::condition_variable done_cv_;

    std::queue<Task> tasks_;
    std::atomic<bool> stop_ = false;
    std::atomic<unsigned int> active_threads_ = 0;
};
} // namespace rfbase

#endif
