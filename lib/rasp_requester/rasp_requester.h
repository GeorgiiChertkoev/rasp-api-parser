#pragma once
#include <optional>
#include <expected>
#include <filesystem>
#include <fstream>
#include "cpr/cpr.h"
#include "lib/rasp_response/rasp_response.h"


enum class ErrorType {
    kNoError,
    kJsonError,
    kFileSystemError,
    kUnkownError,
    kBadRequest = 400,
    kPageNotFound = 404,
    kUnknownRequestError
};

class RaspRequester {
public:
    time_t cache_lifetime;
    const char* api_key;

    RaspRequester(const char* api_key, time_t cache_lifetime = 12 * 3600) : 
        api_key(api_key),
        cache_lifetime(cache_lifetime) {}
    RaspRequester(time_t cache_lifetime = 12 * 3600) : 
        cache_lifetime(cache_lifetime) {}

    std::expected<RaspResponse, ErrorType> Get(std::string from, std::string to, 
        std::string date, std::ostream& err_stream = std::cerr);

private:
    inline cpr::Response GetYandexRasp(std::string from, std::string to, std::string date);
    std::optional<RaspResponse> GetFromCache(std::string filename);
    void WriteCache(const RaspResponse& rasp, std::string filename);
};