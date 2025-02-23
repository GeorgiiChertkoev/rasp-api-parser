add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE rasp_response)
target_link_libraries(${PROJECT_NAME} PRIVATE rasp_requester)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
