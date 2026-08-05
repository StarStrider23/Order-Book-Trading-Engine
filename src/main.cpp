#include "git sEngine.h"

// Order Book & Trading Engine

/* A C++ implementation of an electronic trading matching engine 
featuring price-time priority, limit order handling, dynamic order 
book management, trade execution, order modification, cancellation, 
and execution history tracking.*/

// Add time?
// Add stock names?

/* 
Any time a .cpp file is changed:

cd build
cmake --build .
./TradingingEngine 

If adding a new class (new .h/.cpp files): 

cd build
cmake ..
cmake --build .
./TradingEngine 

To run:

Once in the build directory:

./TradingEngine

*/

constexpr Side Buy = Side::Buy;
constexpr Side Sell = Side::Sell;

int main() {
    
    Engine eng;

    eng.submitOrder(Buy, 100, 30);
    eng.submitOrder(Sell, 105, 80);
    eng.submitOrder(Sell, 100, 50);
    eng.submitOrder(Buy, 110, 100);

    eng.submitOrder(Buy, 100, 50);
    eng.submitOrder(Buy, 100, 75);
    eng.submitOrder(Sell, 150, 50);
    eng.changeOrder("B3", 150, 50);
    eng.printBook();

    return 0;
}