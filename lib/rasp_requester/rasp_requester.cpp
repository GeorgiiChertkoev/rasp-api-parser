#include "rasp_requester.h"

std::expected<RaspResponse, ErrorType> RaspRequester::Get(std::string from, std::string to, std::string date) {
    // check cache
    std::string cache_filename = from + '_' + to + '_' + date + ".json";
    std::optional<RaspResponse> from_cache = GetFromCache(cache_filename);
    if (from_cache.has_value()) {
        return from_cache.value();
    }
    std::cout << "no cache\n";
    cpr::Response cpr_resp = GetYandexRasp(from, to, date);
    if (cpr_resp.status_code != 200) {
        return {};
    }
    RaspResponse r;
    json j = json::parse(cpr_resp.text);
    std::ofstream file("cpr_resp.json");
    try {
        file << j << std::endl;
        r = j.get<RaspResponse>();
        time_t now;
        time(&now);
        r.response_date = now;
        WriteCache(r, cache_filename);
    }
    catch (...) {
        std::cout << "caught error\n";
        return {};
    }
    // write cache;
    return r;
    // cache will be in folder "cache" in format "{from}_{to}_{date}.json"
}


std::optional<RaspResponse> RaspRequester::GetFromCache(std::string filename) {
    filename = "cache/" + filename;
    if (!std::filesystem::exists(filename)) {
        return {};
    }
    
    std::ifstream cache_file(filename);
    RaspResponse r = json::parse(cache_file);
    time_t now;
    time(&now);
    if (now - r.response_date >= kCacheInvalidationPeriod) {
        std::cerr << "cache is too old\n";
        return {};
    }
    return r;
}


inline cpr::Response RaspRequester::GetYandexRasp(std::string from, std::string to, std::string date) {
    return cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"}, 
                    cpr::Parameters({{"apikey", kApiKey}, {"format", "json"}, 
                        {"lang", "ru_RU"}, {"transfers", "true"},
                        {"from", from}, {"to", to}, 
                        {"date", date}}));
}




void RaspRequester::WriteCache(const RaspResponse& rasp, std::string filename) {
    std::filesystem::create_directory("cache");
    std::ofstream file("cache/" + filename);
    file << (json) rasp;
    file.close();
}



// CreateCacheDir();