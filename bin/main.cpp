#include <iostream>
#include <string>
#include <regex>

// #include "cpr/cpr.h"
// #include "nlohmann/json.hpp"
// using json = nlohmann::json;

#include "lib/rasp_requester/rasp_requester.h"

// const char* kApiKey = "7ca52401-0466-4ad4-9a20-afe69440ac7a";


void GetAndPrint(const std::string& date, const std::string& from, const std::string& to) {
    RaspRequester requester("7ca52401-0466-4ad4-9a20-afe69440ac7a");
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
        std::cout << segment << '\n';
        std::cout << "<--->" << '\n';
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "incorrect amount of arguments\n";
        return 1;
    }
    const std::regex date_format(R"(^\d{4}.\d{2}.\d{2}$)");
    std::string date(argv[1]);
    if (!std::regex_match(date, date_format)) {
        std::cerr << "incorrect date format\n";
        std::cerr << "use YYYY-MM-DD\n";
    }
    GetAndPrint(date, "c968", "c2");
    GetAndPrint(date, "c2", "c968");

    return 0;
}