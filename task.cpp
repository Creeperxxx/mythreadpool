/** Created by Jiale on 2022/3/14 10:19.
 *  Decryption: 任务队列源文件
**/

#include "task.h"

/**
 * 从任务队列中取任务
 * @return 取出的任务
 */
TaskQueue::Task TaskQueue::takeTask() {
    Task task;
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex);  // 上锁
    if (!taskQueue.empty()) {
        task = std::move(taskQueue.front());    // 取出任务
        taskQueue.pop();  // 将任务从队列中删除
        return task;
    }
    return nullptr;
}