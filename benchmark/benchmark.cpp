#include "Engine.h"

#include <random>
#include <chrono>

// 100k orders: 
// Elapsed: 0.152422 seconds
// Throughput: 656073 orders/sec

// 1M orders:
// Elapsed: 1.49847 seconds
// Throughput: 667347 orders/sec

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