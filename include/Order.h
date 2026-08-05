#include <iostream>
#include <string>

#include "Types.h"

class Order {

    friend class OrderBook;
    friend class Engine;

    friend std::ostream& operator<<(std::ostream& os, const Order& order);

    private: 

        std::string id;
        Side side;
        double price;
        int quantity;

        Order(const std::string& id, Side side, double price, int quantity);

        static double validatePrice(double price);

        static int validateQuantity(int quantity);

        void setPrice(double new_price);

        void setQuantity(int new_quantity);

        void reduceQuantity(int amount);

    public:

        std::string getId() const;

        Side getSide() const;

        double getPrice() const;

        int getQuantity() const;


};