#include "iostream"
#include "string"
#include "ostream"
#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

const std::string kApiKey = "7ca52401-0466-4ad4-9a20-afe69440ac7a";

inline cpr::Response GetYandexRasp(std::string from, std::string to, std::string date) {
    return cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"}, 
                    cpr::Parameters({{"apikey", kApiKey}, {"format", "json"}, 
                        {"lang", "ru_RU"}, {"transfers", "true"},
                        {"from", from}, {"to", to}, {"date", date}}));
}

void PrintResponse(cpr::Response r) {
    std::cout << r.url << '\n';
    std::cout << r.status_code << '\n';
    std::cout << r.header["content-type"] << '\n';
    std::ofstream file("out.json");
    file << r.text;
    file.close();
    // std::cout << r.text << '\n';
}

int main() {
    std::string date = "2025-02-10";
    PrintResponse(GetYandexRasp("c968", "c2", date));
    json Doc;
    // PrintResponse(GetYandexRasp("c967", "c2", date));
    // std::cout << "good!\n";

    return 0;
}