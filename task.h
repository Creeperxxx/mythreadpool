#pragma once
#include <queue>
#include <functional>
#include <mutex>
#include <future>
#include <iostream>
#include <utility>

class TaskQueue
{
public:
    using Task = std::function<void()>; 
    template <typename F, typename... Args>
    auto addTask(F &f, Args &&...args) -> std::future<decltype(f(args...))>; // 添加任务
    Task takeTask();                                                         // 取任务
    bool empty() { return taskQueue.empty(); }

private:
    std::mutex taskQueueMutex;  // 任务队列互斥锁
    std::queue<Task> taskQueue; // 任务队列
};

template <typename F, typename... Args> // 可变参数模板，模板必须在头文件定义
auto TaskQueue::addTask(F &f, Args &&...args) -> std::future<decltype(f(args...))>
{
    using RetType = decltype(f(args...));
    auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(f, std::forward<Args>(args)...));
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex);
    taskQueue.emplace([task]{ (*task)(); });
    return task->get_future();
}

