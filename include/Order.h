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

    friend std::ostream& operator<<(std::ostream& os, const Order& order);

    private: 

        std::string id;
        Side side;
        OrderType orderType;
        double price;
        int quantity;
        TimePoint submittedAt;
        std::optional<TimePoint> addedToBookAt;
        std::optional<TimePoint> modifiedAt;
        std::optional<TimePoint> cancelledAt;

        Order(const std::string& id, Side side, double price, int quantity);

        Order(const std::string& id, Side side, int quantity);

        static double validatePrice(double price);

        static int validateQuantity(int quantity);

        void setPrice(double new_price);

        void setQuantity(int new_quantity);

        void reduceQuantity(int amount);

        void setAddedToBookAt(TimePoint timestamp);

        void setModifiedAt(TimePoint timstamp);

        void setCancelledAt(TimePoint timstamp);

    public:

        std::string getId() const;

        Side getSide() const;

        OrderType getOrderType() const;

        std::optional<double> getPrice() const;

        int getQuantity() const;

        TimePoint getSubmittedAt() const;

        std::optional<TimePoint> getAddedToBookAt() const;

        std::optional<TimePoint> getModifiedAt() const;

        std::optional<TimePoint> getCancelledAt() const;

        OrderInformation getOrderInformation() const;

};