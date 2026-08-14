#include "Types.h"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <vector>

// ************************************************************
// TYPES
// ************************************************************

/*

1. Side
2. OrderType
3. OrderStatus
4. OrderLocation
5. OrderModification
6. OrderInformation
7. BookStatistics
8. TradeStatistics

*/

// ************************************************************
// 1. SIDE. OPERATOR
// ************************************************************

std::ostream& operator<<(std::ostream& os, const Side& side) {

    switch (side) {

        case Side::Buy:

            os << "BUY";
            break;

        case Side::Sell:

            os << "SELL";
            break;
    }
    
    return os;
}

// ************************************************************
// 2. ORDERTYPE. OPERATOR OVERLOAD
// ************************************************************

std::ostream& operator<<(std::ostream& os, const OrderType& orderType) {

    switch (orderType) {

        case OrderType::Limit:

            os << "LIMIT";
            break;

        case OrderType::Market:

            os << "MARKET";
            break;
    }

    return os;
}

// ************************************************************
// 3. ORDERSTATUS. OPERATOR OVERLOAD
// ************************************************************


std::ostream& operator<<(std::ostream& os, const OrderStatus& orderStatus) {

    switch (orderStatus) {

        case OrderStatus::Active:

            os << "ACTIVE";
            break;

        case OrderStatus::Filled:

            os << "FILLED";
            break;

        case OrderStatus::PartiallyFilled:

            os << "PARTIALLY FILLED";
            break;

        case OrderStatus::Cancelled:

            os << "CANCELLED";
            break;
    }

    return os;
}

// ************************************************************
// 4. ORDERLOCATION. 
// ************************************************************

/*

1. No methods

*/

// ************************************************************
// 5. ORDERMODIFICATION. ORDER OVERLOAD
// ************************************************************

std::ostream& operator<<(std::ostream& os, const OrderModification& orderModification) {

    auto timeModified = std::chrono::system_clock::to_time_t(orderModification.modifiedAt);

    os << std::put_time(std::localtime(&timeModified), "%H:%M:%S") << "\n";
    os << "PRICE: ";

    if (orderModification.oldPrice) {
        os << *orderModification.oldPrice;
    } else {
        os << "N/A";
    }

    os << " -> ";

    if (orderModification.newPrice) {
        os << *orderModification.newPrice << "\n";
    } else {
        os << "N/A" << "\n";
    }

    os << "QUANTITY: " << orderModification.oldQuantity << " -> " << orderModification.newQuantity << "\n";


    return os;
}

// ************************************************************
// 6. ORDERINFORMATION. PUBLIC METHODS
// ************************************************************

/*

1. Getter Methods

*/

// ============================================================
// 1. OrderInformation. Getter Methods
// ============================================================

const OrderType& OrderInformation::getOrderType() const {
    return orderType;
}

const OrderStatus& OrderInformation::getOrderStatus() const {
    return status;
}

const std::vector<OrderModification>& OrderInformation::getModificationHistory() const {
    return modificationHistory;
}


// ************************************************************
// 6. ORDERINFORMATION. PRIVATE METHODS
// ************************************************************

/*

1. Setter Methods
2. Add Modification To History

*/

// ============================================================
// 1. OrderInformation. Setter Methods
// ============================================================

void OrderInformation::setStatus(OrderStatus orderStatus) {

    status = orderStatus;
}

void OrderInformation::setPrice(double newPrice) {

    price = newPrice;
}

void OrderInformation::setAverageExecution(double newAverageExecution) {

    averageExecution = newAverageExecution;
}

void OrderInformation::setOriginalQuantity(int newQuantity) {

    originalQuantity = newQuantity;
}

void OrderInformation::setFilledQuantity(int newQuantity) {

    filledQuantity = newQuantity;
}

void OrderInformation::setRemainingQuantity(int newQuantity) {

    remainingQuantity = newQuantity;
}

void OrderInformation::setModifiedAt(TimePoint time) {

    modifiedAt = time;
}

// ============================================================
// 2. OrderInformation. Add Modification To History
// ============================================================

void OrderInformation::addModification(TimePoint modifiedAt, const int oldQuantity, 
    const int newQuantity, std::optional<double> oldPrice, std::optional<double> newPrice) {

    modificationHistory.push_back({modifiedAt, oldQuantity, 
        newQuantity, oldPrice, newPrice});
}

// ************************************************************
// 6. ORDERINFORMATION. ORDER OVERLOAD
// ************************************************************

std::ostream& operator<<(std::ostream& os, const OrderInformation& orderInformation) {

    os << "ORDER INFORMATION" << "\n\n";

    os << "ID: " << orderInformation.id << "\n";
    os << "-----------------------------" << "\n";
    os << "STATUS: " << orderInformation.status << "\n";
    os << "TYPE: " << orderInformation.orderType << "\n";
    os << "SIDE: " << orderInformation.side << "\n";
    os << "ORIGINAL QUANTITY: " << orderInformation.originalQuantity << "\n";
    os << "FILLED QUANTITY: " << orderInformation.filledQuantity << "\n";
    os << "REMAINING QUANTITY: " << orderInformation.remainingQuantity << "\n";

    if (orderInformation.price) {
        os << "PRICE: " << *orderInformation.price << "\n";
    } else {
        os << "PRICE: N/A" << "\n";
    }

    if (orderInformation.averageExecution) {
        os << "AVERAGE EXECUTION: " << *orderInformation.averageExecution << "\n";
    } else {
        os << "AVERAGE EXECUTION: N/A" << "\n";
    }

    auto timeSubmitted = std::chrono::system_clock::to_time_t(orderInformation.submittedAt);

    os << "SUBMITTED: " << std::put_time(std::localtime(&timeSubmitted), "%H:%M:%S") << "\n";

    os << "ADDED: ";

    if (orderInformation.addedToBookAt) {

        auto timeAddedToBook = std::chrono::system_clock::to_time_t(*orderInformation.addedToBookAt);

        os << std::put_time(std::localtime(&timeAddedToBook), "%H:%M:%S") << "\n";

    } else {

        os << "N/A" << "\n";
    }

    os << "MODIFIED: ";

    if (orderInformation.modifiedAt) {

        auto timeModified = std::chrono::system_clock::to_time_t(*orderInformation.modifiedAt);

        os << std::put_time(std::localtime(&timeModified), "%H:%M:%S") << "\n";

    } else {

        os << "N/A" << "\n";
    }

    os << "CANCELLED: ";

    if (orderInformation.cancelledAt) {

        auto timeCancelled = std::chrono::system_clock::to_time_t(*orderInformation.cancelledAt);

        os << std::put_time(std::localtime(&timeCancelled), "%H:%M:%S") << "\n\n";

    } else {

        os << "N/A" << "\n\n";
    }

    return os;
}

// ************************************************************
// 7. BOOKSTATISTICS. OPERATOR OVERLOAD
// ************************************************************

std::ostream& operator<<(std::ostream& os, const BookStatistics& bookStatistics) {

    os << "-----------------------------" << "\n\n";

    os << "BOOK STATISTICS" << "\n\n";

    if (bookStatistics.bestBid) {
        os << "BEST BID: " << *bookStatistics.bestBid << "\n";
    } else {
        os << "BEST BID: N/A" << "\n";
    }

    if (bookStatistics.bestBid) {
        os << "BEST ASK: " << *bookStatistics.bestBid << "\n";
    } else {
        os << "BEST ASK: N/A" << "\n";
    }

    if (bookStatistics.bestBid) {
        os << "SPREAD: " << *bookStatistics.bestBid << "\n";
    } else {
        os << "SPREAD: N/A" << "\n";
    }

    os << "TOTAL BID QUANTITY: " << bookStatistics.totalBidQuantity << "\n";
    os << "TOTAL ASK QUANTITY: " << bookStatistics.totalAskQuantity << "\n\n";

    os << "TOTAL ACTIVE ORDERS: " << bookStatistics.activeBuyOrders << "\n";
    os << "TOTAL SELL ORDERS: " << bookStatistics.activeSellOrders << "\n\n";

    os << "TOTAL BID LEVELS: " << bookStatistics.bidLevelCount << "\n";
    os << "TOTAL ASK LEVELS: " << bookStatistics.askLevelCount << "\n\n";

    os << "-----------------------------" << "\n\n";

    return os;
}

// ************************************************************
// 8. TRADESTATISTICS. OPERATOR OVERLOAD
// ************************************************************

std::ostream& operator<<(std::ostream& os, const TradeStatistics& tradeStatistics) {

    os << "-----------------------------" << "\n\n";
    
    os << "TRADE STATISTICS" << "\n\n"; 
        
    os << "NUMBER OF TRADES: " << tradeStatistics.numberOfTrades << "\n";
    os << "TOTAL TRADE QUANTITY: " << tradeStatistics.totalQuantity << "\n\n";

    os << "AVERAGE TRADE PRICE: " << tradeStatistics.averagePrice << "\n";
    os << "VOLUME WEIGHTED AVERAGE PRICE: " << tradeStatistics.vwap << "\n\n";
        
    os << "-----------------------------" << "\n\n";

    return os;
};