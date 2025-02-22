add_library(segment segment.cpp segment.h)

target_link_libraries(segment PUBLIC nlohmann_json::nlohmann_json)
