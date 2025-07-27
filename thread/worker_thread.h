#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H
#include <atomic>
#include <string>
#include <thread>
#include "MPMClist.hpp"

class WorkerThread {
    public:
    WorkerThread(): is_running(false){}
    
     using Task = std::function<void()>;
     // Init 
     void Init(std::string inst);
     // Start will have
     // thread up and running to do all the taks in the queue
     bool Start();
     // mechanism to Stop the start/running function
     bool Stop();
     // get the tasks
     void PostTask(Task t);
     
     bool IsSame(std::thread::id thread_id){
        return thread_id == worker_thred_id_;
     }

    private:
    void Run();
     MPMClist tasks_;
     std::thread::id worker_thred_id_;
     std::string instance_working_for_;
     std::thread worker_thread_;
     std::atomic<bool> is_running;
};
#endif // WORKER_THREAD_H