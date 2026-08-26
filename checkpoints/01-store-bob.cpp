#include <iostream>
#include <string_view>
#include <vector>

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

class OrderBook {
public:
    void add(Order order) {
        // TODO: Store order at the back of orders_.
        (void)order;
    }

    std::size_t size() const {
        return orders_.size();
    }

    const Order& first() const {
        return orders_.front();
    }

private:
    std::vector<Order> orders_;
};

// Supplied behaviour tests start here. Ignore main() during the workshop.
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
    const Order bob{1, Side::Buy, 100, 6};

    book.add(bob);

    checks.expect("the book contains one order", book.size() == 1);

    if (book.size() == 1) {
        const Order& stored_bob = book.first();
        checks.expect("the stored order belongs to Bob", stored_bob.id == 1);
        checks.expect("Bob's Buy instruction is unchanged", stored_bob.side == Side::Buy);
        checks.expect(
            "Bob still wants 6 at price 100",
            stored_bob.price == 100 && stored_bob.quantity == 6);
    }

    return checks.finish();
}
