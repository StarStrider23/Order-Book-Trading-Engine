#include <iostream>
#include <unordered_map>
#include <vector>

#include "OrderBook.h"
#include "Trade.h"

class Engine {

    private:

        int nextBuyId = 1;
        int nextSellId = 1;

        int nextTradeId = 1;

        OrderBook book;

        std::unordered_map<std::string, OrderLocation> orderIndex;

        std::vector<Trade> tradeHistory;

        Order* findOrderInDeque(std::deque<Order>& orders, const std::string& id);

        const Order* findOrderInDeque(const std::deque<Order>& orders, const std::string& id) const;

        Order* findOrderById(const std::string& id);

        const Order* findOrderById(const std::string& id) const;

        void modifyOrder(const std::string& id, double new_price, int new_quantity);

        void removeOrderFromDeque(std::deque<Order>& orders, const std::string& id);

        void removeOrder(const std::string& id);

        void matchOrder(Order& order, OrderBook& book);

        const Trade* findTradeById(const std::string& tradeId) const;

    public:

        void submitOrder(Side side, double price, int quantity);

        void printBook() const;

        void displayOrderById(const std::string& id) const;

        void changeOrder(const std::string& id, double price, int quantity);

        void cancelOrder(const std::string& id);

        void displayTradeById(const std::string& tradeId) const;

};