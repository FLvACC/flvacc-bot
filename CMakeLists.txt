cmake_minimum_required(VERSION 4.1)

project(flvacc-bot)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/config.cpp
    src/listeners.cpp
    src/commands/ping.cpp
    src/commands/apply.cpp
)

find_package(DPP REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    ${DPP_LIBRARIES}
    libpqxx::pqxx
    fmt::fmt-header-only
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DPP_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/commands
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    EXPORT_COMPILE_COMMANDS ON
)
