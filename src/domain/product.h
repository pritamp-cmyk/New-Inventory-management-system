#pragma once
#include <string>

class product {
private:
    int id;
    std::string name;
    std::string description;
    double price;

public:
    product(int id,
             const std::string& name,
             const std::string& description,
            double price)
        : id(id), name(name), description(description), price(price) {}

    product(int id,
             const std::string& name,
             const std::string& description)
        : id(id), name(name), description(description), price(0.0) {}

    int get_id() const { return id; }
    const std::string& get_name() const { return name; }
    const std::string& get_description() const { return description; }
    double get_price() const { return price; }
};
