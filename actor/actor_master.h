#ifndef ACTOR_MASTER_H
#define ACTOR_MASTER_H
#include <functional>
#include "../order/order.h"
#include "../thread/worker_thread.h"
#include <map>

class ActorMaster {
    using  Task = std::function<void()>;
    public:
        void Init();
        bool Start();
        bool Stop();
        bool LaunchOrder(std::shared_ptr<Order> new_order);
        bool ScheduleTask(uint64_t order_id, Task& tsk);
    private:
        WorkerThread wrk_thread;
        // order_id mapped to the Order actors
        std::map<uint64_t, std::shared_ptr<WorkerThread>> order_id_to_order_actor;
};
#endif // ACTOR_MASTER_H