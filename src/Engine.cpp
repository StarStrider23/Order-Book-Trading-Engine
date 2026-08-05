#include "Engine.h"

void Engine::submitOrder(Side side, double price, int quantity) {

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

            orderIndex[order.getId()] = {order.getSide(), order.getPrice()};
        }

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
        std::cout << "Order with ID " << id << " not found. \n";
    }
}

void Engine::modifyOrder(const std::string& id, double new_price, int new_quantity) {
    auto order = findOrderById(id);
    
    if (!order) {
        return;
    }

    if (order->getPrice() == new_price && order->getQuantity() == new_quantity) {
        return;
    }

    Order updatedOrder = *order;

    try {

        new_price = order->validatePrice(new_price);

    } catch (const std::invalid_argument& inv) {
        
        std::cout << inv.what() << "\n";
        return;
    
    }

    try {

        new_quantity = order->validateQuantity(new_quantity);

    } catch (const std::invalid_argument& inv) {

        std::cout << inv.what() << "\n";
        return;

    }  

    if (updatedOrder.getPrice() != new_price) {

        removeOrder(id);

        updatedOrder.setQuantity(new_quantity);
        updatedOrder.setPrice(new_price);

        orderIndex[id] = {updatedOrder.getSide(), updatedOrder.getPrice()};

        matchOrder(updatedOrder, book);

        if (updatedOrder.getQuantity() > 0) {

            book.addToBook(updatedOrder);
        }

    } else if (order->getQuantity() != new_quantity) {

        order->setQuantity(new_quantity);
        
    }
}

void Engine::changeOrder(const std::string& id, double price, int quantity) {

    modifyOrder(id, price, quantity);

    std::cout << "Order with ID " << id << " updated." << "\n\n";

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
        std::cout << "Order not found." << "\n";
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
        std::cout << "Trade with ID " << id << " not found. \n";
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
    }

}