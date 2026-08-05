cmake_minimum_required(VERSION 3.20)

project(TradingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/Engine.cpp
    src/Order.cpp
    src/OrderBook.cpp
    src/Trade.cpp
    src/Types.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_options(TradingEngine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(
    TradingApp
    app/main.cpp
)

target_link_libraries(TradingApp PRIVATE TradingEngine)