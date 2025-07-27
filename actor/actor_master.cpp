#include <iostream>
#include <memory>
#include <functional>
#include <thread>

#include "../utils/logger.h" // Add logger include

#include "actor_master.h"

void ActorMaster::Init()
{
    wrk_thread.Init("Actor_Master"); 
}

bool ActorMaster::Start()
{
    LOG(INFO, "ActorMaster::Start called");
    return wrk_thread.Start(); 
}

bool ActorMaster::Stop()
{
    LOG(INFO, "ActorMaster::Stop called");
    return wrk_thread.Stop(); 
}

bool ActorMaster::LaunchOrder(Order &new_order)
{
    if (!wrk_thread.IsSame(std::this_thread::get_id())){
        wrk_thread.PostTask([this, &new_order]() { this->LaunchOrder(new_order); });
        return true;
    }
    std::shared_ptr<WorkerThread> order_actor = std::make_shared<WorkerThread>();
    order_id_to_order_actor[new_order.omsg_.order_id] = order_actor;
    LOG(INFO, "Created WorkerThread for order_id: " << new_order.omsg_.order_id);
    new_order.Launch(order_actor);
    return true;
}

bool ActorMaster::ScheduleTask(uint64_t order_id, Task &tsk)
{
    LOG(INFO, "ActorMaster::ScheduleTask called for order_id: " << order_id);
    if (!wrk_thread.IsSame(std::this_thread::get_id()))
    {
        LOG(DEBUG, "Not in worker thread, posting ScheduleTask for order_id: " << order_id);
        wrk_thread.PostTask([this, order_id, &tsk]() { this->ScheduleTask(order_id, tsk); });
        return true;
    }
    if (order_id_to_order_actor.find(order_id)!=order_id_to_order_actor.end())
    {
        LOG(INFO, "Posting task to WorkerThread for order_id: " << order_id);
        order_id_to_order_actor[order_id]->PostTask(tsk);
        return true;
    }
    else{
        LOG(ERROR, "No WorkerThread found for order_id: " << order_id << ". Task not scheduled.");
        //some ERROR or crash type thing
        return false;
    }
}