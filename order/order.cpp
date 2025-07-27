#include "order.h"
#include "../thread/worker_thread.h"
#include "../thread/task_scheduler.h"
#include "../utils/message_types.h"
#include "../utils/logger.h"
#include <iomanip>
Order::Order(OrderMessage &omsg){
    omsg_ = omsg;
}

bool Order::Launch(const std::shared_ptr<WorkerThread> &actor)
{
    order_actor = actor;
    task_scheduler.Start(order_actor);
    order_actor->Start();
    order_actor->PostTask(std::bind(&Order::Start, this));
    LOG(INFO, "Order launched with order_id: " << omsg_.order_id);
    return true;
}

bool Order::Start()
{
    // Logic to start the order processing
    if (omsg_.reps<=0) {
        LOG(INFO, "Invalid reps in order_id=" << omsg_.order_id);
        return true;
    }
    order_actor->PostTask([this]() {
        Print();
    });
    return true;
}

void Order::Print()
{
    Plog(omsg_.text);
    omsg_.reps--;
    if (omsg_.reps>0)
    {
        task_scheduler.Schedule_after(std::chrono::duration<int64_t, std::micro> (omsg_.time_prd*1000),[=] {Order::Print();});
    }
    
}

void Order::Plog(const std::string& msg) {
    auto now = std::chrono::system_clock::now();
    auto tid = std::this_thread::get_id();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::cout << "[" << std::put_time(std::localtime(&t), "%T") << "][TID:" << tid << "] " << msg << "\n";
}


bool Order::Cancel(){
    // Logic to cancel the order
    return true; // Assuming cancellation is successful
}