#pragma once
#include <optional>
#include <filesystem>
#include <fstream>
#include "lib/rasp_response/rasp_response.h"

const time_t kCacheInvalidationPeriod = 4 * 3600;

class RaspRequester {
public: 
    RaspResponse Get(std::string from, std::string to, std::string date);

    
private:
    std::optional<RaspResponse> GetFromCache(std::string filename);
};