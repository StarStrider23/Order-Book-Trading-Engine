cmake_minimum_required(VERSION 3.20)

project(TradingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
    ${PROJECT_NAME}
    src/Engine.cpp
    src/Order.cpp
    src/OrderBook.cpp
    src/Trade.cpp
    src/Types.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(
    TradingApp
    app/main.cpp
)

target_link_libraries(TradingApp PRIVATE ${PROJECT_NAME})