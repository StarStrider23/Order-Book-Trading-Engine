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

TEST(PartialFill, PartialOrderRemains) {

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

TEST(OrderModification, PriceChange) {
    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderPrice(order.getId(), 105);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getPrice(), 105);
}

TEST(OrderModification, QuantityChange) {
    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrderQuantity(order.getId(), 105);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getQuantity(), 105);
}

TEST(OrderModification, PriceAndQuantityChange) {
    Engine eng;

    Order order = eng.submitOrder(Side::Buy, 100, 100);

    eng.changeOrder(order.getId(), 105, 110);

    Order* modified = eng.findOrderById(order.getId());

    ASSERT_NE(modified, nullptr);

    EXPECT_EQ(modified->getPrice(), 105);

    EXPECT_EQ(modified->getQuantity(), 110);
}