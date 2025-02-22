#include "rasp_response.h"


void from_json(const json& j, RaspResponse& r) {
    j.at("search").at("from").at("title").get_to(r.search_from);
    j.at("search").at("to").at("title").get_to(r.search_to);
    j.at("search").at("date").get_to(r.search_date);
    // when to write response date
    r.segments.resize(j.at("segments").size());
    int i = 0;
    std::cout << r.segments.size() << ' ' << r.segments.capacity() << '\n';
    for (const json& segment : j.at("segments")) {
        if (segment.value("has_transfers", false) && 
                segment.at("transfers").size() > 1) {
            continue;
        }
        
        // std::cout << "segment " << i << '\n'; 

        segment.get_to(r.segments[i]);
        i++;
    }
    r.segments.shrink_to_fit();
}


void to_json(json& j, const RaspResponse& r) {
    j["search"]["from"]["title"] = r.search_from; 
    j["search"]["to"]["title"] = r.search_to;
    j["search"]["date"] = r.search_date; 
    int i = 0;
    for (const Segment& s : r.segments) {
        j["segments"].push_back(s);
    }
}
