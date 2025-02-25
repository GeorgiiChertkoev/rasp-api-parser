#include "rasp_response.h"


void from_json(const json& j, RaspResponse& r) {
    j.at("search").at("from").at("title").get_to(r.search_from);
    j.at("search").at("to").at("title").get_to(r.search_to);
    j.at("search").at("date").get_to(r.search_date);
    r.response_date = j.value("response_date", 0);
    r.segments.reserve(j.at("segments").size());
    for (const json& segment : j.at("segments")) {
        if (segment.value("has_transfers", false) && 
                segment.at("transfers").size() > 1) {
            continue;
        }        
        r.segments.push_back(segment.get<Segment>());
    }
}

void to_json(json& j, const RaspResponse& r) {
    j["search"]["from"]["title"] = r.search_from; 
    j["search"]["to"]["title"] = r.search_to;
    j["search"]["date"] = r.search_date; 
    j["response_date"] = r.response_date;
    j["segments"] = json::array();
    for (const Segment& s : r.segments) {
        j["segments"].push_back(s);
    }
}
