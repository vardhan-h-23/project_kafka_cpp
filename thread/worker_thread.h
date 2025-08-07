
#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H
#include <atomic>
#include <string>
#include <thread>
#include <iostream>
#include "MPMClist.hpp"

class WorkerThread {
    public:
    std::thread::id worker_thread_id_;
    WorkerThread(): worker_thread_(&WorkerThread::Run, this) {
      worker_thread_id_ = worker_thread_.get_id();
    }
    WorkerThread(std::string str): worker_thread_(&WorkerThread::Run, this) {}

     using Task = std::function<void()>;
     // Init 
     void Init(std::string inst);
     // Start will have
     // thread up and running to do all the taks in the queue
   //   bool Start();
     // mechanism to Stop the start/running function
     bool Stop();
     // get the tasks
     void PostTask(Task t);
     
     bool IsSame(std::thread::id thread_id){
        return thread_id == worker_thread_id_;
     }

    private:
    void Run();
     MPMClist tasks_;
     
     std::string instance_working_for_;
     std::thread worker_thread_;
     std::atomic<bool> is_running{true};
};
#endif // WORKER_THREAD_H