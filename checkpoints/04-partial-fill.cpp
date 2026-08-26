#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <string_view>

using Price = int;  // Whole-number ticks: 99, 100, 101.
using Quantity = int;

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
    int buyer_id = 0;
    int seller_id = 0;
    Price price = 0;
    Quantity quantity = 0;
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

    Trade match_one_sell(Order& incoming_sell) {
        // TODO: If this Sell crosses the best Buy, trade the smaller quantity.
        // The trade happens at the resting Buy's price.
        (void)incoming_sell;
        return {};
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

// Supplied behaviour tests start here. Ignore main() during the workshop.
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
    book.add({1, Side::Buy, 100, 6});  // resting Bob

    Order alice{2, Side::Sell, 99, 3};
    const Trade trade = book.match_one_sell(alice);

    checks.expect("Alice trades with Bob", trade.buyer_id == 1 && trade.seller_id == 2);
    checks.expect("the smaller quantity of 3 trades", trade.quantity == 3);
    checks.expect("the resting price 100 sets the trade", trade.price == 100);
    checks.expect("Alice is completely filled", alice.quantity == 0);

    const Order* bob = book.best_buy();
    checks.expect("Bob remains with quantity 3", bob != nullptr && bob->quantity == 3);

    Order charlie{3, Side::Sell, 101, 2};
    const Trade no_trade = book.match_one_sell(charlie);
    checks.expect("a Sell at 101 does not cross a Buy at 100", no_trade.quantity == 0
                                                            && charlie.quantity == 2);

    return checks.finish();
}
