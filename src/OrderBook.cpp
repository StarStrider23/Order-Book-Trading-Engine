#include "OrderBook.h"

// ************************************************************
// PRIVATE METHODS
// ************************************************************

/*

1. Add Order To Book
2. Print Book

*/

// ============================================================
// 1. Add Ordeer To Book
// ============================================================

void OrderBook::addToBook(const Order& order) {

    switch (order.getSide()) {

        case Side::Buy:

            buyBook[order.getPrice().value()].push_back(order);
            break;

        case Side::Sell:

            sellBook[order.getPrice().value()].push_back(order);
            break;
    }

}

// ============================================================
// 1. Print Book
// ============================================================

void OrderBook::printBook() const {

    std::cout << "-----------------------------" << "\n\n";
    std::cout << "BUY" << "\n\n";
    std::cout << "-----------------------------" << "\n\n";

    for (const auto& [price, orders] : buyBook) {

        std::cout <<  "PRICE: " << price << "\n\n";
        
        for (const Order& order : orders) {
            std::cout << order << "\n";
        }
        std::cout << "-----------------------------" << "\n\n";
    }

    std::cout << "SELL" << "\n\n";
    std::cout << "-----------------------------" << "\n\n";

    for (const auto& [price, orders] : sellBook) {

        std::cout <<  "PRICE: " << price << "\n\n";

        for (const Order& order : orders) {
            std::cout << order << "\n";
        }

        std::cout << "-----------------------------" << "\n\n";
        }
}
