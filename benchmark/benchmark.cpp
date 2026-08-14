#include "Engine.h"

#include <random>
#include <chrono>

// Before running the test, make sure to:
// 1. Remove notification about order submission (Engine.cpp line 36 (and 69 if including Market Orders))
// 2. Remove notification about trades (Engine.cpp line 812) 
// 3. Build Release (cmake -S . -B build -DCMAKE_BUILD_TYPE=Release)

// P.S. To return to the Debug Build, type: cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Debug

// Results:

// 100k orders: 
// Elapsed: 0.0804477 seconds
// Throughput: 1.24304e+06 orders/sec

// 1M orders:
// Elapsed: 1.12879 seconds
// Throughput: 885903 orders/sec

int main() {

    const int numberOfOrders = 1000000;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> priceDist(9000, 11000);
    std::uniform_int_distribution<int> quantityDist(1, 1000);
    std::uniform_int_distribution<int> sideDist(0, 1);

    Engine eng;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < numberOfOrders; ++i) {

        int priceTicks = priceDist(gen);
        double price = priceTicks / 100.0;

        int quantity = quantityDist(gen);

        Side side = sideDist(gen) == 0 ? Side::Buy : Side::Sell;

        eng.submitOrder(side, price, quantity);
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration<double>(end - start);

    double seconds = elapsed.count();
    double throughput = numberOfOrders / seconds;

    std::cout << "Elapsed: " << seconds << " seconds\n";
    std::cout << "Throughput: " << throughput << " orders/sec\n";

    eng.printBookStatistics();

    eng.printTradeStatistics();

    return 0;
}