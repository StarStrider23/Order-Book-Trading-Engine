#pragma once

#include <iostream>

// Side

enum class Side {
    Buy,
    Sell
};

std::ostream& operator<<(std::ostream& os, const Side& side);

// OrderType

enum class OrderType {
    Limit, 
    Market
};

std::ostream& operator<<(std::ostream& os, const OrderType& orderType);

// OrderLocation

struct OrderLocation {

    Side side;
    double price;
};

// BookStatistics

struct BookStatistics {

    std::optional<double> bestBid;
    std::optional<double> bestAsk;
    std::optional<double> spread;

    int totalBidQuantity;
    int totalAskQuantity;

    int activeBuyOrders;
    int activeSellOrders;
    int activeTotalOrders;

    int bidLevelCount;
    int askLevelCount;
};

std::ostream& operator<<(std::ostream& os, const BookStatistics& bookStatistics);

// TradeStatistics

struct TradeStatistics {

    std::size_t numberOfTrades;
    int totalQuantity;
    double averagePrice;
    double vwap;
};

std::ostream& operator<<(std::ostream& os, const TradeStatistics& bookStatistics);