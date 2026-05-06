#include <iostream>
#include <string>
#include <vector>

enum class Side {
    Buy,
    Sell
};

struct Order {
    int id;
    Side side;
    long long price;
    long long quantity;
};

std::string sideToString(Side side) {
    if (side == Side::Buy) {
        return "Buy";
    }

    return "Sell";
}

bool isBuyOrder(const Order& order) {
    return order.side == Side::Buy;
}

bool isValidOrder(const Order& order) {
    return order.price > 0 && order.quantity > 0;
}

void printOrder(const Order& order) {
    std::cout << "Order ID: " << order.id << '\n';
    std::cout << "Side: " << sideToString(order.side) << '\n';
    std::cout << "Price: " << order.price << '\n';
    std::cout << "Quantity: " << order.quantity << '\n';
    std::cout << "Valid: " << (isValidOrder(order) ? "Yes" : "No") << '\n';
    std::cout << "-------------------" << '\n';
}

int main() {
    std::vector<Order> orders;

    orders.push_back(Order{1, Side::Buy, 100, 50});
    orders.push_back(Order{2, Side::Sell, 105, 20});
    orders.push_back(Order{3, Side::Buy, 101, 10});
    orders.push_back(Order{4, Side::Sell, 99, 30});

    for (const Order& order : orders) {
        printOrder(order);
    }

    return 0;
}