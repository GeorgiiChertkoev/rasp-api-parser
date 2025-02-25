#include "rasp_requester.h"


std::optional<RaspResponse> RaspRequester::GetFromCache(std::string filename) {
    filename = "cache/" + filename;
    if (!std::filesystem::exists(filename)) {
        return {};
    }
    std::ifstream cache_file(filename);
    RaspResponse r = json::parse(cache_file);
    time_t now;
    time(&now);
    if (now - r.response_date >= cache_lifetime) {
        std::cerr << "cache is too old\n";
        return {};
    }
    return r;
}


std::expected<RaspResponse, ErrorType> RaspRequester::Get(std::string from, std::string to, 
                                                          std::string date, std::ostream& err_stream) {
    std::string cache_filename = from + '_' + to + '_' + date + ".json";
    std::optional<RaspResponse> from_cache;
    try {
        from_cache = GetFromCache(cache_filename);
    } catch (...) {
        from_cache.reset();
    }
    if (from_cache.has_value()) {
        return from_cache.value();
    }
    cpr::Response cpr_resp = GetYandexRasp(from, to, date);
    if (cpr_resp.status_code == 400) {
        err_stream << "BadRequest" << std::endl;
        return std::unexpected(ErrorType::kBadRequest);
    } else if (cpr_resp.status_code == 404) {
        err_stream << "PageNotFound" << std::endl;
        return std::unexpected(ErrorType::kPageNotFound);   
    } else if (cpr_resp.status_code != 200)  {
        err_stream << "UnknownRequestError" << std::endl;
        return std::unexpected(ErrorType::kUnknownRequestError);
    }
    RaspResponse r;
    json j = json::parse(cpr_resp.text);
    try {
        r = j.get<RaspResponse>();
        time_t now;
        time(&now);
        r.response_date = now;
        WriteCache(r, cache_filename);
    }
    catch (const json::exception& ex) {
        err_stream << ex.what() << std::endl;
        return std::unexpected(ErrorType::kJsonError);
    }
    catch (const std::filesystem::filesystem_error& fs_er) {
        err_stream << fs_er.what() << std::endl;
        return std::unexpected(ErrorType::kFileSystemError);
    }
    catch (const std::exception& e) {
        err_stream << e.what() << std::endl;
        return std::unexpected(ErrorType::kUnkownError);
    }
    return r;
}


inline cpr::Response RaspRequester::GetYandexRasp(std::string from, std::string to, std::string date) {
    return cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"}, 
                    cpr::Parameters({{"apikey", api_key}, {"format", "json"}, 
                        {"lang", "ru_RU"}, {"transfers", "true"},
                        {"from", from}, {"to", to}, 
                        {"date", date}}));
}


void RaspRequester::WriteCache(const RaspResponse& rasp, std::string filename) {
    std::filesystem::create_directory("cache");
    std::ofstream file("cache/" + filename);
    file << static_cast<json>(rasp);
    file.close();
}
