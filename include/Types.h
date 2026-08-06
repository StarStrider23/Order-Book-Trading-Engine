#pragma once

#include <iostream>

enum class Side {
    Buy,
    Sell
};

std::ostream& operator<<(std::ostream& os, const Side& side);

struct OrderLocation {

    Side side;
    double price;
};