#include "segment.h"



Segment ParseThread(const json& j) {
    Segment thread;
    // thread.transport_types.push_back(j.at("thread").at("transport_type").get<TransportType>());
    if (j.contains("transfer_from") && j.contains("transfer_to")) {
        thread.is_transfer = true;
        j.at("transfer_from").at("title").get_to(thread.from);
        j.at("transfer_to").at("title").get_to(thread.to);
    } else {
        thread.is_transfer = false;
        j.at("thread").at("title").get_to(thread.title);
        j.at("from").at("title").get_to(thread.from);
        j.at("to").at("title").get_to(thread.to);
        j.at("departure").get_to(thread.departure);
        j.at("arrival").get_to(thread.arrival);
        j.at("thread").at("transport_type").get_to(thread.transport_type);
    }

    j.at("duration").get_to(thread.duration);
    return thread;
}



void from_json(const json& j, Thread& thread) {
    thread = ParseThread(j);
}


void ParseWithTransfers(const json& j, Segment& segment) {
    // segment.is_transfer = false;
    j.at("departure_from").at("title").get_to(segment.from);
    j.at("arrival_to").at("title").get_to(segment.to);
    j.at("transfers").at(0).at("title").get_to(segment.transfer_point);
    segment.title = segment.from + " -> "
                    + segment.transfer_point + " -> "
                    + segment.to;
    for (const json& d : j.at("details")) {
        segment.details.push_back(ParseThread(d));
    }
    // j.at("duration").get_to(segment.duration);
}


void from_json(const json& j, TransportType& transport_type) {
    if (j.is_number_integer()) {
        transport_type = static_cast<TransportType>(j.get<int>());
        return;
    }
    std::string line = j.get<std::string>();
    if (line == "plane") {
        transport_type = TransportType::kPlane;
    } else if (line == "train") {
        transport_type = TransportType::kTrain;
    } else if (line == "suburban") {
        transport_type = TransportType::kSuburban;
    } else if (line == "bus") {
        transport_type = TransportType::kBus;
    } else if (line == "water") {
        transport_type = TransportType::kWater;
    } else if (line == "helicopter") {
        transport_type = TransportType::kHelicopter;
    } else {
        transport_type = TransportType::kUndefined;
    }
}


void from_json(const json& j, Segment& segment) {
    // на одном уровне с segment : {}
    // if (j.contains("departure") && j.contains("arrival")) {
    j.at("departure").get_to(segment.departure);
    j.at("arrival").get_to(segment.arrival);
    // }
    if (j.contains("has_transfers")) {
        if (j.at("has_transfers").get<bool>()) {
            std::cerr << "parsing with transfer\n";
            ParseWithTransfers(j, segment);
        } else {
            std::cerr << "parsing without transfer\n";
            segment = ParseThread(j);
            // from_json(j, reinterpret_cast<Thread>(segment));
        }
    } else {
        std::cerr << "WHAT IS THAT\n";
    }
}


std::ostream& operator<<(std::ostream& stream, const TransportType& t) {
    switch (t) {
    case TransportType::kPlane:
        stream << "самолет";
        break;
    case TransportType::kTrain:
        stream << "поезд";
        break;
    case TransportType::kSuburban:
        stream << "электричка";
        break;
    case TransportType::kBus:
        stream << "автобус";
        break;
    case TransportType::kWater:
        stream << "морской транспорт";
        break;
    case TransportType::kHelicopter:
        stream << "вертолет";
        break;
    case TransportType::kUndefined:
        stream << "неизвестно";
        break;
    }
    return stream;
}

void to_json(json& j, const TransportType& transport_type) {
    j = static_cast<int>(transport_type);
}


void to_json(json& j, const Thread& thread) {
    // j = ThreadToJson(thread);
    j["duration"] = thread.duration;
    if (thread.is_transfer) {
        j["is_transfer"] = true;
        j["transfer_from"]["title"] = thread.from;
        j["transfer_to"]["title"] = thread.to;
        return;
    }
    j["departure"] = thread.departure;
    j["arrival"] = thread.arrival;
    j["to"]["title"] = thread.to;
    j["from"]["title"] = thread.from;
    j["thread"] = {
        {"title", thread.title},
        {"transport_type", thread.transport_type}
    };
}

void to_json(json& j, const Segment& segment) {
    // DO THIS
    if (segment.details.size() > 0) {
        j["has_transfers"] = true;
        j["departure_from"]["title"] = segment.from;
        j["arrival_to"]["title"] = segment.to;
        j["departure"] = segment.departure;
        j["arrival"] = segment.arrival;
        json transfer;
        transfer["title"] = segment.transfer_point;
        j["transfers"].push_back(transfer);
        for (const Thread& t : segment.details) {
            j["details"].push_back(t);
        }
    } else {
        to_json(j, (Thread) segment);
        j["has_transfers"] = false;
    }
}
