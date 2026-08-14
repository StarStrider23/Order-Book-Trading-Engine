# Order Book & Trading Engine

Project by Alexsey Chernichenko. August 2026.

## Overview

This project is a C++17 (and above) limit order book and matching engine designed to simulate the core functionality of an electronic trading system. The engine supports limit and market orders, order modification and cancellation, order matching, partial and full executions, trade history, execution prices as well as order information and modification history.

## Requirements 

The requirements are the following:

- C++17 or later
- CMake 3.20 or later
- Internet connection for the first CMake configuration, as GoogleTest is downloaded automatically

## Features

- Limit orders
- Market orders
- Order Search
- Order modification
- Order cancellation
- Buy and sell order matching
- Price-time priority
- Partial and full order execution
- Average execution price
- Order status tracking
- Order information/history
- Modification history
- Trade history
- Order book statistics
- Trade statistics
- Unit tests using GoogleTest
- 100 000 and 1 000 000 order performance benchmark

## Architecture

### Main Classes/structures

- `Order` - represents an individual limit/market buy or sell order. It stores the order's type, side, price, quantities, execution information and various timestamps. It also tracks its current execution state.

- `OrderBook` - maintains the active buy and sell orders. Orders are organized by price and preserved in time priority within each price level, allowing the engine to identify the best available order for matching.

- `Engine` — this is the central component of the trading system. It handles order submission, matching, modification, cancellation and also maintains order information, trade history and the order index.

- `Trade` — Represents an executed transaction between a buy and a sell order. It records the participating order IDs, execution price and quantity as well as trade timestamp.

- `OrderInformation` — stores information about an order independently of whether the order is still active in the order book. This includes its status, quantities, prices, timestamps and modification history.

- `OrderModification` — represents a single modification made to an order, recording the relevant changes to the order's attributes. Multiple modifications are stored as part of an order's modification history and are available through OrderInformation.

- `BookStatistics` - provides a snapshot of the current state of the order book. It summarises the best ask and bid orders, spread, number of active buy and sell orders as well as their available quantities.

- `Trade Statistics` - summarises trading activity, including the number of trades, total quantities traded, average and volume weighted average prices.

## Order Matching

The engine matches incoming orders against the best available orders on the opposite side of the order book. Orders are processed according to price-time priority, meaning the best price is considered first and orders at the same price are matched in the order they entered the book.

Limit orders are matched only when the available resting order satisfies the incoming order's price. Market orders are matched against available orders without any price constraints. In other words, a submitted market order matches any opposite order regardless of its price. 

Trades are executed at the resting order's price. In general, an order can be fully or partially filled after trade. However, only market orders can be partially filled because they never enter the order book. If an incoming market order has no match, it is cancelled. On the other hand, if a limit order has no match, it enters the order book and remains active. If a limit order is partially filled, it also stays active and remains in the order book with its updated remaining quantity. Fully filled orders are removed from the order book. Each execution updates the affected orders' filled and remaining quantity, average execution price and order information.

### Example

Suppose the order book contains two limit orders:

`15:46:37 | B1 | BUY | 110 @ 10`

`15:46:40 | B2 | BUY | 50 @ 99`

To clarify, let's take order with ID `B1`. It was added to the order book at `15:46:37`. It is a `BUY` order with price `100` and quantity `110` (buying 110 shares at/@ 100). Now, let's assume an incoming sell order.

`Order submitted: 15:47:00 | S1 | SELL | 150 @ 100`

The sell order can be immediately matched against the best buy order, which is order `B1`. This produces a trade:

```text
TRADE  
TRADE TIME: 15:47:00
TRADE ID: T1
BUY ORDER ID: B1
SELL ORDER ID: S1
PRICE: 100
QUANTITY: 110
```

As order `B1` is fully filled, it is removed from the order book. Order `S1` is now partially filled and the remaining quantity to sell is `40`. 

The next best buy order is at `99`, but the seller doesn't agree to sell unless the price is `100`. Therefore the remaining `40` shares will not execute and the order `S1` is added to the order book. The remaining sell order therefore stays unfilled rather than executing below its limit price.

The seller has now a few options - 1. to change the order price and match order `B2`; 2. to cancel the order; 3. to wait until another order is submitted.

## Usage

###

To initiate the engine, type:

```cpp
Engine engine;
```

### Submitting Orders

Limit orders are submitted by providing a side, price and quantity:

```cpp
Order buy = engine.submitOrder(Side::Buy, 100, 100);
```

Whereas market orders only require a side and a quantity:

```cpp
Order buy = engine.submitOrder(Side::Buy, 100);
```

Effectively, one doesn't need an indicator, but it is useful for procedures such as order search, modification, cancellation, etc as these require order ID. Order ID can be retrieved using the corresponding getter method:

```cpp
buy.getId();
```

This returns a string object.

### Order Search

An order can be retrieved by its ID:

```cpp
Order* order = engine.findOrderById(buy.getId());
```

This gives a pointer to the `Order` object called order. If one wants to print out explicitly the order and the information about the side, price and quantity, one should run:

```cpp
eng.displayOrderById(buy.getId());
```

### Order Modification


## Configure the project

From the project root in `bash`:

`cmake -S . -B build`

## Building

To build:

`cmake --build build`

This builds the following executables:

- `TradingApp` — example application demonstrating the trading engine
- `EngineTests` — GoogleTest test suite
- `EngineBenchmark` — performance benchmark test

## EngineTests

EngineTests contains GoogleTest for unit testing. The test suite verifies the core functionality of the trading engine, including order submission and validation, limit and market orders, order matching, partial and full executions, order status updates, execution prices, order modification and cancellation, timestamps and modification history, etc. There are 26 unity tests in total.

The tests are designed to verify individual components and typical trading scenarios. The tests are particularly helpful after introduction of changes of core functionalities or additions of new features. They ensure that changes to the engine do not introduce regressions and the core logic is executed as expected.

## Benchmark

The project also includes a benchmark for evaluating the engine's performance under a large order workload. It submits a huge number randomly generated orders with varying prices, quantities and buy/sell sides, and then measures the total execution time and resulting order throughput.

The benchmark is intended to provide an indication of the engine's performance. It should be run using a Release build for meaningful results and the section `Running` explains how to do it.

### Results

The benchmark was run using a Release build on:

- CPU: Apple M1
- Compiler: AppleClang 17.0.0
- Orders: 100 000 (100K) / 1000000 (1M)

Results:

```text
100K orders.
Elapsed: 0.0804477 seconds
Throughput: 1.24304e+06 orders/sec
```

```text
1M orders.
Elapsed: 1.12879 seconds
Throughput: 885903 orders/sec
```

## Running

To run the `TradingApp`, run `.build/TradingApp` from the project root directory.

For `GoogleTests`, run `ctest --test-dir build` from the project root directory.

For performance testing, first configure the project in Release mode:

`cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`

`cmake --build build`

and then run `./build/EngineBenchmark` from the project root directory.

## Future Improvements

As the engine is intended to be a simulation/educational project rather than a production trading system, it currently doesn't include lots of more advanced features. Possible feature improvements may include:

- Additional order types
- Improved order book data structures
- More extensive benchmarking
- Concurrency support
- More detailed performance profiling
- Additional matching strategies
