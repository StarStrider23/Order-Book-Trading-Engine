#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

using TimePoint = std::chrono::system_clock::time_point;

class Trade {

    friend class Engine;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);

    private:
        std::string tradeId;
        std::string buyOrderId;
        std::string sellOrderId;
        double tradePrice;
        int tradeQuantity;
        TimePoint tradedAt;

        Trade(const std::string& tradeId, 
                const std::string& buyOrderId, 
                const std::string& sellOrderId, 
                const double tradePrice, 
                const int tradeQuantity);

    public:

        std::string getTradeId() const;

        std::string getBuyOrderId() const;

        std::string getSellOrderId() const;

        double getPrice() const;

        int getQuantity() const;

        TimePoint getTradeTime() const;

};
