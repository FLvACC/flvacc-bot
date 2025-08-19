cmake_minimum_required(VERSION 3.22)

project(flvacc-bot)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(${PROJECT_NAME} src/main.cpp)

find_package(DPP REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} ${DPP_LIBRARIES}
    nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME} PRIVATE ${DPP_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
