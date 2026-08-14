#pragma once

#include "OrderBook.h"
#include "Trade.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>

class Engine {

        public:

        // 1. Order Submisstion

        Order submitOrder(Side side, double price, int quantity);

        Order submitOrder(Side side, int quantity);

        // 2. Order Search 

        Order* findOrderById(const std::string& id);

        const Order* findOrderById(const std::string& id) const;

        void displayOrderById(const std::string& id) const;

        // 3. Order Modification

        void changeOrder(const std::string& id, double newPrice, int newQuantity);

        void changeOrderPrice(const std::string& id, double newPrice);

        void changeOrderQuantity(const std::string& id, int newQuantity);

        // 4. Order Cancellation

        void cancelOrder(const std::string& id);

        // 5. Order Information

        void printOrderInformationById(const std::string& id) const;

        OrderInformation getOrderInformation(const std::string& id) const;

        // 6. Order Modification History

        void printOrderModificationHistoryById(const std::string& id) const;

        const std::vector<OrderModification>& getOrderModificationHistory(const std::string& id) const;

        // 7. Trade Search

        std::vector<Trade> findTradesByOrderId(const std::string& id) const;

        const Trade* findTradeByTradeId(const std::string& tradeId) const;
        
        void displayTradeById(const std::string& tradeId) const;

        void printTradeHistory() const;

        // 8. Book Statistics

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

        // 9. Trade Statistics

        int getTradeQuantity() const;

        std::size_t getNumberOfTrades() const;

        double getAveragePrice() const;

        double getVolumeWeightedAveragePrice() const;

        TradeStatistics getTradeStatistics() const;

        void printTradeStatistics() const;

        // 10. Utility Functions

        void printBook() const;

        bool emptyOrderBook() const;

        bool emptyTradeHistory() const;

    private:

        int nextBuyId = 1;
        int nextSellId = 1;

        int nextTradeId = 1;

        OrderBook book;

        std::unordered_map<std::string, OrderLocation> orderIndex;
        std::unordered_map<std::string, OrderInformation> orderInformation;
        std::vector<Trade> tradeHistory;

        // 1. ID Assignment

        std::string assignId(Side side);

        // 2. Order Search In Deque

        Order* findOrderInDeque(std::deque<Order>& orders, const std::string& id);

        const Order* findOrderInDeque(const std::deque<Order>& orders, const std::string& id) const;

        // 3. Order Modification
        
        std::optional<Order> modifyOrder(const std::string& id, double new_price, int new_quantity);

        std::optional<Order> modifyOrderPrice(const std::string& id, double newPrice);

        std::optional<Order> modifyOrderQuantity(const std::string& id, int newQuantity);

        // 4. Order removal

        std::optional<Order> removeOrderFromDeque(std::deque<Order>& orders, const std::string& id);

        void removeOrder(const std::string& id);

        // 5. Order Matching & Trade Execution

        void matchOrder(Order& order, OrderBook& book);

        void executeTrade(Order& incomingOrder, Order& restingOrder, int tradedQuantity);
};