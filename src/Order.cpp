#include "Order.h"

Order::Order(const std::string& id, Side side, double price, int quantity) :

    id(id),
    side(side),
    orderType(OrderType::Limit),
    status(OrderStatus::Active),

    price(validatePrice(price)),
    averageExecution(std::nullopt),

    originalQuantity(validateQuantity(quantity)),
    filledQuantity(0),
    remainingQuantity(originalQuantity),

    submittedAt(std::chrono::system_clock::now())

    {   
}

Order::Order(const std::string& id, Side side, int quantity) :

    id(id),
    side(side),
    orderType(OrderType::Market),
    status(OrderStatus::Active),

    averageExecution(std::nullopt),

    originalQuantity(validateQuantity(quantity)),
    filledQuantity(0),
    remainingQuantity(originalQuantity),

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

std::optional<double> Order::getAverageExecution() const {

    if (!averageExecution) {

        return std::nullopt;
    
    } else {

        return averageExecution;
    }
}

int Order::getOriginalQuantity() const {
    return originalQuantity;
}

int Order::getFilledQuantity() const {
    return filledQuantity;
}

int Order::getRemainingQuantity() const {
    return remainingQuantity;
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
    info.status = status;
    info.side = side;

    info.price = price;
    info.averageExecution = averageExecution;

    info.originalQuantity = originalQuantity;
    info.filledQuantity = filledQuantity;
    info.remainingQuantity = remainingQuantity;

    info.submittedAt = submittedAt;
    info.addedToBookAt = addedToBookAt;
    info.modifiedAt = modifiedAt;
    info.cancelledAt = cancelledAt;

    return info;
}

void Order::setPrice(double newPrice) {
    this->price = newPrice;
}

void Order::setQuantity(int newQuantity) {
    this->originalQuantity = newQuantity;
}

void Order::reduceQuantity(int amount) {
    this->originalQuantity -= amount;
}

void Order::setRemainingQuantity(int newQuantity) {
    this->remainingQuantity = newQuantity;
} 

void Order::setAverageExecution(double newAverageExecution) {

    this->averageExecution = newAverageExecution;
}

void Order::setAddedToBookAt(TimePoint timestamp) {

    this->addedToBookAt = timestamp;
}

void Order::setModifiedAt(TimePoint timestamp) {
    this->modifiedAt = timestamp;
}

void Order::setCancelledAt(TimePoint timestamp) {
    this->cancelledAt = timestamp;
}

void Order::updateAverageExecution(double tradePrice, int tradeQuantity) {

    if (!averageExecution) {

        averageExecution = tradePrice;

    } else {

        double oldAverage = averageExecution.value();

        double newAverage =
            (oldAverage * filledQuantity
            + tradePrice * tradeQuantity)
            / (filledQuantity + tradeQuantity);

        averageExecution = newAverage;
    }

}

void Order::updateQuantity(int tradeQuantity) {

    filledQuantity += tradeQuantity;
    remainingQuantity -= tradeQuantity;

}

std::ostream& operator<<(std::ostream& os, const Order& order) {

    auto timeSubmitted = std::chrono::system_clock::to_time_t(order.submittedAt);
    os << std::put_time(std::localtime(&timeSubmitted), "%H:%M:%S") << " | ";

    switch (order.orderType) {

        case OrderType::Limit:

            os << order.id << " | ";
            os << order.side << " | ";
            os << order.remainingQuantity << " @ ";
            os << order.price.value() << "\n\n";
            break;

        case OrderType::Market:

            os << order.id << " | ";
            os << order.side << " | ";
            os << order.remainingQuantity << "\n\n";
            break;
    }

    return os;
}
