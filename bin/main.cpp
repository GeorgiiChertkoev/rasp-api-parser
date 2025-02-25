#include <iostream>
#include <string>
#include <regex>

#include "lib/rasp_requester/rasp_requester.h"

std::string GetApiKey() {
    std::ifstream config("../config.json");
    if (!config.is_open()) {
        return "";
    }
    return json::parse(config).value("apikey", "");
}

void GetAndPrint(const std::string& date, const std::string& from, const std::string& to) {
    RaspRequester requester(GetApiKey().c_str());
    std::expected<RaspResponse, ErrorType> r = requester.Get(from, to, date);
    if (!r.has_value()) {
        std::cerr << "error happened\n";
        return ;
    }
    std::cout << "Маршруты " << r.value().search_from << " -> " << r.value().search_to;
    if (r.value().segments.size() == 0) {
        std::cout << " с неболее чем одной пересадкой не найдены";
    }
    std::cout << '\n';
    for (const Segment& segment : r.value().segments) {
        if (segment.arrival.timestamp < 10000) {
            std::exit(1);
        }
        std::cout << segment << '\n';
        std::cout << "<--->" << '\n';
    }
}


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "incorrect amount of arguments (expected 2, got " << argc << ")\n";
        return 1;
    }
    const std::regex date_format(R"(^\d{4}-\d{2}-\d{2}$)");
    std::string date(argv[1]);
    if (!std::regex_match(date, date_format)) {
        std::cerr << "incorrect date format\n";
        std::cerr << "use YYYY-MM-DD\n";
        return 1;
    }

    GetAndPrint(date, "c2", "c968");
    GetAndPrint(date, "c968", "c2");

    return 0;
}