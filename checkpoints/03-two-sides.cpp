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

using PriceLevel = std::deque<Order>;
using BuyLevels = std::map<Price, PriceLevel, std::greater<Price>>;
using SellLevels = std::map<Price, PriceLevel>;

class OrderBook {
public:
    void add(Order order) {
        if (order.side == Side::Buy) {
            buys_[order.price].push_back(order);
            return;
        }

        // TODO: Put a Sell at the back of its Sell price level.
    }

    const Order* best_buy() const {
        if (buys_.empty()) {
            return nullptr;
        }
        return &buys_.begin()->second.front();
    }

    const Order* best_sell() const {
        // TODO: Return the oldest order at the first (lowest) Sell price.
        return nullptr;
    }

    std::size_t sell_price_count() const {
        return sells_.size();
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

    book.add({1, Side::Buy, 100, 6});   // Bob
    book.add({2, Side::Buy, 99, 3});    // David
    book.add({3, Side::Sell, 103, 4});  // Alice
    book.add({4, Side::Sell, 102, 2});  // Charlie

    const Order* buy = book.best_buy();
    const Order* sell = book.best_sell();

    checks.expect("two Sell prices are stored", book.sell_price_count() == 2);
    checks.expect("Bob's price 100 order is the best Buy", buy != nullptr
                                                        && buy->id == 1
                                                        && buy->price == 100);
    checks.expect("Charlie's price 102 order is the best Sell", sell != nullptr
                                                            && sell->id == 4
                                                            && sell->price == 102);
    checks.expect("the spread is 2 ticks", buy != nullptr && sell != nullptr
                                           && sell->price - buy->price == 2);

    return checks.finish();
}
