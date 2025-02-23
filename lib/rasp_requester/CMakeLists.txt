add_library(rasp_requester rasp_requester.cpp rasp_requester.h)

target_link_libraries(rasp_requester PRIVATE rasp_response)
target_link_libraries(rasp_requester PRIVATE cpr::cpr)
target_include_directories(rasp_requester PRIVATE ${PROJECT_SOURCE_DIR})

