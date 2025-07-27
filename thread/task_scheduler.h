#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H
#include <queue>
#include <chrono>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <memory>
#include "../thread/worker_thread.h"

class TaskScheduler
{
    private:
        struct Task
        {
            std::chrono::steady_clock::time_point when;
            std::function<void()> func;
            
            bool operator>(const Task &other) const {
                return when > other.when;
            }
            bool operator<(const Task &other) const {
                return when < other.when;
            }
        };
        std::priority_queue<Task,std::vector<Task>, std::greater<Task>> task_queue;
        std::mutex queue_mutex;
        std::condition_variable cv;
        std::thread worker_thread_;
        std::atomic<bool> running{true};
        std::shared_ptr<WorkerThread> order_actor;
        
        void working_loop()
        {
            while (running.load(std::memory_order_seq_cst))
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                
                if (task_queue.empty()){
                    //
                    cv.wait(lock,[this]{ return !task_queue.empty() || !running; });
                    if (!running) {return;}
                }

                std::chrono::steady_clock::time_point now(std::chrono::steady_clock::now());
                Task tsk = task_queue.top();
                if (tsk.when <= now){
                    //
                    task_queue.pop();

                    try{
                        order_actor->PostTask(tsk.func);
                    }
                    catch (const std::exception& ex){

                    }
                }
                else {
                    cv.wait_for(lock,tsk.when-now);
                }
            }
        }

    public:
        TaskScheduler() : worker_thread_(&TaskScheduler::working_loop, this) {}
        ~TaskScheduler() 
        {
            Stop();
        }

        void Start(std::shared_ptr<WorkerThread> ptr) 
        {
            order_actor = ptr;
        }

        void Stop() 
        {
            if (running.exchange(false)){
                cv.notify_all();
                if (worker_thread_.joinable()) {
                    worker_thread_.join();
                }
            }
        }

        void Schedule_at(std::chrono::steady_clock::time_point when, std::function<void()> func) 
        {
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                task_queue.push(Task{when, func});
            }
            cv.notify_one();
        }

        template<typename Rep, typename Period>
        void Schedule_after(std::chrono::duration<Rep, Period> delay, std::function<void()> func) 
        {
            auto when = std::chrono::steady_clock::now() + delay;
            Schedule_at(when, func);
        }

        void schedule_at_system_time(std::chrono::system_clock::time_point when, std::function<void()> func) 
        {
            std::chrono::steady_clock::time_point now_steady = std::chrono::steady_clock::now();
            std::chrono::system_clock::time_point now_system = std::chrono::system_clock::now();
            auto when_steady = now_steady + (when - now_system);
            Schedule_at(when_steady, func);
        }
};
#endif // TASK_SCHEDULER_H