#ifndef ORDER_H
#define ORDER_H
#include "../utils/message_types.h"
#include "../thread/task_scheduler.h"
class Order {
    public:
        Order(OrderMessage &omsg);
        bool Launch(const std::shared_ptr<WorkerThread> &actor);
        bool Start();
        bool Cancel();
        OrderMessage omsg_;
    private:
        std::shared_ptr<WorkerThread> order_actor;
        TaskScheduler task_scheduler; // Assuming TaskScheduler is defined elsewhere
        void Print();
        void Plog(const std::string &msg);
};
#endif // ORDER_H