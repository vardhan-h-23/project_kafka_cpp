// consumer.h
// This header will declare the Kafka consumer class and related functions.

#include <string>
#include <atomic>
#include <thread>
#include <functional>
#include <memory>
#include "../utils/message_types.h"

namespace cppkafka {
    class Consumer;
}

class Consumer {
   public:
    Consumer(const std::string& brokers);
    ~Consumer();
    void Init(const std::string& topic);
    bool Start(std::function<void(OrderMessage&)> on_message);
    bool Stop();

   private:
    std::string brokers_;
    std::string topic_;
    std::unique_ptr<cppkafka::Consumer> consumer_;
    std::atomic<bool> running_;
    std::thread consumer_thread_;
    std::function<void(OrderMessage&)> on_message_;
};