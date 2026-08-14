#include "Engine.h"

// Order Book & Trading Engine

/* A C++ implementation of an electronic trading matching engine 
featuring price-time priority, limit order handling, dynamic order 
book management, trade execution, order modification, cancellation, 
and execution history tracking.*/

constexpr Side Buy = Side::Buy;
constexpr Side Sell = Side::Sell;

int main() {

    Engine eng;

    std::cout << "=== SUBMITTING ORDERS ===" << "\n\n";

    Order buy = eng.submitOrder(Side::Buy, 100, 100);

    eng.printBook();

    Order sell = eng.submitOrder(Side::Sell, 100, 60);

    std::cout << "\n" << "=== BUY ORDER INFORMATION ===" << "\n\n";
    eng.printOrderInformationById(buy.getId());

    std::cout << "\n" << "=== COMPLETING BUY ORDER ===" << "\n\n";

    eng.submitOrder(Side::Sell, 100, 40);

    eng.printOrderInformationById(buy.getId());

    std::cout << "\n" << "=== MODIFYING ORDER ===" << "\n\n";

    Order modified = eng.submitOrder(Side::Buy, 95, 100);

    eng.changeOrderPrice(modified.getId(), 97);

    eng.printOrderInformationById(modified.getId());
    eng.printOrderModificationHistoryById(modified.getId());

    std::cout << "\n" << "=== CANCELLING ORDER ===" << "\n\n";

    eng.cancelOrder(modified.getId());

    eng.printOrderInformationById(modified.getId());

    std::cout << "\n" << "=== BOOK STATISTICS ===" << "\n";

    eng.printBookStatistics();
    eng.printTradeStatistics();

    return 0;
}