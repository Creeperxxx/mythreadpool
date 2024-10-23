#include "threadpool.h"
threadpool::threadpool(int threadnum):taskqueue(std::make_unique<TaskQueue>()),shutdown(false),busythreadnum(0)
{
    this->threadnum.store(threadnum);
    for(int i = 0 ;i<this->threadnum;i++)
    {
        threadvec.push_back(std::make_shared<std::thread>(&threadpool::worker,this));
        threadvec.back()->detach();
    }
}

threadpool::~threadpool()
{
    shutdown = true;
}

void threadpool::worker()
{
    while(!shutdown)
    {
        std::unique_lock<std::mutex> uniquelock(this->threadpoolmutex);
        this->notemptycondvar.wait(uniquelock,[this]{return !this->taskqueue->empty()||shutdown;});
        auto currtask = std::move(this->taskqueue->takeTask());
        uniquelock.unlock();
        ++busythreadnum;
        currtask();
        --busythreadnum;
    }
}

