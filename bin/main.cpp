#include "iostream"
#include "string"
#include "ostream"

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

// #include "lib/rasp_response/rasp_response.h"
#include "lib/rasp_requester/rasp_requester.h"

const char* kApiKey = "7ca52401-0466-4ad4-9a20-afe69440ac7a";

inline cpr::Response GetYandexRasp(std::string from, std::string to, std::string date) {
    return cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"}, 
                    cpr::Parameters({{"apikey", kApiKey}, {"format", "json"}, 
                        {"lang", "ru_RU"}, {"transfers", "true"},
                        {"from", from}, {"to", to}, 
                        {"date", date}}));
}

void PrintResponse(cpr::Response r) {
    // std::cout << r.url << '\n';
    // std::cout << r.status_code << '\n';
    // std::cout << r.header["content-type"] << '\n';
    std::ofstream file("out.json");
    file << r.text;
    file.close();
    // std::cout << r.text << '\n';
}

int main() {
    std::string date = "2025-02-10";
    RaspRequester requester;
    std::expected<RaspResponse, ErrorType> r = requester.Get("c967", "c2", date);
    if (!r.has_value()) {
        std::cout << "error happened\n";
    }
    std::ofstream back("backwards.json");
    // RaspResponse res = ;
    json out = r.value();
    std::cout << "will write\n";
    back << out << std::endl;
    std::cout << "wrote\n";
    return 0;


    // // PrintResponse(GetYandexRasp("c967", "c2", date));
    // PrintResponse(GetYandexRasp("c967", "c2", date));
    // std::ifstream file("out.json");
    // json j = json::parse(file);
    // std::cout << j["search"]["from"]["title"] << " -> " << j["search"]["to"]["title"] << '\n';
    // // PrintResponse(GetYandexRasp("c967", "c2", date));

    // r = j.get<RaspResponse>();
    // // RaspResponse r = j.get<RaspResponse>();
    // // json out = r;
    // // std::ofstream back("backwards.json");
    // out = r;
    // std::cout << "will write\n";
    // back << out << std::endl;
    // std::cout << "wrote\n";
    // RaspResponse other = out.get<RaspResponse>();
    // std::cout << r.segments.size() << '\n';
    // std::cout << r.search_from << '\n';
    // // for (const json& s : data.at("segments")) {
    // for (const Segment& data : r.segments) {
    //     // if (s.contains("transfers") && s["transfers"].size() > 1) {
    //         // continue;
    //     // }
    //     std::cout << 1 << '\n';
    //     // Segment data = s.get<Segment>();
    //     std::cout << "123\n";
    //     std::cout << data.title << " on " << data.transport_type << '\n';
    //     std::cout << "123\n";
    //     if (data.details.size() != 0) {
    //         for (const Thread& thread : data.details) {
    //             std::cout << thread.from << " -> " << thread.to << '\n';
    //         }
    //     }
    //     std::cout << "departure at " << data.departure << '\n';
    //     std::cout << "arrival at   " << data.arrival << '\n';
    //     std::cout << "<--->\n";
    // }
    
    // // Segment t = data.at("segments").at(0).get<Segment>();
    // // std::cout << t.title << '\n';
    // // }

    // return 0;
}