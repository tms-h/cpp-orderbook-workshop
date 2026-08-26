#include <iostream>
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

class OrderBook {
public:
    void add(Order order) {
        orders_.push_back(order);
    }

    std::size_t size() const {
        return orders_.size();
    }

    const Order* find(int id) const {
        for (const Order& order : orders_) {
            if (order.id == id) {
                return &order;
            }
        }
        return nullptr;
    }

private:
    std::vector<Order> orders_;
};

class Checks {
public:
    void expect(std::string_view behaviour, bool happened) {
        std::cout << (happened ? "PASS: " : "FAIL: ") << behaviour << '\n';
        failures_ += happened ? 0 : 1;
    }

    int finish() const {
        return failures_ == 0 ? 0 : 1;
    }

private:
    int failures_ = 0;
};

int main() {
    Checks checks;
    OrderBook book;
    const Order bob{1, Side::Buy, dollars(100), 6};

    book.add(bob);

    checks.expect("the book contains one order", book.size() == 1);

    const Order* stored_bob = book.find(1);
    checks.expect("Bob can be found by order id", stored_bob != nullptr);
    if (stored_bob != nullptr) {
        checks.expect("Bob's Buy instruction is unchanged", stored_bob->side == Side::Buy);
        checks.expect("Bob still wants 6 at $100", stored_bob->price == dollars(100)
                                                  && stored_bob->quantity == 6);
    }

    return checks.finish();
}
