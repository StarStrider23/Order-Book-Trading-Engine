#include "Engine.h"

// Empty methods

bool Engine::emptyOrderBook() const {

    return book.buyBook.empty() && book.sellBook.empty();
}

bool Engine::emptyTradeHistory() const {

    return tradeHistory.empty();
}

// Book Statistics

std::optional<double> Engine::getBestBid() const {

    if (book.buyBook.empty()) {

        return std::nullopt;
    }

    return book.buyBook.begin()->first;
}

std::optional<double> Engine::getBestAsk() const {

    if (book.sellBook.empty()) {

        return std::nullopt;
    }

    return book.buyBook.begin()->first;
}

std::optional<double> Engine::getSpread() const {
    
    if (book.buyBook.empty() || book.sellBook.empty()) {

        return std::nullopt;
    }

    auto bestAsk = getBestAsk().value();
    auto bestBid = getBestBid().value();

    return bestAsk - bestBid;
}

int Engine::getTotalBidQuantity() const {

    int totalBidQuantity = 0;

    for (const auto& [price, orders] : book.buyBook) {

        for (const Order& order : orders) {

            totalBidQuantity += order.getQuantity();
        }
    }
    return totalBidQuantity;
};

int Engine::getTotalAskQuantity() const {
        
    int totalAskQuantity = 0;

    for (const auto& [price, orders] : book.sellBook) {

        for (const Order& order : orders) {

            totalAskQuantity += order.getQuantity();
        }
    }
    return totalAskQuantity;
};

int Engine::getActiveBuyOrderCount() const {

    int totalBuyOrders = 0;

    for (const auto& [price, orders] : book.buyBook) {

        totalBuyOrders += orders.size();
    }

    return totalBuyOrders;
}

int Engine::getActiveSellOrderCount() const {

    int totalSellOrders = 0;

    for (const auto& [price, orders] : book.sellBook) {
        
        totalSellOrders += orders.size();
    }

    return totalSellOrders;

}

int Engine::getActiveOrderCount() const {

    return getActiveBuyOrderCount() + getActiveSellOrderCount();
}

int Engine::getBidLevelCount() const {

    return book.buyBook.size();
}

int Engine::getAskLevelCount() const {

    return book.sellBook.size();

}

BookStatistics Engine::getBookStatistics() const {

    BookStatistics stats;

    stats.bestBid = getBestBid();
    stats.bestAsk = getBestAsk();
    stats.spread = getSpread();

    stats.totalBidQuantity = getTotalBidQuantity();
    stats.totalAskQuantity = getTotalAskQuantity();

    stats.activeBuyOrders = getActiveBuyOrderCount();
    stats.activeSellOrders = getActiveSellOrderCount();
    stats.activeTotalOrders = getActiveOrderCount();

    stats.bidLevelCount = getBidLevelCount();
    stats.askLevelCount = getAskLevelCount();

    return stats;
}

void Engine::printBookStatistics() const {

    std::cout << getBookStatistics();

}

// Trade Statistics

int Engine::getTradeQuantity() const {

    int totalTradeQuantity = 0;

    for (const Trade& trade : tradeHistory) {

        totalTradeQuantity += trade.getQuantity();
    }

    return totalTradeQuantity;

}

std::size_t Engine::getNumberOfTrades() const {

    return tradeHistory.size();
}

double Engine::getAveragePrice() const {

    double totalTradePrice = 0;

    if (tradeHistory.empty()) {
        return totalTradePrice;
    }

    for (const Trade& trade : tradeHistory) {

        totalTradePrice += trade.getPrice();
    }

    return totalTradePrice / tradeHistory.size();

}

double Engine::getVolumeWeightedAveragePrice() const {

    double priceVolume = 0;
    int totalQuantity = 0;

    if (tradeHistory.empty()) {
        return priceVolume;
    }

    for (const Trade& trade : tradeHistory) {

        priceVolume += trade.getPrice() * trade.getQuantity();
        totalQuantity += trade.getQuantity();
    }

    return priceVolume / totalQuantity;
}

TradeStatistics Engine::getTradeStatistics() const {

    TradeStatistics stats;

    stats.numberOfTrades = getNumberOfTrades();
    stats.totalQuantity = getTradeQuantity();
    stats.averagePrice = getAveragePrice();
    stats.vwap = getVolumeWeightedAveragePrice();

    return stats;
}

void Engine::printTradeStatistics() const {

    std::cout << getTradeStatistics();
}

// Order Submission (Limit & Market)

Order Engine::submitOrder(Side side, double price, int quantity) {

    std::string id = assignId(side);

    Order order(id, side, price, quantity);

    orderInformation[order.getId()] = order.getOrderInformation();

    std::cout << "Order submitted: " << order;

    matchOrder(order, book);

    if (order.getQuantity() > 0) {

        order.setAddedToBookAt(std::chrono::system_clock::now());

        book.addToBook(order);

        orderIndex[order.getId()] = {order.getSide(), order.getPrice().value()};

        orderInformation[order.getId()] = order.getOrderInformation();
        orderInformation[order.getId()].setStatus(OrderStatus::Active);
    } else {

        orderInformation[order.getId()].setStatus(OrderStatus::Filled);
    }

    return order;

}

Order Engine::submitOrder(Side side, int quantity) {

    std::string id = assignId(side);

    Order order(id, side, quantity);

    orderInformation[order.getId()] = order.getOrderInformation();

    orderInformation[order.getId()].setStatus(OrderStatus::Active);

    std::cout << "Order submitted: " << order;

    int initialQuantity = order.getQuantity();

    matchOrder(order, book);

    if (order.getQuantity() == initialQuantity) {

        orderInformation[order.getId()].setStatus(OrderStatus::Cancelled);
    } else if (order.getQuantity() == 0) {

        orderInformation[order.getId()].setStatus(OrderStatus::Filled);
    } else {

        orderInformation[order.getId()].setStatus(OrderStatus::PartiallyFilled);
    }

    return order;
}

// Assigning ID

std::string Engine::assignId(Side side) {

        std::string id;

        switch (side) {
        case Side::Buy:

            id = "B" + std::to_string(nextBuyId++);
            break;

        case Side::Sell:

            id = "S" + std::to_string(nextSellId++);
            break;
    }

    return id;
}

// Print Order Book

void Engine::printBook() const {
    book.printBook();
}

// Find Order By ID

Order* Engine::findOrderInDeque(std::deque<Order>& orders, const std::string& id) {

    for (Order& order : orders) {

        if (order.getId() == id) {

            return &order;
        }

    }

    return nullptr;

}

const Order* Engine::findOrderInDeque(const std::deque<Order>& orders, const std::string& id) const {

    for (const Order& order : orders) {

        if (order.getId() == id) {

            return &order;
        }

    } 

    return nullptr;

}

Order* Engine::findOrderById(const std::string& id) {

    auto index = orderIndex.find(id);

    if (index == orderIndex.end()) {
        return nullptr;
    }

    const OrderLocation& location = index->second;

    if (location.side == Side::Buy) {
        auto priceIt = book.buyBook.find(location.price);

        if (priceIt == book.buyBook.end()) {
            return nullptr;
        }

        return findOrderInDeque(priceIt->second, id);
    } else {
        auto priceIt = book.sellBook.find(location.price);

        if (priceIt == book.sellBook.end()) {
            return nullptr;
        }

        return findOrderInDeque(priceIt->second, id);
    }

}

const Order* Engine::findOrderById(const std::string& id) const {

    auto index = orderIndex.find(id);

    if (index == orderIndex.end()) {
        return nullptr;
    }

    const OrderLocation& location = index->second;

    if (location.side == Side::Buy) {
        auto priceIt = book.buyBook.find(location.price);

        if (priceIt == book.buyBook.end()) {
            return nullptr;
        }

        return findOrderInDeque(priceIt->second, id);
    } else {
        auto priceIt = book.sellBook.find(location.price);

        if (priceIt == book.sellBook.end()) {
            return nullptr;
        }

        return findOrderInDeque(priceIt->second, id);
    }

}

void Engine::displayOrderById(const std::string& id) const {

    auto order = findOrderById(id);

    if (order) {
        std::cout << *order;
    } else {
        std::cout << "Order with ID " << id << " not found. \n\n";
    }
}

// Order Modification

std::optional<Order> Engine::modifyOrderPrice(const std::string& id, double newPrice) {

    auto order = findOrderById(id);
    
    if (!order) {
        return std::nullopt;
    }

    auto oldQuantity = order->getQuantity();
    
    return modifyOrder(id, newPrice, oldQuantity);
}

std::optional<Order> Engine::modifyOrderQuantity(const std::string& id, int newQuantity) {

    auto order = findOrderById(id);

    if (!order) {
        return std::nullopt;
    }

    auto oldPrice = order->getPrice().value();

    return modifyOrder(id, oldPrice, newQuantity);;
}

std::optional<Order> Engine::modifyOrder(const std::string& id, double newPrice, int newQuantity) {

    Order* order = findOrderById(id);
    
    if (!order) {
        return std::nullopt;
    }

    if (order->getPrice() == newPrice && order->getQuantity() == newQuantity) {
        return *order;
    }



    try {

        newPrice = order->validatePrice(newPrice);

    } catch (const std::invalid_argument& inv) {
        
        std::cout << inv.what() << "\n\n";
        return *order;
    
    }

    try {

        newQuantity = order->validateQuantity(newQuantity);

    } catch (const std::invalid_argument& inv) {

        std::cout << inv.what() << "\n\n";
        return *order;

    } 
    
    Order updatedOrder = *order;

    updatedOrder.setModifiedAt(std::chrono::system_clock::now());

    if (updatedOrder.getPrice().value() != newPrice) {

        removeOrder(id);

        updatedOrder.setQuantity(newQuantity);
        updatedOrder.setPrice(newPrice);

        matchOrder(updatedOrder, book);

        if (updatedOrder.getQuantity() > 0) {

            book.addToBook(updatedOrder);

            orderIndex[id] = {updatedOrder.getSide(), updatedOrder.getPrice().value()};
        }

    } else if (updatedOrder.getQuantity() != newQuantity) {

        removeOrder(id);

        updatedOrder.setQuantity(newQuantity);

        book.addToBook(updatedOrder);
        
    }

    return updatedOrder;
}

void Engine::changeOrderQuantity(const std::string& id, int newQuantity) {

    std::optional<Order> order = modifyOrderQuantity(id, newQuantity);

    if (order) {
        std::cout << "Order with ID " << id << " updated." << "\n\n";
    } else {
        std::cout << "Order with ID " << id << " not found." << "\n\n";
    }
}

void Engine::changeOrderPrice(const std::string& id, double newPrice) {

    std::optional<Order> order = modifyOrderPrice(id, newPrice);

    if (order) {
        std::cout << "Order with ID " << id << " updated." << "\n\n";
    } else {
        std::cout << "Order with ID " << id << " not found." << "\n\n";
    }
}

void Engine::changeOrder(const std::string& id, double price, int quantity) {

    std::optional<Order> order = modifyOrder(id, price, quantity);

    if (order) {
        std::cout << "Order with ID " << id << " updated." << "\n\n";
    } else {
        std::cout << "Order with ID " << id << " not found." << "\n\n";
    }


}

// Order Removal & Cancellation

std::optional<Order> Engine::removeOrderFromDeque(std::deque<Order>& orders, const std::string& id) {

    for (auto it = orders.begin(); it != orders.end(); it++) {

        if (it->getId() == id) {

            Order removedOrder = *it;
            orders.erase(it);
            return removedOrder;
        }
    }

    return std::nullopt;
}

void Engine::removeOrder(const std::string& id) {

    auto index = orderIndex.find(id);

    if (index == orderIndex.end()) {
        std::cout << "Order not found." << "\n\n";
        return;
    }

    OrderLocation location = index->second;

    if (location.side == Side::Buy) {

        auto& orders = book.buyBook[location.price];

        std::optional<Order> cancelledOrder = removeOrderFromDeque(orders, id);

        if (cancelledOrder) {

            cancelledOrder->setCancelledAt(std::chrono::system_clock::now());

            orderInformation[cancelledOrder->getId()].setStatus(OrderStatus::Cancelled);
        }

        if (orders.empty()) {
            book.buyBook.erase(location.price);
        }

    } else {

        auto& orders = book.sellBook[location.price];

        std::optional<Order> cancelledOrder = removeOrderFromDeque(orders, id);

        if (cancelledOrder) {

            cancelledOrder->setCancelledAt(std::chrono::system_clock::now());

            orderInformation[cancelledOrder->getId()].setStatus(OrderStatus::Cancelled);
        }

        if (orders.empty()) {
            book.sellBook.erase(location.price);
        }

    }

}

void Engine::cancelOrder(const std::string& id) {

    removeOrder(id);

    std::cout << "Order with ID" << id << " canceled." << "\n\n";

}

// Order Information

void Engine::printOrderInformationById(const std::string& id) const {

    auto it = orderInformation.find(id);

    if (it != orderInformation.end()) {

        std::cout << it->second;
    } else {

        std::cout << "Order with ID " << id << " not found" << "\n\n";
    }

} 

OrderInformation Engine::getOrderInformation(const std::string& id) const {

    return orderInformation.at(id);
}

// Trade

const Trade* Engine::findTradeByTradeId(const std::string& tradeId) const {

    for (const Trade& trade : tradeHistory) {

        if (trade.getTradeId() == tradeId || trade.getTradeId() == tradeId) {
            
            return &trade;
        }
    }
    
    return nullptr;
}

std::vector<Trade> Engine::findTradesByOrderId(const std::string& id) const
{
    std::vector<Trade> result;

    for (const Trade& trade : tradeHistory) {
        if (trade.getBuyOrderId() == id || trade.getSellOrderId() == id) {
            result.push_back(trade);
        }
    }

    return result;
}

void Engine::displayTradeById(const std::string& id) const {

    auto trade = findTradeByTradeId(id);

    if (trade) {
        std::cout << *trade;
    } else {
        std::cout << "Trade with ID " << id << " not found. \n\n";
    }
}

void Engine::printTradeHistory() const {
    for (const Trade& trade : tradeHistory) {
        std::cout << trade;
    }
}

// Order Matching

void Engine::matchOrder(Order& order, OrderBook& book) {

    switch (order.getSide()) {

        case Side::Sell:

            while (!book.buyBook.empty() && order.getQuantity() > 0) {

                auto it = book.buyBook.begin();
                Order& bestOrder = it->second.front();

                if (order.getOrderType() == OrderType::Limit && 
                    bestOrder.getPrice().value() < order.getPrice().value()) {
                    
                    break;
                }

                int tradedQuantity = std::min(bestOrder.getQuantity(), order.getQuantity());
                
                Trade trade("T" + std::to_string(nextTradeId++),
                            bestOrder.getId(), 
                            order.getId(), 
                            bestOrder.getPrice().value(), 
                            tradedQuantity);

                tradeHistory.push_back(trade);

                std::cout << trade;
                
                order.reduceQuantity(tradedQuantity);
                bestOrder.reduceQuantity(tradedQuantity);

                if (bestOrder.getQuantity() == 0) {

                    it->second.pop_front();

                    orderInformation[bestOrder.getId()].setStatus(OrderStatus::Filled);

                    if (it->second.empty()) {

                        book.buyBook.erase(it);
                    }
                }   
            }

            break;
        
        case Side::Buy:

            while (!book.sellBook.empty() && order.getQuantity() > 0) {

                auto it = book.sellBook.begin();
                Order& bestOrder = it->second.front();

                if (order.getOrderType() == OrderType::Limit && 
                    bestOrder.getPrice().value() > order.getPrice().value()) {

                    break;
                }

                int tradedQuantity = std::min(bestOrder.getQuantity(), order.getQuantity());

                Trade trade("T" + std::to_string(nextTradeId++),
                            order.getId(), 
                            bestOrder.getId(), 
                            bestOrder.getPrice().value(), 
                            tradedQuantity);

                tradeHistory.push_back(trade);

                std::cout << trade;

                order.reduceQuantity(tradedQuantity);
                bestOrder.reduceQuantity(tradedQuantity);

                if (bestOrder.getQuantity() == 0) {

                    it->second.pop_front();

                    if (it->second.empty()) {

                        orderInformation[bestOrder.getId()].setStatus(OrderStatus::Filled);

                        book.sellBook.erase(it);
                    }
                }
            }

            break;
    }

}