#include <cstdlib>
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
        // TODO: store the order in orders_.
    }

    std::size_t size() const {
        return orders_.size();
    }

private:
    std::vector<Order> orders_;
};

void check_equal(std::string_view name, std::size_t actual, std::size_t expected) {
    if (actual != expected) {
        std::cerr << "FAIL: " << name << " (expected " << expected
                  << ", got " << actual << ")\n";
        std::exit(1);
    }

    std::cout << "PASS: " << name << '\n';
}

int main() {
    OrderBook book;
    const Order bob{1, Side::Buy, dollars(100), 6};

    book.add(bob);

    check_equal("Bob is stored in the book", book.size(), 1);
}
