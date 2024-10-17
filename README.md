# Algorithmic Trader

This project implements an algorithmic trading system that uses efficient algorithms to place trading orders. It is divided into multiple phases, focusing on strategies such as "buy low, sell high," arbitrage detection, and processing complex order books.

## Project Structure

The project is structured across three phases:

- **Phase 1**: Basic trading strategies, such as buying stocks at a low price and selling them at a higher price, as well as detecting arbitrage opportunities.
- **Phase 2**: Simulation of a market that processes trade orders, with a focus on order expiration and priority-based matching.
- **Phase 3**: Statistical arbitrage, which utilizes multi-market data to identify inefficiencies and exploit arbitrage opportunities across different markets.

Each phase is located in its respective directory (`phase1/`, `phase2/`, `phase3/`), and only the relevant code files have been modified for implementation.

### Files

- **phase1/trader.cpp**: Implements trading strategies for placing orders and arbitrage detection.
- **phase2/market.cpp**: Simulates a market to process orders, matching buy and sell requests with priority.
- **phase3/trader.cpp**: Implements statistical arbitrage strategies across multiple markets.
