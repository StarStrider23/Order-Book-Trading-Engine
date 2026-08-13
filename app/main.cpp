#include "Engine.h"

// Order Book & Trading Engine

/* A C++ implementation of an electronic trading matching engine 
featuring price-time priority, limit order handling, dynamic order 
book management, trade execution, order modification, cancellation, 
and execution history tracking.*/

// Add timestamps? order submission/addition/modification/cancellation; trade
// Add order modification history 

/* 
Any time a .cpp file is changed:

cd build
cmake --build .
./TradingApp

If adding a new class (new .h/.cpp files): 

cd build
cmake ..
cmake --build .
./TradingApp

To run:

Once in the build directory:

./TradingApp

*/

// cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
// cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Debug



constexpr Side Buy = Side::Buy;
constexpr Side Sell = Side::Sell;

int main() {

    Engine eng;

    Order buy = eng.submitOrder(Side::Buy, 100, 100);

    eng.submitOrder(Side::Sell, 100, 70);
    eng.submitOrder(Side::Sell, 90, 30);

    return 0;
}