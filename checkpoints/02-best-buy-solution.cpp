#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <string_view>

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

using PriceLevel = std::deque<Order>;
using BuyLevels = std::map<Price, PriceLevel, std::greater<Price>>;

class OrderBook {
public:
    void add_buy(Order order) {
        buys_[order.price].push_back(order);
    }

    const Order* best_buy() const {
        if (buys_.empty()) {
            return nullptr;
        }
        return &buys_.begin()->second.front();
    }

    std::size_t buy_price_count() const {
        return buys_.size();
    }

private:
    BuyLevels buys_;
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

    book.add_buy({1, Side::Buy, dollars(100), 6});  // Bob
    book.add_buy({2, Side::Buy, dollars(99), 3});   // David
    book.add_buy({3, Side::Buy, dollars(101), 2});  // Eve

    checks.expect("three Buy prices are stored", book.buy_price_count() == 3);

    const Order* best = book.best_buy();
    checks.expect("a best Buy exists", best != nullptr);
    if (best != nullptr) {
        checks.expect("the highest Buy price is best", best->price == dollars(101));
        checks.expect("Eve owns the best Buy", best->id == 3);
    }

    return checks.finish();
}
