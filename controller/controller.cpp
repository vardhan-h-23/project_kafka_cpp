// controller.cpp
// This source file will define the controller class and its methods for
// controlling application logic.
#include <iostream>
#include "controller.h"
#include "../utils/message_types.h"
#include "../order/order.h"
#include <boost/uuid/uuid.hpp>             // uuid class
#include <boost/uuid/uuid_generators.hpp>  // generators
#include <boost/uuid/uuid_io.hpp>          // streaming operators etc
#include <unordered_map>

void Controller::Init() 
{ 
    wrk_thread.Init("Controller"); 
    actor_master.Init();
}

void Controller::Start() { 
    wrk_thread.Start(); 
    actor_master.Start();
}
void Controller::Stop() { 
    actor_master.Stop();
    wrk_thread.Stop(); 
}

void Controller::OnMessageReceived(OrderMessage &msg){
    wrk_thread.PostTask(std::bind(&Controller::CreateAndLaunchOrder,this,msg));
}

void Controller::CreateAndLaunchOrder(OrderMessage &msg) {
    Order ord_(msg);
    boost::uuids::random_generator uuid_gen;
    boost::uuids::uuid my_uuid = uuid_gen();
    std::string uuid_str = boost::uuids::to_string(my_uuid);
    std::cout << "Generated UUID: " << uuid_str << std::endl;
    ord_.omsg_.order_id = std::stoull(uuid_str.substr(0, 16), nullptr, 16);
    actor_master.LaunchOrder(ord_);
}