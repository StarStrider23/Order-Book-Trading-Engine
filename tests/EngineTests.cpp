#include "Engine.h"

#include <gtest/gtest.h>

TEST(EngineTest, StartsEmpty) {

    Engine eng;

    EXPECT_TRUE(eng.emptyOrderBook());
}

TEST(EngineTest, AddBuyOrderMakesBookNonEmpty) {
    
    Engine eng;

    eng.submitOrder(Side::Buy, 100, 100);

    EXPECT_FALSE(eng.emptyOrderBook());
}

TEST(EngineTest, CorrectLimitOrderParameters) {

    Engine eng;

    auto before = std::chrono::system_clock::now();

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    auto after = std::chrono::system_clock::now();

    Order* orderPtr = eng.findOrderById(order.getId());

    ASSERT_EQ(eng.getOrderInformation(order.getId()).getOrderType(), OrderType::Limit);

    ASSERT_EQ(eng.getOrderInformation(order.getId()).getOrderStatus(), OrderStatus::Active);

    EXPECT_EQ(orderPtr->getId(), "B1");

    EXPECT_EQ(orderPtr->getSide(), Side::Buy);

    EXPECT_EQ(orderPtr->getPrice(), 100);
    EXPECT_EQ(orderPtr->getAverageExecution(), std::nullopt);

    EXPECT_EQ(orderPtr->getOriginalQuantity(), 100);
    EXPECT_EQ(orderPtr->getFilledQuantity(), 0);
    EXPECT_EQ(orderPtr->getRemainingQuantity(), order.getOriginalQuantity());

    ASSERT_GE(orderPtr->getSubmittedAt(), before);
    ASSERT_LE(orderPtr->getSubmittedAt(), after);

    ASSERT_TRUE(orderPtr->getAddedToBookAt().has_value());
    ASSERT_GE(orderPtr->getAddedToBookAt(), order.getSubmittedAt());

    EXPECT_EQ(orderPtr->getModifiedAt(), std::nullopt);

    EXPECT_EQ(orderPtr->getCancelledAt(), std::nullopt);

}

TEST(EngineTest, CorrectMarketOrderParameters) {

    Engine eng;

    auto before = std::chrono::system_clock::now();

    Order order = eng.submitOrder(Side::Buy, 100);

    auto after = std::chrono::system_clock::now();

    ASSERT_TRUE(eng.emptyOrderBook());

    ASSERT_EQ(eng.getOrderInformation(order.getId()).getOrderType(), OrderType::Market);

    ASSERT_EQ(eng.getOrderInformation(order.getId()).getOrderStatus(), OrderStatus::Cancelled);

    EXPECT_EQ(order.getId(), "B1");

    EXPECT_EQ(order.getSide(), Side::Buy);

    EXPECT_EQ(order.getPrice(), std::nullopt);
    EXPECT_EQ(order.getAverageExecution(), std::nullopt);

    EXPECT_EQ(order.getOriginalQuantity(), 100);
    EXPECT_EQ(order.getFilledQuantity(), 0);
    EXPECT_EQ(order.getRemainingQuantity(), order.getOriginalQuantity());

    ASSERT_GE(order.getSubmittedAt(), before);
    ASSERT_LE(order.getSubmittedAt(), after);

    EXPECT_EQ(order.getAddedToBookAt(), std::nullopt);

    EXPECT_EQ(order.getModifiedAt(), std::nullopt);

    EXPECT_EQ(order.getCancelledAt(), std::nullopt);

}

TEST(OrderTest, InvalidLimitOrderParametersDontPopulateBook) {

    Engine eng;

    EXPECT_THROW(eng.submitOrder(Side::Buy, 0, -100), std::invalid_argument);

    EXPECT_TRUE(eng.emptyOrderBook());

}

TEST(OrderTest, InvalidMarketOrderParametersDontPopulateBook) {

    Engine eng;

    EXPECT_THROW(eng.submitOrder(Side::Buy, -100), std::invalid_argument);

    EXPECT_TRUE(eng.emptyOrderBook());
    
}

TEST(EngineTest, MatchingOrdersProduceTrade) {
    
    Engine eng;

    Order buy = eng.submitOrder(Side::Buy, 100, 100);
    Order sell = eng.submitOrder(Side::Sell, 100, 100);

    ASSERT_TRUE(eng.emptyOrderBook());

    ASSERT_EQ(eng.getOrderInformation(buy.getId()).getOrderStatus(), OrderStatus::Filled);
    ASSERT_EQ(eng.getOrderInformation(sell.getId()).getOrderStatus(), OrderStatus::Filled);
}

TEST(TradeTest, NotMatchingOrdersDontProduceTrade) {

    Engine eng;

    Order buy = eng.submitOrder(Side::Buy, 100, 100);
    Order sell = eng.submitOrder(Side::Sell, 150, 150);

    ASSERT_FALSE(eng.emptyOrderBook());

    ASSERT_EQ(eng.getOrderInformation(buy.getId()).getOrderStatus(), OrderStatus::Active);
    ASSERT_EQ(eng.getOrderInformation(sell.getId()).getOrderStatus(), OrderStatus::Active);

    EXPECT_EQ(buy.getOriginalQuantity(), 100);
    EXPECT_EQ(buy.getFilledQuantity(), 0);
    EXPECT_EQ(buy.getRemainingQuantity(), 100);

    EXPECT_EQ(sell.getOriginalQuantity(), 150);
    EXPECT_EQ(sell.getFilledQuantity(), 0);
    EXPECT_EQ(sell.getRemainingQuantity(), 150);

    EXPECT_EQ(buy.getAverageExecution(), std::nullopt);
    EXPECT_EQ(sell.getAverageExecution(), std::nullopt);

    EXPECT_TRUE(buy.getAddedToBookAt().has_value());
    EXPECT_TRUE(sell.getAddedToBookAt().has_value());
}

TEST(TradeTest, FilledMarketOrder) {

    Engine eng;

    Order sell = eng.submitOrder(Side::Sell, 100, 100);
    Order buy = eng.submitOrder(Side::Buy, 100);
    
    auto sellInfo = eng.getOrderInformation(sell.getId());
    auto buyInfo = eng.getOrderInformation(buy.getId());

    ASSERT_EQ(sellInfo.getOrderStatus(), OrderStatus::Filled);
    ASSERT_EQ(buyInfo.getOrderStatus(), OrderStatus::Filled);

    EXPECT_EQ(sellInfo.price, 100);
    EXPECT_EQ(buyInfo.price, std::nullopt);

    EXPECT_EQ(sellInfo.originalQuantity, 100);
    EXPECT_EQ(sellInfo.filledQuantity, 100);
    EXPECT_EQ(sellInfo.remainingQuantity, 0);

    EXPECT_EQ(buyInfo.originalQuantity, 100);
    EXPECT_EQ(buyInfo.filledQuantity, 100);
    EXPECT_EQ(buyInfo.remainingQuantity, 0);

    ASSERT_TRUE(sellInfo.averageExecution.has_value());
    ASSERT_TRUE(buyInfo.averageExecution.has_value());

    EXPECT_DOUBLE_EQ(*sellInfo.averageExecution, 100.0);
    EXPECT_DOUBLE_EQ(*buyInfo.averageExecution, 100.0);

}

TEST(TradeTest, PartialFillMarketOrder) {

    Engine eng;

    Order sell = eng.submitOrder(Side::Sell, 100, 100);
    Order buy = eng.submitOrder(Side::Buy, 150);
    
    auto sellInfo = eng.getOrderInformation(sell.getId());
    auto buyInfo = eng.getOrderInformation(buy.getId());

    ASSERT_EQ(sellInfo.getOrderStatus(), OrderStatus::Filled);
    ASSERT_EQ(buyInfo.getOrderStatus(), OrderStatus::PartiallyFilled);

    EXPECT_EQ(sellInfo.price, 100);
    EXPECT_EQ(buyInfo.price, std::nullopt);

    EXPECT_EQ(sellInfo.originalQuantity, 100);
    EXPECT_EQ(sellInfo.filledQuantity, 100);
    EXPECT_EQ(sellInfo.remainingQuantity, 0);

    EXPECT_EQ(buyInfo.originalQuantity, 150);
    EXPECT_EQ(buyInfo.filledQuantity, 100);
    EXPECT_EQ(buyInfo.remainingQuantity, 50);

    ASSERT_TRUE(sellInfo.averageExecution.has_value());
    ASSERT_TRUE(buyInfo.averageExecution.has_value());

    EXPECT_EQ(*sellInfo.averageExecution, 100);
    EXPECT_EQ(*buyInfo.averageExecution, 100);
    
}

TEST(TradeTest, TradePopulatesTradingHistory) {
    
    Engine eng;

    eng.submitOrder(Side::Buy, 100, 100);
    eng.submitOrder(Side::Sell, 100, 100);

    EXPECT_FALSE(eng.emptyTradeHistory());
}

TEST(TradeTest, RestingOrderPrice) {

    Engine eng;

    Order first = eng.submitOrder(Side::Buy, 105, 100);
    eng.submitOrder(Side::Sell, 100, 100);

    Order second = eng.submitOrder(Side::Sell, 100, 100);
    eng.submitOrder(Side::Buy, 105, 100);

    auto firstTrades = eng.findTradesByOrderId(first.getId());
    auto secondTrades = eng.findTradesByOrderId(second.getId());

    ASSERT_EQ(firstTrades.size(), 1);
    ASSERT_EQ(secondTrades.size(), 1);

    EXPECT_EQ(firstTrades[0].getPrice(), 105);
    EXPECT_EQ(secondTrades[0].getPrice(), 100);

}

TEST(TradeTest, AverageExecution) {

    Engine eng;

    eng.submitOrder(Side::Sell, 90, 70);
    eng.submitOrder(Side::Sell, 95, 30);

    Order buy = eng.submitOrder(Side::Buy, 100, 100);

    auto info = eng.getOrderInformation(buy.getId());

    EXPECT_DOUBLE_EQ(*info.averageExecution, 91.5);

}

TEST(PartialFillTest, PartialOrderRemains) {

    Engine eng;

    Order orderBuy = eng.submitOrder(Side::Buy, 100, 100);

    auto idBuy = orderBuy.getId();

    Order orderSell = eng.submitOrder(Side::Sell, 100, 40);

    EXPECT_FALSE(eng.emptyOrderBook());

    Order* orderPartial = eng.findOrderById(idBuy);

    auto quantityPartial = orderPartial->getRemainingQuantity();

    EXPECT_EQ(quantityPartial, 60);

    EXPECT_FALSE(eng.emptyTradeHistory());
}

TEST(OrderModificationTest, PriceChange) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderPrice(order.getId(), 105);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getPrice(), 105);

    auto info = eng.getOrderInformation(order.getId());

    EXPECT_FALSE(info.getModificationHistory().empty());

    EXPECT_EQ(info.getModificationHistory().size(), 1);
}

TEST(OrderModificationTest, QuantityChange) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderQuantity(order.getId(), 105);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getRemainingQuantity(), 105);

    auto info = eng.getOrderInformation(order.getId());

    EXPECT_FALSE(info.getModificationHistory().empty());
    
    EXPECT_EQ(info.getModificationHistory().size(), 1);
}

TEST(OrderModificationTest, PriceAndQuantityChange) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrder(order.getId(), 105, 110);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getPrice(), 105);

    EXPECT_EQ(modified->getRemainingQuantity(), 110);

    auto info = eng.getOrderInformation(order.getId());

    EXPECT_FALSE(info.getModificationHistory().empty());
    
    EXPECT_EQ(info.getModificationHistory().size(), 1);
}

TEST(OrderModificationTest, TwoChangesProduceTwoModifications) {
    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderPrice(order.getId(), 105);
    
    eng.changeOrderQuantity(order.getId(), 110);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getPrice(), 105);

    EXPECT_EQ(modified->getRemainingQuantity(), 110);

    auto info = eng.getOrderInformation(order.getId());

    EXPECT_FALSE(info.getModificationHistory().empty());
    
    EXPECT_EQ(info.getModificationHistory().size(), 2);
}

TEST(OrderModification, QuantityChangeLosesPriority)
{
    Engine eng;

    Order first = eng.submitOrder(Side::Buy, 100, 100);
    Order second = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderQuantity(first.getId(), 80);

    eng.submitOrder(Side::Sell, 100, 50);

    Order* firstRemaining = eng.findOrderById(first.getId());
    Order* secondRemaining = eng.findOrderById(second.getId());

    ASSERT_NE(firstRemaining, nullptr);
    ASSERT_NE(secondRemaining, nullptr);

    EXPECT_EQ(firstRemaining->getRemainingQuantity(), 80);
    EXPECT_EQ(secondRemaining->getRemainingQuantity(), 50);
}

TEST(OrderModification, OrderModificationSurvivesTrade) {

    Engine eng;

    Order buy = eng.submitOrder(Side::Buy, 100, 90);
    eng.changeOrderQuantity(buy.getId(), 100);

    Order sell = eng.submitOrder(Side::Sell, 100);

    auto infoBuy = eng.getOrderInformation(buy.getId());
    auto infoSell = eng.getOrderInformation(sell.getId());

    EXPECT_FALSE(infoBuy.getModificationHistory().empty());
    EXPECT_TRUE(infoSell.getModificationHistory().empty());
    
    EXPECT_EQ(infoBuy.getModificationHistory().size(), 1);
    EXPECT_EQ(infoSell.getModificationHistory().size(), 0);
    
}

TEST(OrderModification, SamePriceQuantityDontChangeModificationInfo) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 150);

    eng.changeOrder(order.getId(), 100, 150);

    eng.changeOrderPrice(order.getId(), 100);

    eng.changeOrderQuantity(order.getId(), 150);

    auto info = eng.getOrderInformation(order.getId());

    EXPECT_TRUE(info.getModificationHistory().empty());
    
    EXPECT_EQ(info.getModificationHistory().size(), 0);
}

TEST(OrderModification, InvalidModificationsDontChangeOrderModificationInfo) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderPrice(order.getId(), 0);

    eng.changeOrderQuantity(order.getId(), -10);

    eng.changeOrder(order.getId(), -100, 0);

    auto info = eng.getOrderInformation(order.getId());

    EXPECT_TRUE(info.getModificationHistory().empty());
    
    EXPECT_EQ(info.getModificationHistory().size(), 0);
}

TEST(OrderCancellation, OrderCancellation) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.cancelOrder(order.getId());

    auto info = eng.getOrderInformation(order.getId());

    ASSERT_TRUE(eng.emptyOrderBook());

    ASSERT_EQ(info.getOrderStatus(), OrderStatus::Cancelled);

}

TEST(OrderCancellation, OrderModificationSurvives) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrder(order.getId(), 110, 105);

    eng.cancelOrder(order.getId());

    auto info = eng.getOrderInformation(order.getId());

    ASSERT_TRUE(eng.emptyOrderBook());

    EXPECT_FALSE(info.getModificationHistory().empty());

    EXPECT_EQ(info.getModificationHistory().size(), 1);

}

TEST(FIFO, OlderOrderMatchesFirst)
{
    Engine eng;

    Order first = eng.submitOrder(Side::Buy, 100, 50);
    Order second = eng.submitOrder(Side::Buy, 100, 50);

    eng.submitOrder(Side::Sell, 100, 60);

    auto firstTrades = eng.findTradesByOrderId(first.getId());
    auto secondTrades = eng.findTradesByOrderId(second.getId());

    ASSERT_EQ(firstTrades.size(), 1);
    ASSERT_EQ(secondTrades.size(), 1);

    EXPECT_EQ(firstTrades[0].getQuantity(), 50);
    EXPECT_EQ(secondTrades[0].getQuantity(), 10);

    ASSERT_EQ(eng.getOrderInformation(first.getId()).getOrderStatus(), OrderStatus::Filled);
    ASSERT_EQ(eng.getOrderInformation(second.getId()).getOrderStatus(), OrderStatus::Active);
}

TEST(PriceMatching, BestPriceSelection) {

    Engine eng;

    Order first = eng.submitOrder(Side::Buy, 99, 50);
    Order second = eng.submitOrder(Side::Buy, 100, 50);
    Order third = eng.submitOrder(Side::Buy, 99, 50);

    Order* firstRemaining = eng.findOrderById(first.getId());
    Order* secondRemaining = eng.findOrderById(second.getId());
    Order* thirdRemaining = eng.findOrderById(third.getId());

    ASSERT_NE(firstRemaining, nullptr);
    ASSERT_NE(secondRemaining, nullptr);
    ASSERT_NE(thirdRemaining, nullptr);

    eng.submitOrder(Side::Sell, 100, 50);

    EXPECT_EQ(firstRemaining->getRemainingQuantity(), 50);
    EXPECT_EQ(secondRemaining->getRemainingQuantity(), 0);
    EXPECT_EQ(thirdRemaining->getRemainingQuantity(), 50);

    ASSERT_EQ(eng.getOrderInformation(first.getId()).getOrderStatus(), OrderStatus::Active);
    ASSERT_EQ(eng.getOrderInformation(second.getId()).getOrderStatus(), OrderStatus::Filled);
    ASSERT_EQ(eng.getOrderInformation(third.getId()).getOrderStatus(), OrderStatus::Active);
}