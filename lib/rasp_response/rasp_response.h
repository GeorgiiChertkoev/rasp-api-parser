#pragma once 
#include "lib/segment/segment.h"

struct RaspResponse {
    std::string search_from;
    std::string search_to;
    std::string search_date;
    time_t response_date;

    std::vector<Segment> segments;
};

void from_json(const json& j, RaspResponse& r);
void to_json(json& j, const RaspResponse& r);