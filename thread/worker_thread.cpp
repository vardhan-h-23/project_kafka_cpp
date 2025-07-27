
#include "worker_thread.h"
#include <atomic>

void WorkerThread::Init(std::string inst)
{
    instance_working_for_ = inst;
}

bool WorkerThread::Start()
{
    is_running.store(true);
    worker_thread_ = std::thread(&WorkerThread::Run,this);
    return true;
}

bool WorkerThread::Stop()
{
    if (!is_running.exchange(false)) return 0;
    
    PostTask([] {});
    if (worker_thread_.joinable()){
        worker_thread_.join();
    }
    return true;
}

void WorkerThread::PostTask(Task t)
{
    tasks_.push(std::move(t));
}

void WorkerThread::Run()
{
    worker_thred_id_ = std::this_thread::get_id();
    while(is_running.load(std::memory_order_acquire))
    {
        auto exec_= tasks_.pop();
        if (exec_){
            (*exec_)();
        }
        else{
            std::this_thread::yield();
        }
    }
}