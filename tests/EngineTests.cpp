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

TEST(EngineTest, CorrectIdSidePriceAndQuantity) {

    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    EXPECT_EQ(order.getId(), "B1");

    EXPECT_EQ(order.getSide(), Side::Buy);

    EXPECT_EQ(order.getPrice(), 100);

    EXPECT_EQ(order.getQuantity(), 100);
}

TEST(EngineTest, MatchingOrdersProduceTrade) {
    
    Engine eng;

    eng.submitOrder(Side::Buy, 100, 100);
    eng.submitOrder(Side::Sell, 100, 100);

    EXPECT_TRUE(eng.emptyOrderBook());
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

TEST(PartialFillTest, PartialOrderRemains) {

    Engine eng;

    Order orderBuy = eng.submitOrder(Side::Buy, 100, 100);

    auto idBuy = orderBuy.getId();

    Order orderSell = eng.submitOrder(Side::Sell, 100, 40);

    EXPECT_FALSE(eng.emptyOrderBook());

    Order* orderPartial = eng.findOrderById(idBuy);

    auto quantityPartial = orderPartial->getQuantity();

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

    EXPECT_EQ(modified->getQuantity(), 105);
}

TEST(OrderModificationTest, PriceAndQuantityChange) {
    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrder(order.getId(), 105, 110);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getPrice(), 105);

    EXPECT_EQ(modified->getQuantity(), 110);
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

    EXPECT_EQ(firstRemaining->getQuantity(), 80);
    EXPECT_EQ(secondRemaining->getQuantity(), 50);
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

    EXPECT_EQ(firstRemaining->getQuantity(), 50);
    EXPECT_EQ(secondRemaining->getQuantity(), 0);
    EXPECT_EQ(thirdRemaining->getQuantity(), 50);

}