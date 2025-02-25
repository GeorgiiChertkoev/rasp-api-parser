#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

const char* kDatetimeFormat = "%Y-%m-%dT%H:%M:%S±hh:mm";
const char* kPrettyDatetimeFormat = "%d.%m.%Y %H:%M:%S";

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

struct TimeWithTimezone {
    time_t timestamp;
    int32_t zone_in_mins;
};

std::ostream& operator<<(std::ostream& stream, const TimeWithTimezone& t);

struct Thread {
    bool is_transfer;
    std::string title;
    TransportType transport_type;
    std::string from;
    std::string to;
    uint64_t duration;
    TimeWithTimezone departure;
    TimeWithTimezone arrival;
};

struct Segment : public Thread {
    std::string transfer_point;
    std::vector<Thread> details;
};

std::ostream& operator<<(std::ostream& stream, const Thread& thread);
std::ostream& operator<<(std::ostream& stream, const Segment& segment);


void from_json(const json& j, TransportType& transport_type);
void from_json(const json& j, TimeWithTimezone& time);
void from_json(const json& j, Segment& segment);
void from_json(const json& j, Thread& thread);
void to_json(json& j, const TransportType& transport_type);
void to_json(json& j, const TimeWithTimezone& time);
void to_json(json& j, const Segment& segment);
void to_json(json& j, const Thread& thread);


