#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <algorithm>

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
    std::cout << "quantity=" << trade.quantity << '\n';

}


//Map with the key as the price and the value is the queue that FIFOs the incoming orders
//std::map keeps its keys sorted in ascending order. Thus, the first item is the lowest price.
void addSellOrder(std::map<long long, std::deque<Order>>& sellBook, const Order& order) {
    sellBook[order.price].push_back(order);
}

void printSellBook(const std::map<long long, std::deque<Order>>& sellBook) {
    std::cout << "SELL BOOK:" << '\n';

    if (sellBook.empty()) {
        std::cout << "empty" << '\n';
        return;
    }

    for (const auto& [price, orders] : sellBook) {
        std::cout << "Price" << price << ":" << '\n';

        for (const Order& order : orders) {
            std::cout << "  id=" << order.id
                      << " qty=" << order.quantity
                      << '\n';
        }
    }
}

void matchBuyOrder(
    Order& buyOrder,
    std::map<long long, std::deque<Order>>& sellBook,
    std::vector<Trade>& trades
) {
    while (!isFilled(buyOrder) && !sellBook.empty()) {
        auto bestSellLevel = sellBook.begin();

        long long bestSellPrice = bestSellLevel->first;
        std::deque<Order>& ordersAtBestPrice = bestSellLevel->second;

        if (buyOrder.price < bestSellPrice) {
            break;
        }

        Order& sellOrder = ordersAtBestPrice.front();

        Trade trade = createTrade(buyOrder, sellOrder);
        applyTrade(buyOrder, sellOrder, trade);
        trades.push_back(trade);

        if (isFilled(sellOrder)) {
            ordersAtBestPrice.pop_front();
        }

        if (ordersAtBestPrice.empty()) {
            sellBook.erase(bestSellLevel);
        }
    }
}

int main() {
    std::map<long long, std::deque<Order>> sellBook;
    std::vector<Trade> trades;

    addSellOrder(sellBook, Order{1, Side::Sell, 105, 30});
    addSellOrder(sellBook, Order{2, Side::Sell, 100, 40});
    addSellOrder(sellBook, Order{3, Side::Sell, 100, 20});
    addSellOrder(sellBook, Order{4, Side::Sell, 101, 50});

    Order buyOrder{5, Side::Buy, 101, 70};

    std::cout << "Before matching:" << '\n';
    printSellBook(sellBook);

    std::cout << '\n';
    std::cout << "Incoming buy order:" << '\n';
    printOrder(buyOrder);

    matchBuyOrder(buyOrder, sellBook, trades);

    std::cout << '\n';
    std::cout << "Trades:" << '\n';
    for (const Trade& trade : trades) {
        printTrade(trade);
    }

    std::cout << '\n';
    std::cout << "After matching:" << '\n';
    std::cout << "Incoming buy order:" << '\n';
    printOrder(buyOrder);

    printSellBook(sellBook);

    return 0;
}