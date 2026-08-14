# Order Book & Trading Engine

Project by Alexsey Chernichenko. August 2026.

## Overview

This project is a C++17 (and above) limit order book and matching engine designed to simulate the core functionality of an electronic trading system. The engine supports limit and market orders, order modification and cancellation, order matching, partial and full executions, trade history, execution prices as well as order information and modification history.

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

To clarify, let's take order with ID B1. It was added to the order book at 15:46:37. It is a buy order with price 100 and quantity 110 (buying 110 shares at/@ 100). Now, let's assume an incoming sell order.

`Order submitted: 15:47:00 | S1 | SELL | 150 @ 100`

The sell order can be immediately matched against the best buy order, which is the B1 order. This produces a trade:

`TRADE. 
TRADE TIME: 15:47:00
TRADE ID: T1
BUY ORDER ID: B1
SELL ORDER ID: S1
PRICE: 100
QUANTITY: 110`

As the B1 order is fully filled, it is removed from the order book. The S1 order is now partially filled and the remaining quantity to sell is 40. 

The next best buy order is at 99.00, but the seller doesn't agree to sell unless the price is 100. Therefore the remaining 40 shares will not execute and the order S1 is added to the order book. The remaining sell order therefore stays unfilled rather than executing below its limit price.

The seller has now a few options - 1. to change the order price and match order B2; 2. to cancel the order; 3. to wait until another order is submitted.

## Building

## Running

## Testing

## Benchmark

## Design Notes

## Future Improvements

## License
