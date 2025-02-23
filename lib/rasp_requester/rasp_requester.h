#pragma once
#include <optional>
#include <expected>
#include <filesystem>
#include <fstream>
#include "cpr/cpr.h"
#include "lib/rasp_response/rasp_response.h"


// CreateCacheDir();

enum class ErrorType {
    kNoError,
    kBadRequest
};

class RaspRequester {
public: 
    static const time_t kCacheInvalidationPeriod = 12 * 3600;

    std::expected<RaspResponse, ErrorType> Get(std::string from, std::string to, std::string date);

private:
    const char* kApiKey = "7ca52401-0466-4ad4-9a20-afe69440ac7a";


    inline cpr::Response GetYandexRasp(std::string from, std::string to, std::string date);
    std::optional<RaspResponse> GetFromCache(std::string filename);
    void WriteCache(const RaspResponse& rasp, std::string filename);
    
};