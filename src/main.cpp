#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <algorithm>
#include <functional>

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

// Buy book uses std::greater<long long> so prices are sorted in descending order.
// The first item is the highest buy price.
void addBuyOrder(
    std::map<long long, std::deque<Order>, std::greater<long long>>& buyBook,
    const Order& order
) {
    buyBook[order.price].push_back(order);
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

void printBuyBook(
    const std::map<long long, std::deque<Order>, std::greater<long long>>& buyBook
) {
    std::cout << "BUY BOOK:" << '\n';

    if (buyBook.empty()) {
        std::cout << "empty" << '\n';
        return;
    }

    for (const auto& [price, orders] : buyBook) {
        std::cout << "Price " << price << ":" << '\n';

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

void matchSellOrder(
    Order& sellOrder,
    std::map<long long, std::deque<Order>, std::greater<long long>>& buyBook,
    std::vector<Trade>& trades
) {
    while (!isFilled(sellOrder) && !buyBook.empty()) {
        auto bestBuyLevel = buyBook.begin();

        long long bestBuyPrice = bestBuyLevel->first;
        std::deque<Order>& ordersAtBestPrice = bestBuyLevel->second;

        if (sellOrder.price > bestBuyPrice) {
            break;
        }

        Order& buyOrder = ordersAtBestPrice.front();

        Trade trade = createTrade(buyOrder, sellOrder);
        applyTrade(buyOrder, sellOrder, trade);
        trades.push_back(trade);

        if (isFilled(buyOrder)) {
            ordersAtBestPrice.pop_front();
        }

        if (ordersAtBestPrice.empty()) {
            buyBook.erase(bestBuyLevel);
        }
    }
}

// Receives the order by copy without the pointer. Because submitOrder needs to modify the order's remaining quantity while matching.
void submitOrder(
    Order order,
    std::map<long long, std::deque<Order>, std::greater<long long>>& buyBook,
    std::map<long long, std::deque<Order>>& sellBook,
    std::vector<Trade>& trades
) {
    if (!isValidOrder(order)) {
        std::cout << "Rejected invalid order: id=" << order.id << '\n';
        return;
    }

    if (order.side == Side::Buy) {
        matchBuyOrder(order, sellBook, trades);

        if (!isFilled(order)) {
            addBuyOrder(buyBook, order);
        }
    } else {
        matchSellOrder(order, buyBook, trades);

        if (!isFilled(order)) {
            addSellOrder(sellBook, order);
        }
    }
}

int main() {
    std::map<long long, std::deque<Order>, std::greater<long long>> buyBook;
    std::map<long long, std::deque<Order>> sellBook;
    std::vector<Trade> trades;

    submitOrder(Order{1, Side::Buy, 99, 30}, buyBook, sellBook, trades);
    submitOrder(Order{2, Side::Buy, 98, 40}, buyBook, sellBook, trades);

    submitOrder(Order{3, Side::Sell, 105, 30}, buyBook, sellBook, trades);
    submitOrder(Order{4, Side::Sell, 106, 40}, buyBook, sellBook, trades);

    std::cout << "Initial stable book:" << '\n';
    printBuyBook(buyBook);
    std::cout << '\n';
    printSellBook(sellBook);

    std::cout << '\n';
    std::cout << "Submitting aggressive buy order:" << '\n';
    submitOrder(Order{5, Side::Buy, 106, 50}, buyBook, sellBook, trades);

    std::cout << '\n';
    std::cout << "Trades:" << '\n';
    for (const Trade& trade : trades) {
        printTrade(trade);
    }

    std::cout << '\n';
    std::cout << "Final book:" << '\n';
    printBuyBook(buyBook);
    std::cout << '\n';
    printSellBook(sellBook);

    return 0;
}