# Rayo (C++20 Low-Latency Limit Order Book)

This project is a C++20 implementation of a limit order book and matching engine.

The first goal is correctness: the engine should correctly accept buy and sell orders, match compatible orders using price-time priority, generate trades, and maintain the remaining order book.

The second goal is performance: once the matching engine is correct, the project will add benchmarking, latency percentile reporting, preallocated memory pools, and a lock-free inbound queue.

## Current Stage

Stage 2: Sell-side price-time priority matching

The project currently supports matching one incoming buy order against a sell-side order book.

## Implemented So Far

- `Side` enum for buy/sell orders
- `Order` struct
- `Trade` struct
- Order validation
- Trade creation
- Quantity updates after trades
- Sell-side order book
- Price priority using `std::map`
- FIFO time priority using `std::deque`
- Matching one buy order against multiple sell orders
- Trade history stored in a `std::vector`

## Current Order Book Design

The sell-side book is represented as:

```cpp
using OrderQueue = std::deque<Order>;
using SellBook = std::map<long long, OrderQueue>;

## Planned Features

- Limit buy/sell orders
- Price-time priority matching
- Partial fills
- Cancel orders
- Unit tests
- Benchmark mode
- p50 / p99 / p99.9 latency reporting
- Preallocated order pool
- SPSC lock-free inbound queue