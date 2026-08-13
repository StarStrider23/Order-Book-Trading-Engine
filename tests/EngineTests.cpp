#include <gtest/gtest.h>

#include "Engine.h"

/*

cmake --build build
cd build
ctest

*/

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

    ASSERT_EQ(eng.getOrderInformation(order.getId()).getOrderType(), OrderType::Limit);

    ASSERT_EQ(eng.getOrderInformation(order.getId()).getOrderStatus(), OrderStatus::Active);

    EXPECT_EQ(order.getId(), "B1");

    EXPECT_EQ(order.getSide(), Side::Buy);

    EXPECT_EQ(order.getPrice(), 100);
    EXPECT_EQ(order.getAverageExecution(), std::nullopt);

    EXPECT_EQ(order.getOriginalQuantity(), 100);
    EXPECT_EQ(order.getFilledQuantity(), 0);
    EXPECT_EQ(order.getRemainingQuantity(), order.getOriginalQuantity());

    ASSERT_GE(order.getSubmittedAt(), before);
    ASSERT_LE(order.getSubmittedAt(), after);

    ASSERT_TRUE(order.getAddedToBookAt().has_value());
    ASSERT_GE(*order.getAddedToBookAt(), order.getSubmittedAt());

    EXPECT_EQ(order.getModifiedAt(), std::nullopt);

    EXPECT_EQ(order.getCancelledAt(), std::nullopt);

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
}

TEST(OrderModificationTest, QuantityChange) {
    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderQuantity(order.getId(), 105);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getRemainingQuantity(), 105);
}

TEST(OrderModificationTest, PriceAndQuantityChange) {
    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrder(order.getId(), 105, 110);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getPrice(), 105);

    EXPECT_EQ(modified->getRemainingQuantity(), 110);
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

TEST(OrderCancellationTest, OrderCancellation) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.cancelOrder(order.getId());

    EXPECT_TRUE(eng.emptyOrderBook());

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

}