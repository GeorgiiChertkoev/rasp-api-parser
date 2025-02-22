#include "rasp_requester.h"

RaspResponse RaspRequester::Get(std::string from, std::string to, std::string date) {
    // check cache
    std::optional<RaspResponse> from_cache = GetFromCache(from + '_' + to + '_' + date + ".json");
    if (from_cache.has_value()) {
        return from_cache.value();
    }
    // cache will be in folder "cache" in format "{from}_{to}_{date}.json"
}
std::optional<RaspResponse> RaspRequester::GetFromCache(std::string filename) {
    filename = "cache/" + filename;
    if (!std::filesystem::exists(filename)) {
        return {};
    }
    std::ifstream cache_file(filename);
    RaspResponse r = json::parse(cache_file);
    // if (r.response_date - )
    return r;
}
