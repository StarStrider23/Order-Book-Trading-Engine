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

        void addToBook(const Order& order);

    public:

        void printBook() const;
        
};