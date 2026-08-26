#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <string_view>
#include <vector>

using Price = int;
using Quantity = int;

constexpr Price dollars(int amount) {
    return amount * 100;
}

enum class Side {
    Buy,
    Sell,
};

struct Order {
    int id;
    Side side;
    Price price;
    Quantity quantity;
};

struct Trade {
    int buyer_id;
    int seller_id;
    Price price;
    Quantity quantity;
};

using PriceLevel = std::deque<Order>;
using BuyLevels = std::map<Price, PriceLevel, std::greater<Price>>;
using SellLevels = std::map<Price, PriceLevel>;

class OrderBook {
public:
    std::vector<Trade> submit(Order incoming) {
        std::vector<Trade> trades;
        if (incoming.side == Side::Buy) {
            trades = match_buy(incoming);
        } else {
            trades = match_sell(incoming);
        }

        if (incoming.quantity > 0) {
            add_resting(incoming);
        }
        return trades;
    }

    const Order* best_buy() const {
        if (buys_.empty()) {
            return nullptr;
        }
        return &buys_.begin()->second.front();
    }

    const Order* best_sell() const {
        if (sells_.empty()) {
            return nullptr;
        }
        return &sells_.begin()->second.front();
    }

private:
    void add_resting(Order order) {
        if (order.side == Side::Buy) {
            buys_[order.price].push_back(order);
        } else {
            sells_[order.price].push_back(order);
        }
    }

    std::vector<Trade> match_sell(Order& incoming_sell) {
        std::vector<Trade> trades;
        while (incoming_sell.quantity > 0 && !buys_.empty()
               && incoming_sell.price <= buys_.begin()->first) {
            Order& resting_buy = buys_.begin()->second.front();
            const Quantity traded = std::min(incoming_sell.quantity, resting_buy.quantity);
            incoming_sell.quantity -= traded;
            resting_buy.quantity -= traded;
            trades.push_back({resting_buy.id, incoming_sell.id, resting_buy.price, traded});

            if (resting_buy.quantity == 0) {
                buys_.begin()->second.pop_front();
                if (buys_.begin()->second.empty()) {
                    buys_.erase(buys_.begin());
                }
            }
        }
        return trades;
    }

    std::vector<Trade> match_buy(Order& incoming_buy) {
        std::vector<Trade> trades;
        while (incoming_buy.quantity > 0 && !sells_.empty()
               && incoming_buy.price >= sells_.begin()->first) {
            Order& resting_sell = sells_.begin()->second.front();
            const Quantity traded = std::min(incoming_buy.quantity, resting_sell.quantity);
            incoming_buy.quantity -= traded;
            resting_sell.quantity -= traded;
            trades.push_back({incoming_buy.id, resting_sell.id, resting_sell.price, traded});

            if (resting_sell.quantity == 0) {
                sells_.begin()->second.pop_front();
                if (sells_.begin()->second.empty()) {
                    sells_.erase(sells_.begin());
                }
            }
        }
        return trades;
    }

    BuyLevels buys_;
    SellLevels sells_;
};

class Checks {
public:
    void expect(std::string_view behaviour, bool happened) {
        std::cout << (happened ? "PASS: " : "FAIL: ") << behaviour << '\n';
        failures_ += happened ? 0 : 1;
    }

    int finish() const { return failures_ == 0 ? 0 : 1; }

private:
    int failures_ = 0;
};

int main() {
    Checks checks;
    OrderBook book;

    book.submit({1, Side::Buy, dollars(100), 6});  // Bob
    book.submit({2, Side::Buy, dollars(100), 4});  // John
    book.submit({3, Side::Buy, dollars(99), 3});   // David

    const std::vector<Trade> alice_trades =
        book.submit({4, Side::Sell, dollars(99), 8});
    checks.expect("Alice trades with Bob, then John", alice_trades.size() == 2
                                                           && alice_trades[0].buyer_id == 1
                                                           && alice_trades[0].quantity == 6
                                                           && alice_trades[1].buyer_id == 2
                                                           && alice_trades[1].quantity == 2);

    const Order* john = book.best_buy();
    checks.expect("John remains best with quantity 2", john != nullptr
                                                       && john->id == 2
                                                       && john->quantity == 2);

    const std::vector<Trade> charlie_trades =
        book.submit({5, Side::Sell, dollars(99), 5});
    checks.expect("Charlie sweeps two Buy prices", charlie_trades.size() == 2
                                                       && charlie_trades[0].price == dollars(100)
                                                       && charlie_trades[1].price == dollars(99));
    checks.expect("all resting Buys are filled", book.best_buy() == nullptr);

    book.submit({6, Side::Buy, dollars(103), 4});   // Emma rests
    const std::vector<Trade> frank_trades =
        book.submit({7, Side::Sell, dollars(102), 6});
    checks.expect("Frank trades 4 at Emma's resting $103", frank_trades.size() == 1
                                                               && frank_trades[0].price == dollars(103)
                                                               && frank_trades[0].quantity == 4);

    const Order* frank = book.best_sell();
    checks.expect("Frank's remaining Sell 2 rests at $102", frank != nullptr
                                                               && frank->id == 7
                                                               && frank->price == dollars(102)
                                                               && frank->quantity == 2);

    return checks.finish();
}
