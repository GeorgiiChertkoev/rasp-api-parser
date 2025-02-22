#include "rasp_response.h"


void from_json(const json& j, RaspResponse& r) {
    j.at("search").at("from").at("title").get_to(r.search_from);
    j.at("search").at("to").at("title").get_to(r.search_to);
    j.at("search").at("date").get_to(r.search_date);
    // when to write response date
    r.segments.resize(j.at("segments").size());
    int i = 0;
    for (const json& segment : j.at("segments")) {
        if (segment.value("has_transfers", false) && 
                segment.at("transfers").size() > 1) {
            continue;
        }
        segment.get_to(r.segments[i]);
        i++;
    }
}


void to_json(json& j, const RaspResponse& r) {
    j["search"] = {
        {"from", {"title", r.search_from}}, 
        {"to", {"title", r.search_to}},
        {"date", r.search_date}
    };
    for (const Segment& s : r.segments) {
        j["segments"].push_back(s);
    }
}
