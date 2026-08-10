#include "Types.h"

#include <iostream>
#include <iomanip>

// Side

std::ostream& operator<<(std::ostream& os, const Side& side) {

    switch (side) {

        case Side::Buy:

            os << "BUY";
            break;

        case Side::Sell:

            os << "SELL";
            break;
    }
    
    return os;
}

// OrderType

std::ostream& operator<<(std::ostream& os, const OrderType& orderType) {

    switch (orderType) {

        case OrderType::Limit:

            os << "LIMIT";
            break;

        case OrderType::Market:

            os << "MARKET";
            break;
    }

    return os;
}

// BookStatistics

std::ostream& operator<<(std::ostream& os, const BookStatistics& bookStatistics) {

    os << "-----------------------------" << "\n\n";

    os << "BOOK STATISTICS" << "\n\n";

    if (bookStatistics.bestBid) {
        os << "BEST BID: " << *bookStatistics.bestBid << "\n";
    } else {
        os << "BEST BID: N/A" << "\n";
    }

    if (bookStatistics.bestBid) {
        os << "BEST ASK: " << *bookStatistics.bestBid << "\n";
    } else {
        os << "BEST ASK: N/A" << "\n";
    }

    if (bookStatistics.bestBid) {
        os << "SPREAD: " << *bookStatistics.bestBid << "\n";
    } else {
        os << "SPREAD: N/A" << "\n";
    }

    os << "TOTAL BID QUANTITY: " << bookStatistics.totalBidQuantity << "\n";
    os << "TOTAL ASK QUANTITY: " << bookStatistics.totalAskQuantity << "\n\n";

    os << "TOTAL ACTIVE ORDERS: " << bookStatistics.activeBuyOrders << "\n";
    os << "TOTAL SELL ORDERS: " << bookStatistics.activeSellOrders << "\n\n";

    os << "TOTAL BID LEVELS: " << bookStatistics.bidLevelCount << "\n";
    os << "TOTAL ASK LEVELS: " << bookStatistics.askLevelCount << "\n\n";

    os << "-----------------------------" << "\n\n";

    return os;
}

// TradeStatistics

std::ostream& operator<<(std::ostream& os, const TradeStatistics& tradeStatistics) {

    os << "-----------------------------" << "\n\n";
    
    os << "TRADE STATISTICS" << "\n\n"; 
        
    os << "NUMBER OF TRADES: " << tradeStatistics.numberOfTrades << "\n";
    os << "TOTAL TRADE QUANTITY: " << tradeStatistics.totalQuantity << "\n\n";

    os << "AVERAGE TRADE PRICE: " << tradeStatistics.averagePrice << "\n";
    os << "VOLUME WEIGHTED AVERAGE PRICE: " << tradeStatistics.vwap << "\n\n";
        
    os << "-----------------------------" << "\n\n";

    return os;
};