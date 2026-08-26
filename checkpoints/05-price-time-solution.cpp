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
    void add(Order order) {
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

    const Order* best_buy() const {
        if (buys_.empty()) {
            return nullptr;
        }
        return &buys_.begin()->second.front();
    }

private:
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
    book.add({1, Side::Buy, dollars(100), 6});  // Bob arrives first
    book.add({2, Side::Buy, dollars(100), 4});  // John arrives second

    Order alice{3, Side::Sell, dollars(100), 8};
    const std::vector<Trade> trades = book.match_sell(alice);

    checks.expect("two trades are reported", trades.size() == 2);
    if (trades.size() == 2) {
        checks.expect("Bob trades first for all 6", trades[0].buyer_id == 1
                                                      && trades[0].quantity == 6);
        checks.expect("John trades second for 2", trades[1].buyer_id == 2
                                                    && trades[1].quantity == 2);
    }
    checks.expect("Alice's quantity is completely filled", alice.quantity == 0);

    const Order* john = book.best_buy();
    checks.expect("John remains at the front with 2", john != nullptr
                                                       && john->id == 2
                                                       && john->quantity == 2);

    return checks.finish();
}
