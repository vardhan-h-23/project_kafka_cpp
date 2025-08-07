// controller.h
// This header will declare the controller class and related functions for
// controlling application logic.
#include "../thread/worker_thread.h"
#include "../utils/message_types.h"
#include "../actor/actor_master.h"
#include <boost/uuid/uuid.hpp>             // uuid class
#include <boost/uuid/uuid_generators.hpp>  // generators
#include <boost/uuid/uuid_io.hpp>          // streaming operators etc
#include <map>
class Controller {
    public:
        void Init();
        void Start();
        void Stop();
        void OnMessageReceived(OrderMessage &msg);
        // void PrintMesage(OrderMessage &msg);
        void CreateAndLaunchOrder(OrderMessage &msg);

    private:
        WorkerThread wrk_thread;
        ActorMaster actor_master;
        std::map<boost::uuids::uuid, std::shared_ptr<Order>> order_map_;
};
