#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>

#include "nlohmann/json.hpp"
using json = nlohmann::json;


enum class TransportType {
    kPlane,
    kTrain,
    kSuburban,
    kBus,
    kWater,
    kHelicopter,
    kUndefined
};

std::ostream& operator<<(std::ostream& stream, const TransportType& t);

struct Thread {
    bool is_transfer;
    std::string title;
    TransportType transport_type;
    std::string from;
    std::string to;
    uint64_t duration;
    std::string departure;
    std::string arrival;
};

struct Segment : public Thread {
    std::string transfer_point;
    std::vector<Thread> details;
};

void from_json(const json& j, TransportType& transport_type);
void from_json(const json& j, Segment& segment);
void from_json(const json& j, Thread& thread);
void to_json(json& j, const TransportType& transport_type);
void to_json(json& j, const Segment& segment);
void to_json(json& j, const Thread& thread);


