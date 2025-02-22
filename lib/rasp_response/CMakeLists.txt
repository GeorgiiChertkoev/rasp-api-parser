add_library(rasp_response rasp_response.h rasp_response.cpp)

target_link_libraries(rasp_response PUBLIC segment)

target_include_directories(rasp_response PRIVATE ${PROJECT_SOURCE_DIR})
