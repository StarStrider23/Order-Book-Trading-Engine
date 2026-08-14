#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

using TimePoint = std::chrono::system_clock::time_point;

class Trade {

    friend class Engine;

    public:

        // 1. Getter Methods

        std::string getTradeId() const;

        std::string getBuyOrderId() const;

        std::string getSellOrderId() const;

        double getPrice() const;

        int getQuantity() const;

        TimePoint getTradeTime() const;

    private:

        std::string tradeId;
        std::string buyOrderId;
        std::string sellOrderId;
        double tradePrice;
        int tradeQuantity;
        TimePoint tradedAt;

        // 1. Trade Constructor

        Trade(const std::string& tradeId, 
                const std::string& buyOrderId, 
                const std::string& sellOrderId, 
                const double tradePrice, 
                const int tradeQuantity);

    // 2. Operator Overload

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);

};
