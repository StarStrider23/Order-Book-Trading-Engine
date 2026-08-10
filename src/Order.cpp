#include "Order.h"

Order::Order(const std::string& id, Side side, double price, int quantity) :

    id(id),
    side(side),
    orderType(OrderType::Limit),
    price(validatePrice(price)),
    quantity(validateQuantity(quantity))

    {   
}

Order::Order(const std::string& id, Side side, int quantity) :

    id(id),
    side(side),
    orderType(OrderType::Market),
    quantity(validateQuantity(quantity))

    {   
}


double Order::validatePrice(double price) {

    if (price <= 0) {

        throw std::invalid_argument("Order price must be positive.");
    }
    return price;
}

int Order::validateQuantity(int quantity) {

    if (quantity <= 0) {

        throw std::invalid_argument("Order quantity must be positive.");
    }
    return quantity;
}

std::string Order::getId() const {
    return id;
}

Side Order::getSide() const {
    return side;
}
OrderType Order::getOrderType() const {
    return orderType;
}

std::optional<double> Order::getPrice() const {
    
    if (!price) {

        return std::nullopt;

    } else {

        return price;
    }
}

int Order::getQuantity() const {
    return quantity;
}

void Order::setPrice(double new_price) {
    this->price = new_price;
}

void Order::setQuantity(int new_quantity) {
    this->quantity = new_quantity;
}

void Order::reduceQuantity(int amount) {
    this->quantity -= amount;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {

    switch (order.orderType) {

        case OrderType::Limit:

            os << order.orderType << " | " << order.id << " | " << order.side << " | " << order.quantity << " @ " << order.price << "\n\n";
            break;
        
        case OrderType::Market:

            os << order.orderType << " | " << order.id << " | " << order.side << " | " << order.quantity << "\n\n";
            break;

    }
    
    return os;
}
