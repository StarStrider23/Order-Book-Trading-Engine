#include "Order.h"

Order::Order(const std::string& id, Side side, double price, int quantity) :

    id(id),
    side(side),
    orderType(OrderType::Limit),
    price(validatePrice(price)),
    quantity(validateQuantity(quantity)),
    submittedAt(std::chrono::system_clock::now())

    {   
}

Order::Order(const std::string& id, Side side, int quantity) :

    id(id),
    side(side),
    orderType(OrderType::Market),
    quantity(validateQuantity(quantity)),
    submittedAt(std::chrono::system_clock::now())

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

TimePoint Order::getSubmittedAt() const {
    return submittedAt;
}

std::optional<TimePoint> Order::getAddedToBookAt() const {
    return addedToBookAt;
}

std::optional<TimePoint> Order::getModifiedAt() const {
    return modifiedAt;
}

std::optional<TimePoint> Order::getCancelledAt() const {
    return cancelledAt;
}

OrderInformation Order::getOrderInformation() const {

    OrderInformation info;

    info.id = id;

    info.orderType = orderType;
    info.side = side;
    info.quantity = quantity;
    info.price = price;

    info.submittedAt = submittedAt;
    info.addedToBookAt = addedToBookAt;
    info.modifiedAt = modifiedAt;
    info.cancelledAt = cancelledAt;

    return info;
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

void Order::setAddedToBookAt(TimePoint timestamp) {
    addedToBookAt = timestamp;
}

void Order::setModifiedAt(TimePoint timestamp) {
    modifiedAt = timestamp;
}

void Order::setCancelledAt(TimePoint timestamp) {
    cancelledAt = timestamp;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {

    auto timeSubmitted = std::chrono::system_clock::to_time_t(order.submittedAt);
    os << std::put_time(std::localtime(&timeSubmitted), "%H:%M:%S") << " | ";

    switch (order.orderType) {

        case OrderType::Limit:

            os << order.id << " | ";
            os << order.side << " | ";
            os << order.quantity << " @ ";
            os << order.price << "\n\n";
            break;

        case OrderType::Market:

            os << order.id << " | ";
            os << order.side << " | ";
            os << order.quantity << "\n\n";
            break;
    }

    return os;
}
