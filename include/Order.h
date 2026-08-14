#pragma once

#include "Types.h"

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

using TimePoint = std::chrono::system_clock::time_point;


class Order {

    friend class OrderBook;
    friend class Engine;

        public:

        // 1. Getter Methods

        std::string getId() const;

        Side getSide() const;

        OrderType getOrderType() const;

        std::optional<double> getPrice() const;

        std::optional<double> getAverageExecution() const;

        int getOriginalQuantity() const;

        int getFilledQuantity() const;

        int getRemainingQuantity() const;

        TimePoint getSubmittedAt() const;

        std::optional<TimePoint> getAddedToBookAt() const;

        std::optional<TimePoint> getModifiedAt() const;

        std::optional<TimePoint> getCancelledAt() const;

        OrderInformation getOrderInformation() const;


    private: 

        std::string id;
        Side side;
        OrderType orderType;
        OrderStatus status;

        std::optional<double> price;
        std::optional<double> averageExecution;

        int originalQuantity;
        int filledQuantity;
        int remainingQuantity;

        TimePoint submittedAt;
        std::optional<TimePoint> addedToBookAt;
        std::optional<TimePoint> modifiedAt;
        std::optional<TimePoint> cancelledAt;

        // 1. Order Constructors 

        Order(const std::string& id, Side side, double price, int quantity);

        Order(const std::string& id, Side side, int quantity);

        // 2. Order Validation

        static double validatePrice(double price);

        static int validateQuantity(int quantity);

        // 3. Setter Methods

        void setPrice(double newPrice);

        void setOriginalQuantity(int newQuantity);

        void setRemainingQuantity(int newQuantity);

        void setAverageExecution(double newAverageExecution);

        void setAddedToBookAt(TimePoint timestamp);

        void setModifiedAt(TimePoint timstamp);

        void setCancelledAt(TimePoint timstamp);

        void updateAverageExecution(double tradePrice, int tradeQuantity);

        void updateQuantity(int tradeQuantity);

    // 4. Operator Overload    

    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    
};