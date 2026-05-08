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

struct Trade {
    int buyOrderId;
    int sellOrderId;
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

bool canMatch(const Order& buyOrder, const Order& sellOrder) {
    return buyOrder.price >= sellOrder.price;
}

Trade createTrade(const Order& buyOrder, const Order& sellOrder) {
    long long tradeQuantity = std::min(buyOrder.quantity, sellOrder.quantity);

    return Trade{
        buyOrder.id,
        sellOrder.id,
        sellOrder.price,
        tradeQuantity
    };
}

// Because we modify, we do not use const infront of the parameters
void applyTrade(Order& buyOrder, Order& sellOrder, const Trade& trade) {
    buyOrder.quantity -= trade.quantity;
    sellOrder.quantity -= trade.quantity;
}

void printOrder(const Order& order) {
    std::cout << "Order ID: " << order.id << '\n';
    std::cout << "Side: " << sideToString(order.side) << '\n';
    std::cout << "Price: " << order.price << '\n';
    std::cout << "Quantity: " << order.quantity << '\n';
    std::cout << "Valid: " << (isValidOrder(order) ? "Yes" : "No") << '\n';
    std::cout << "-------------------" << '\n';
}

void printTrade(const Trade& trade) {
    std::cout << "TRADE ";
    std::cout << "buy=" << trade.buyOrderId << " ";
    std::cout << "sell=" << trade.sellOrderId << " ";
    std::cout << "price=" << trade.price << " ";
    std::cout << "quantity=" << trade.quantity << " ";

}

int main() {
    std::vector<Order> orders;

    Order sellOrder(Order{1, Side::Sell, 100, 50});
    Order buyOrder{2, Side::Buy, 101, 80};
    // Order buyOrder(Order{2, Side::Buy, 101, 20});
    // Order buyOrder{2, Side::Buy, 99, 20};
    
    std::cout << "Before matching" << '\n';
    printOrder(sellOrder);
    printOrder(buyOrder);

    if (canMatch(buyOrder, sellOrder)) {
        Trade trade = createTrade(buyOrder, sellOrder);
        applyTrade(buyOrder, sellOrder, trade);

        std::cout << "Trade happened:" << '\n';
        printTrade(trade);
    } else {
        std::cout << "No trade happened." << '\n';
    }

    std::cout << '\n';
    std::cout << "After matching:" << '\n';
    printOrder(sellOrder);
    printOrder(buyOrder);

    return 0;
}