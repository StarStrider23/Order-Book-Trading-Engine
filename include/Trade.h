#pragma once

#include <iostream>
#include <string>

class Trade {

    friend class Engine;

    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);

    private:
        std::string tradeId;
        std::string buyOrderId;
        std::string sellOrderId;
        double tradePrice;
        int tradeQuantity;

        Trade(const std::string& tradeId, 
                const std::string& buyOrderId, 
                const std::string& sellOrderId, 
                double tradePrice, int tradeQuantity);

    public:

        std::string getTradeId() const;

        std::string getBuyOrderId () const;

        std::string getSellOrderId () const;

        double getTradePrice() const;

        int getTradeQuantity() const;
};
