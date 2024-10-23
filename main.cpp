#include "threadpool.h"
#include "task.h"
#include <iostream>
#include <chrono>
std::mutex mut;
using namespace std;

int f(int i)
{
    auto now = time(nullptr);
    auto datetime = localtime(&now);
    mut.lock();
    std::cout<<"任务编号："<<i<<"  线程id："<<std::this_thread::get_id()<<"  时间："<<datetime->tm_hour<<":"<<datetime->tm_min<<":"<<datetime->tm_sec<<std::endl;
    mut.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return i;
}

int main()
{
    threadpool tpool;
    for(int i = 0 ; i< 100;++i)
    {
        auto ret = tpool.commit(f,i);
    }
    std::this_thread::sleep_for(std::chrono::seconds(20));
    return 0;
}