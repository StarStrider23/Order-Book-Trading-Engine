#pragma once

#include "Order.h"

#include <iostream>
#include <map>
#include <deque>


class OrderBook {

    friend class Engine;

    private:

        std::map<double, std::deque<Order>, std::greater<double>> buyBook;
        std::map<double, std::deque<Order>> sellBook;

        // 1. Add Order To Book

        void addToBook(const Order& order);

        // 2. Print Book

        void printBook() const;
        
};