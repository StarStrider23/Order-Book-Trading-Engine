#include <iostream>

#include "Types.h"

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