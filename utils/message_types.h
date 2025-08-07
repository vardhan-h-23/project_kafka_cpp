#pragma once
#include <string>

struct OrderMessage {
    std::uint64_t order_id;
    int reps;
    int time_prd;
    std::string text;
}; 