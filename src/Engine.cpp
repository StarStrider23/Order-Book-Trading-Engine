#include "Engine.h"

bool Engine::emptyOrderBook() const {

    return book.buyBook.empty() && book.sellBook.empty();
}

bool Engine::emptyTradeHistory() const {

    return tradeHistory.empty();
}

Order Engine::submitOrder(Side side, double price, int quantity) {

    std::string id;
    switch (side) {
        case Side::Buy:

            id = "B" + std::to_string(nextBuyId++);
            break;

        case Side::Sell:

            id = "S" + std::to_string(nextSellId++);
            break;
    }

    Order order(id, side, price, quantity);

    matchOrder(order, book);

        if (order.getQuantity() > 0) {

            book.addToBook(order);

            std::cout << "Order submitted: " << order;

            orderIndex[order.getId()] = {order.getSide(), order.getPrice()};
        }

    return order;

}

void Engine::printBook() const {
    book.printBook();
}

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

    auto oldPrice = order->getPrice();

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

    Order updatedOrder = *order;

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

    if (updatedOrder.getPrice() != newPrice) {

        removeOrder(id);

        updatedOrder.setQuantity(newQuantity);
        updatedOrder.setPrice(newPrice);

        matchOrder(updatedOrder, book);

        if (updatedOrder.getQuantity() > 0) {

            book.addToBook(updatedOrder);

            orderIndex[id] = {updatedOrder.getSide(), updatedOrder.getPrice()};
        }

    } else if (order->getQuantity() != newQuantity) {

        order->setQuantity(newQuantity);
        
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

void Engine::removeOrderFromDeque(std::deque<Order>& orders, const std::string& id) {

    for (auto it = orders.begin(); it != orders.end(); it++) {

        if (it->getId() == id) {

            orders.erase(it);
            return;
        }
    }
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

        removeOrderFromDeque(orders, id);

        if (orders.empty()) {
            book.buyBook.erase(location.price);
        }

    } else {

        auto& orders = book.sellBook[location.price];

        removeOrderFromDeque(orders, id);

        if (orders.empty()) {
            book.sellBook.erase(location.price);
        }

    }

}

void Engine::cancelOrder(const std::string& id) {

    removeOrder(id);

    std::cout << "Order with ID" << id << " canceled." << "\n\n";

}


const Trade* Engine::findTradeById(const std::string& tradeId) const {

    for (const Trade& trade : tradeHistory) {

        if (trade.getTradeId() == tradeId || trade.getTradeId() == tradeId) {
            
            return &trade;
        }
    }
    
    return nullptr;
}

void Engine::displayTradeById(const std::string& id) const {

    auto trade = findTradeById(id);

    if (trade) {
        std::cout << *trade;
    } else {
        std::cout << "Trade with ID " << id << " not found. \n\n";
    }
}

void Engine::matchOrder(Order& order, OrderBook& book) {

    switch (order.getSide()) {

        case Side::Sell:

            while (!book.buyBook.empty() && order.getQuantity() > 0) {

                auto it = book.buyBook.begin();
                Order& bestOrder = it->second.front();

                if (bestOrder.getPrice() < order.getPrice()) {
                    break;
                }

                int tradedQuantity = std::min(bestOrder.getQuantity(), order.getQuantity());
                
                Trade trade("T" + std::to_string(nextTradeId++),
                            bestOrder.getId(), 
                            order.getId(), 
                            bestOrder.getPrice(), 
                            tradedQuantity);

                tradeHistory.push_back(trade);

                std::cout << trade;
                
                order.reduceQuantity(tradedQuantity);
                bestOrder.reduceQuantity(tradedQuantity);

                if (bestOrder.getQuantity() == 0) {

                    it->second.pop_front();

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

                if (bestOrder.getPrice() > order.getPrice()) {
                    break;
                }

                int tradedQuantity = std::min(bestOrder.getQuantity(), order.getQuantity());

                Trade trade("T" + std::to_string(nextTradeId++),
                            order.getId(), 
                            bestOrder.getId(), 
                            bestOrder.getPrice(), 
                            tradedQuantity);

                tradeHistory.push_back(trade);

                std::cout << trade;

                order.reduceQuantity(tradedQuantity);
                bestOrder.reduceQuantity(tradedQuantity);

                if (bestOrder.getQuantity() == 0) {

                    it->second.pop_front();

                    if (it->second.empty()) {

                        book.sellBook.erase(it);
                    }
                }
            }

            break;
    }

}

void Engine::printTradeHistory() const {
    for (const Trade& trade : tradeHistory) {
        std::cout << trade;
    }
}