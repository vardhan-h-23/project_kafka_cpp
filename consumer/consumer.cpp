// consumer.cpp
// This source file defines the Kafka consumer class and its methods.

#include "consumer.h"
#include <cppkafka/cppkafka.h>
#include "../utils/json.hpp"
#include "../utils/logger.h"
#include <iostream>

Consumer::Consumer(const std::string& brokers)
    : brokers_(brokers), running_(false) {}

Consumer::~Consumer() {
    Stop();
}

void Consumer::Init(const std::string& topic) {
    topic_ = topic;
    cppkafka::Configuration config = {
        {"metadata.broker.list", brokers_},
        {"group.id", "my_group"},
        {"auto.offset.reset", "earliest"}
    };
    consumer_ = std::make_unique<cppkafka::Consumer>(config);
    consumer_->subscribe({topic_});
}

bool Consumer::Start(std::function<void(OrderMessage&)> on_message) {
    if (running_) return false;
    if (!consumer_) return false;
    running_ = true;
    on_message_ = on_message;
    consumer_thread_ = std::thread([this]() {
        while (running_) {
            auto msg = consumer_->poll();
            if (msg) {
                if (msg.get_error()) {
                    if (!msg.is_eof()) {
                        LOG(ERROR, "Kafka error: " << msg.get_error());
                    }
                } else {
                    try {
                        auto j = nlohmann::json::parse(msg.get_payload());
                        OrderMessage pom;
                        pom.reps = j.at("reps").get<int>();
                        pom.time_prd = j.at("time_prd").get<int>();
                        pom.text = j.at("text").get<std::string>();
                        if (on_message_) on_message_(pom);
                    } catch (const std::exception& e) {
                        LOG(ERROR, "JSON parse error or missing field: " << e.what());
                    }
                }
            }
        }
    });
    return true;
}

bool Consumer::Stop() {
    if (!running_) return false;
    running_ = false;
    if (consumer_thread_.joinable()) consumer_thread_.join();
    if (consumer_) consumer_->unsubscribe();
    return true;
}