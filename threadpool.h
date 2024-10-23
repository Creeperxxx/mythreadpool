#include "task.h"
#pragma once
#include <atomic>
#include <vector>
#include <thread>
#include <memory>
#include <condition_variable>

class threadpool
{
private:
    std::atomic<int> threadnum{};
    std::atomic<int> busythreadnum;
    std::condition_variable notemptycondvar;
    std::mutex threadpoolmutex;
    bool shutdown;
    std::unique_ptr<TaskQueue> taskqueue;
    std::vector<std::shared_ptr<std::thread>> threadvec;

public:
    explicit threadpool(int threadnum = 5);
    ~threadpool();

    template <typename F, typename... Args>
    auto commit(F &f, Args &&...args) -> decltype(taskqueue->addTask(f, std::forward<Args>(args)...));
    void worker();
};
template <typename F,typename...Args>
auto threadpool::commit(F& f,Args&&...args)->decltype(taskqueue->addTask(f,std::forward<Args>(args)...))
{
    auto ret = taskqueue->addTask(f,std::forward<Args>(args)...);
    notemptycondvar.notify_one();
    return ret;
}