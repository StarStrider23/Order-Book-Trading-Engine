#include "Trade.h"

Trade::Trade(const std::string& tradeId, 
    const std::string& buyOrderId, 
    const std::string& sellOrderId, 
    double tradePrice, int tradeQuantity) :

    tradeId(tradeId),
    buyOrderId(buyOrderId),
    sellOrderId(sellOrderId),
    tradePrice(tradePrice),
    tradeQuantity(tradeQuantity)
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

double Trade::getTradePrice() const {
    return tradePrice;
}

int Trade::getTradeQuantity() const {
    return tradeQuantity;
}

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
    os << "TRADE" << "\n"
        << "TRADE ID: " << trade.getTradeId() << "\n"
        << "BUY ORDER ID: " << trade.getBuyOrderId() << "\n"
        << "SELL ORDER ID: " << trade.getSellOrderId() << "\n"
        << "PRICE: " << trade.getTradePrice() << "\n"
        << "QUANTITY: " << trade.getTradeQuantity() << "\n\n";
    return os;
}
