#pragma once

#include <iostream>
#include <chrono>
#include <vector>

using TimePoint = std::chrono::system_clock::time_point;

// 1. Side

enum class Side {

    Buy,
    Sell
};

std::ostream& operator<<(std::ostream& os, const Side& side);

// 2. OrderType

enum class OrderType {

    Limit, 
    Market
};

std::ostream& operator<<(std::ostream& os, const OrderType& orderType);

// 3. OrderLocation

struct OrderLocation {

    Side side;
    double price;
};

// 4. OrderStatus

enum class OrderStatus {

    Active,
    Filled,
    PartiallyFilled,
    Cancelled
};

// 5. Order Modification

struct OrderModification {

    TimePoint modifiedAt;

    int oldQuantity;
    int newQuantity;

    std::optional<double> oldPrice;
    std::optional<double> newPrice;
};

std::ostream& operator<<(std::ostream& os, const OrderModification& orderModification);

// 6. Order Information

struct OrderInformation {

    friend class Engine;

    friend std::ostream& operator<<(std::ostream& os, const OrderInformation& orderInforamtion);

    public:

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

        // 1. Getter Methods

        const OrderType& getOrderType() const;

        const OrderStatus& getOrderStatus() const;

        const std::vector<OrderModification>& getModificationHistory() const;

    private:

        // 1. Setter Methods

        void setStatus(OrderStatus orderStatus);

        void setPrice(double newPrice);

        void setAverageExecution(double newAverageExecution);

        void setOriginalQuantity(int newQuantity);

        void setFilledQuantity(int newQuantity);

        void setRemainingQuantity(int newQuantity);

        void setModifiedAt(TimePoint time);

        // 2. Add Modification To History

        void addModification(TimePoint modifiedAt, int oldQuantity, int newQuantity, 
            std::optional<double> oldPrice, std::optional<double> newPrice);

};

// 7. BookStatistics

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

// 8. TradeStatistics

struct TradeStatistics {

    std::size_t numberOfTrades;
    int totalQuantity;
    double averagePrice;
    double vwap;
};

std::ostream& operator<<(std::ostream& os, const TradeStatistics& bookStatistics);