#pragma once

#include "OrderBook.h"
#include "Trade.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>

class Engine {

    private:

        int nextBuyId = 1;
        int nextSellId = 1;

        int nextTradeId = 1;

        OrderBook book;

        std::unordered_map<std::string, OrderLocation> orderIndex;

        std::vector<Trade> tradeHistory;

        std::string assignId(Side side);

        Order* findOrderInDeque(std::deque<Order>& orders, const std::string& id);

        const Order* findOrderInDeque(const std::deque<Order>& orders, const std::string& id) const;

        std::optional<Order> modifyOrder(const std::string& id, double new_price, int new_quantity);

        std::optional<Order> modifyOrderPrice(const std::string& id, double newPrice);

        std::optional<Order> modifyOrderQuantity(const std::string& id, int newQuantity);

        void removeOrderFromDeque(std::deque<Order>& orders, const std::string& id);

        void removeOrder(const std::string& id);

        void matchOrder(Order& order, OrderBook& book);

        const Trade* findTradeByTradeId(const std::string& tradeId) const;

    public:

        bool emptyOrderBook() const;

        // Active Order Statistics

        std::optional<double> getBestBid() const;

        std::optional<double> getBestAsk() const;

        std::optional<double> getSpread() const;

        int getTotalBidQuantity() const;
        
        int getTotalAskQuantity() const;

        int getActiveBuyOrderCount() const;

        int getActiveSellOrderCount() const;

        int getActiveOrderCount() const;

        int getBidLevelCount() const;

        int getAskLevelCount() const;

        BookStatistics getBookStatistics() const;

        void printBookStatistics() const;

        //

        bool emptyTradeHistory() const;

        // Trade Statistics

        int getTradeQuantity() const;

        std::size_t getNumberOfTrades() const;

        double getAveragePrice() const;

        double getVolumeWeightedAveragePrice() const;

        TradeStatistics getTradeStatistics() const;

        void printTradeStatistics() const;

        //

        Order submitOrder(Side side, double price, int quantity);

        Order submitOrder(Side side, int quantity);

        void printBook() const;

        const Order* findOrderById(const std::string& id) const;

        Order* findOrderById(const std::string& id);

        void displayOrderById(const std::string& id) const;

        std::vector<Trade> findTradesByOrderId(const std::string& id) const;

        void changeOrder(const std::string& id, double newPrice, int newQuantity);

        void changeOrderPrice(const std::string& id, double newPrice);

        void changeOrderQuantity(const std::string& id, int newQuantity);

        void cancelOrder(const std::string& id);

        void displayTradeById(const std::string& tradeId) const;

        void printTradeHistory() const;
};