#include <gtest/gtest.h>

#include "OrderBook.h"

/*

cmake --build build
cd build
ctest

*/

TEST(OrderBookTest, StartsEmpty) {

    OrderBook book;

    EXPECT_TRUE(book.empty());
}