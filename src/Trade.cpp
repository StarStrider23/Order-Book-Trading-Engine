#include "Trade.h"

Trade::Trade(const std::string& tradeId, 
    const std::string& buyOrderId, 
    const std::string& sellOrderId, 
    double tradePrice, int tradeQuantity) :

    tradeId(tradeId),
    buyOrderId(buyOrderId),
    sellOrderId(sellOrderId),
    tradePrice(tradePrice),
    tradeQuantity(tradeQuantity),
    tradedAt(std::chrono::system_clock::now())
    {
}

std::string Trade::getTradeId() const {
    return tradeId;
}

std::string Trade::getBuyOrderId () const {
    return buyOrderId;
}

std::string Trade::getSellOrderId () const {
    return sellOrderId;
}

double Trade::getPrice() const {
    return tradePrice;
}

int Trade::getQuantity() const {
    return tradeQuantity;
}

TimePoint Trade::getTradeTime() const {
    return tradedAt;
}

std::ostream& operator<<(std::ostream& os, const Trade& trade) {

    auto time = std::chrono::system_clock::to_time_t(trade.getTradeTime());

    os << "TRADE" << "\n";
    os << "TRADE TIME: " << std::put_time(std::localtime(&time), "%H:%M:%S") << "\n";
    os << "TRADE ID: " << trade.getTradeId() << "\n";
    os << "BUY ORDER ID: " << trade.getBuyOrderId() << "\n";
    os << "SELL ORDER ID: " << trade.getSellOrderId() << "\n";
    os << "PRICE: " << trade.getPrice() << "\n";
    os << "QUANTITY: " << trade.getQuantity() << "\n\n";

    return os;
}
