#include <pqxx/pqxx>
#include <vector>
#include <string>
using namespace std;
#include "PostgresConnection.h"
#include "../interfaces/IproductRepo.h"
#include "../../domain/product.h"

class ProductRepo : public IproductRepo{
    public:
    int create(string name,string description) override{
        pqxx::work txn(PostgresConnection::getConnection());

        pqxx:: result r = txn.exec_params(
            "INSERT INTO products (name, description) "
            "VALUES ($1, $2) RETURNING id",
            name,
            description
        );
        int productId = r[0][0].as<int>();
        txn.commit();
        return productId;
    }
    product find_by_id(int prod_id)override{
        pqxx::work txn(PostgresConnection::getConnection());

        pqxx::result r = txn.exec_params(
            "SELECT id, name, description "
            "FROM products WHERE id = $1",
            prod_id
        );

        if (r.empty()) {
            throw std::runtime_error("Product not found");
        }

        return product(
            r[0]["id"].as<int>(),
            r[0]["name"].as<std::string>(),
            r[0]["description"].as<std::string>(),
            0
        );
    }
    vector<product> find_by_name(string name)override{
        pqxx::work txn(PostgresConnection::getConnection());
        std::vector<product> products;

        pqxx::result r = txn.exec_params(
            "SELECT id, name, description "
            "FROM products WHERE name ILIKE '%' || $1 || '%'",
            name
        );

        for (const auto& row : r) {
            products.emplace_back(
                row["id"].as<int>(),
                row["name"].as<std::string>(),
                row["description"].as<std::string>()
            );
        }

        return products;
    }
    void update(int prod_id,string name, string description)override{
        pqxx::work txn(PostgresConnection::getConnection());

        txn.exec_params(
            "UPDATE products SET name=$1, description=$2 "
            "WHERE id=$3",
            name, description, prod_id
        );

        txn.commit();
    }
    void remove(int prod_id)override{
        pqxx::work txn(PostgresConnection::getConnection());

        txn.exec_params(
            "DELETE FROM products WHERE id = $1",
            prod_id
        );

        txn.commit();
    }
};