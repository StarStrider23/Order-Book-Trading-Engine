#include "Order.h"

Order::Order(const std::string& id, Side side, double price, int quantity) :

    id(id),
    side(side),
    price(validatePrice(price)),
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

double Order::getPrice() const {
    return price;
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
    os << order.id << " | " << order.side << " | " << order.quantity << " @ " << order.price << "\n\n";
    return os;
}