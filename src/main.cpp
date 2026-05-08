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

bool isFilled(const Order& order) {
    return order.quantity == 0;
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
    
    if (isFilled(order)) {
        std::cout << "Status: Filled" << '\n';
    } else if (isValidOrder(order)) {
        std::cout << "Status: Open" << '\n';
    } else {
        std::cout << "Status: Invalid" << '\n';
    }

    std::cout << "-------------------" << '\n';
}

void printTrade(const Trade& trade) {
    std::cout << "TRADE ";
    std::cout << "buy=" << trade.buyOrderId << " ";
    std::cout << "sell=" << trade.sellOrderId << " ";
    std::cout << "price=" << trade.price << " ";
    std::cout << "quantity=" << trade.quantity << " ";

}

void matchBuyOrder(Order& buyOrder, std::vector<Order>& sellOrders, std::vector<Trade>& trades) {
    for (Order& sellOrder : sellOrders) {
        if (isFilled(buyOrder)) {
            break;
        }

        if (isFilled(sellOrder)) {
            continue;
        }

        if (canMatch(buyOrder, sellOrder)) {
            Trade trade = createTrade(buyOrder, sellOrder);
            applyTrade(buyOrder, sellOrder, trade);
            trades.push_back(trade);
        }
    }
}

int main() {
    std::vector<Order> sellOrders;
    std::vector<Trade> trades;

    sellOrders.push_back(Order{1, Side::Sell, 100, 30});
    sellOrders.push_back(Order{2, Side::Sell, 100, 40});

    Order buyOrder{3, Side::Buy, 100, 50};

    std::cout << "Before matching:" << '\n';
    std::cout << "Incoming buy order:" << '\n';
    printOrder(buyOrder);

    std::cout << "Sell orders:" << '\n';
    for (const Order& sellOrder : sellOrders) {
        printOrder(sellOrder);
    }

    matchBuyOrder(buyOrder, sellOrders, trades);

    std::cout << '\n';
    std::cout << "Trades:" << '\n';
    for (const Trade& trade : trades) {
        printTrade(trade);
    }

    std::cout << '\n';
    std::cout << "After matching:" << '\n';
    std::cout << "Incoming buy order:" << '\n';
    printOrder(buyOrder);

    std::cout << "Sell orders:" << '\n';
    for (const Order& sellOrder : sellOrders) {
        printOrder(sellOrder);
    }

    return 0;
}