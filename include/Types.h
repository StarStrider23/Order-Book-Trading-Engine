#pragma once

#include <iostream>
#include <chrono>
#include <vector>

using TimePoint = std::chrono::system_clock::time_point;

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

// Order Status

enum class OrderStatus {

    Active,
    Filled,
    PartiallyFilled,
    Cancelled
};

// Order Modification History

struct OrderModification {

    TimePoint modifiedAt;

    int oldQuantity;
    int newQuantity;

    std::optional<double> oldPrice;
    std::optional<double> newPrice;
};

std::ostream& operator<<(std::ostream& os, const OrderModification& orderModification);

// Order Information

struct OrderInformation {

    friend class Engine;

    std::string id;
    OrderStatus status;
    OrderType orderType;
    Side side;

    int originalQuantity = 0;
    int filledQuantity = 0;
    int remainingQuantity = 0;

    std::optional<double> price;
    std::optional<double> averageExecution;
    
    TimePoint submittedAt;
    std::optional<TimePoint> addedToBookAt;
    std::optional<TimePoint> modifiedAt;
    std::optional<TimePoint> cancelledAt;

    std::vector<OrderModification> modificationHistory;

    const OrderType& getOrderType() const;

    const OrderStatus& getOrderStatus() const;

    const std::vector<OrderModification>& getModificationHistory() const;

    private:

        void setStatus(OrderStatus orderStatus);

        void setPrice(double newPrice);

        void setAverageExecution(double newAverageExecution);

        void setOriginalQuantity(int newQuantity);

        void setFilledQuantity(int newQuantity);

        void setRemainingQuantity(int newQuantity);

        void setModifiedAt(TimePoint time);

        void addModification(TimePoint modifiedAt, int oldQuantity, int newQuantity, 
            std::optional<double> oldPrice, std::optional<double> newPrice);

};

std::ostream& operator<<(std::ostream& os, const OrderInformation& orderInforamtion);

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