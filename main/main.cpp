// main.cpp
// This is the entry point of the application. It will create instances of the
// consumer, controller, and other components.
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include "../utils/message_types.h"
#include "../consumer/consumer.h"
#include "../controller/controller.h"
#include <chrono>

using namespace std;

std::atomic<bool> running(true);

void signal_handler(int signum) {
    running = false;
}

int main(){
    Consumer consumer_("localhost:9092");
    Controller controller_;
    controller_.Init();
    controller_.Start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::signal(SIGINT, signal_handler);
    consumer_.Init("new_topic_1");
    consumer_.Start(std::bind(&Controller::OnMessageReceived,&controller_,std::placeholders::_1));
    // Wait until Ctrl+C
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    consumer_.Stop();
    controller_.Stop();
    std::cout << "Exiting cleanly." << std::endl;
    return 0;
}