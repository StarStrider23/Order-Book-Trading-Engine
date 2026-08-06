#include "OrderBook.h"

void OrderBook::addToBook(const Order& order) {

    switch (order.getSide()) {

        case Side::Buy:

            buyBook[order.getPrice()].push_back(order);
            break;

        case Side::Sell:

            sellBook[order.getPrice()].push_back(order);
            break;
    }

}

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