#include "Engine.h"

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



constexpr Side Buy = Side::Buy;
constexpr Side Sell = Side::Sell;

int main() {
    
    Engine eng;

    eng.submitOrder(Buy, 100, 500);
    eng.submitOrder(Buy, 100, 300);
    eng.submitOrder(Buy, 105, 200);
    eng.submitOrder(Buy, 105, 200);
    eng.submitOrder(Buy, 99, 100);

    eng.submitOrder(Sell, 100, 300);
    eng.submitOrder(Sell, 100, 500);
    eng.submitOrder(Sell, 105, 500);
    eng.submitOrder(Sell, 98, 200);

    eng.printBook();
    
    std::cout << eng.getTradeQuantity() << "\n\n";

    std::cout << eng.getNumberOfTrades() << "\n\n";

    std::cout << eng.getAveragePrice() << "\n\n";

    std::cout << eng.getVolumeWeightedAveragePrice() << "\n\n";

    return 0;
}