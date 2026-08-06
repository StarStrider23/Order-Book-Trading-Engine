#include <gtest/gtest.h>

#include "Engine.h"

/*

cmake --build build
cd build
ctest

*/

TEST(EngineTest, StartsEmpty) {

    Engine eng;

    EXPECT_TRUE(eng.empty());
}

TEST(EngineTest, AddBuyOrderMakesBookNonEmpty) {
    Engine eng;

    eng.submitOrder(Side::Buy, 100, 100);

    EXPECT_FALSE(eng.empty());
}