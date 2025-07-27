// controller.h
// This header will declare the controller class and related functions for
// controlling application logic.
#include "../thread/worker_thread.h"
#include "../utils/message_types.h"
#include "../actor/actor_master.h"
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
};
