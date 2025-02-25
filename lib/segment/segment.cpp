#include "segment.h"



time_t ParseTime(const json& j) {
    if (j.is_number_integer()) {
        return j.get<time_t>();
    }
    std::istringstream ss(j.get<std::string>());
    std::tm tm = {};
    ss >> std::get_time(&tm, kDatetimeFormat);
    return mktime(&tm);
}

Segment ParseThread(const json& j) {
    Segment thread;
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
    j.at("departure_from").at("title").get_to(segment.from);
    j.at("arrival_to").at("title").get_to(segment.to);
    j.at("transfers").at(0).at("title").get_to(segment.transfer_point);
    segment.title = segment.from + " -> "
                    + segment.transfer_point + " -> "
                    + segment.to;
    for (const json& d : j.at("details")) {
        segment.details.push_back(ParseThread(d));
        segment.duration += segment.details.back().duration;
    }
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
    j.at("departure").get_to(segment.departure);
    j.at("arrival").get_to(segment.arrival);
    if (j.value("has_transfers", false)) {
        ParseWithTransfers(j, segment);
    } else {
        segment = ParseThread(j);
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

void from_json(const json& j, TimeWithTimezone& time) {
    std::string s = j.get<std::string>();
    std::istringstream ss(s);
    std::tm tm = {};
    ss >> std::get_time(&tm, kDatetimeFormat);
    time.timestamp = mktime(&tm);
    time.zone_in_mins = 60 * std::stoi(s.substr(20, 2));
    time.zone_in_mins += std::stoi(s.substr(23, 2));
    if (s[19] == '-') {
        time.zone_in_mins *= -1;
    }
    
}


void to_json(json& j, const TimeWithTimezone& time) {
    std::string res = "";
    std::tm* tm_ptr = std::localtime(&time.timestamp);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), kDatetimeFormat, tm_ptr);
    buffer[19] = '\0';
    time_t temp = time.zone_in_mins;
    if (time.zone_in_mins < 0) {
        res += '-';
        temp *= -1;
    } else {
        res += '+';
    }
    if (temp / 60 < 10) {
        res += '0';
    }
    res += std::to_string(temp / 60);
    res += ':';
    if (temp % 60 < 10) {
        res += '0';
    }
    res += std::to_string(temp % 60);
    j = std::string(buffer) + res;
}


std::ostream& operator<<(std::ostream& stream, const TimeWithTimezone& t) {
    std::tm* tm_ptr = std::localtime(&t.timestamp);
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), kPrettyDatetimeFormat, tm_ptr);
    stream << buffer << " (";
    if (t.zone_in_mins < 0) {
        stream << '-';
    } else {
        stream << '+';
    }
    stream << t.zone_in_mins / 60 << ":" << t.zone_in_mins % 60 << ')';
    return stream;
}


void to_json(json& j, const Thread& thread) {
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


std::ostream& operator<<(std::ostream& stream, const Thread& thread) {
    if (thread.is_transfer) {
        stream << "пересадка: ";
        stream << '"' << thread.from << '"' << " -> " << '"' <<  thread.to << '"';
        return stream;
    }
    stream << thread.transport_type << ": " << thread.title  << '\n';
    stream << '"' << thread.from << '"' << " -> " << '"' <<  thread.to << '"' << '\n';
    stream << "Отправление: " << thread.departure << '\n';
    stream << "Прибытие:    " << thread.arrival;
    return stream;
}

std::ostream& PrettyPrint(std::ostream& stream, const Thread& thread,
                          int indentation_level = 0) {
    std::string indent(indentation_level * 3, ' ');
    if (thread.is_transfer) {
        stream << indent << "пересадка: ";
        stream << '"' << thread.from << '"' << " -> " << '"' <<  thread.to << '"';
        return stream;
    }
    if (indentation_level == 0) {
        stream << "прямой ";
    }
    stream << indent << thread.transport_type << ": " << thread.title  << '\n';
    stream << indent << "   " << '"' << thread.from << '"' << " -> " << '"' <<  thread.to << '"' << '\n';
    stream << indent << "   " << "Отправление: " << thread.departure << '\n';
    stream << indent << "   " << "Прибытие:    " << thread.arrival;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Segment& segment) {
    if (segment.details.size() == 0) {
        PrettyPrint(stream, segment);
        return stream;
    }

    stream << segment.title << '\n';
    for (size_t i = 0; i < segment.details.size(); i++) {
        PrettyPrint(stream, segment.details[i], 1);
        if (i != segment.details.size() - 1) {
            stream << '\n';
        }
    }

    return stream;
}



