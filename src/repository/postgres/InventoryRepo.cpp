#include<pqxx/pqxx>
#include <vector>
#include <string>
using namespace std;

#include "PostgresConnection.h"
#include "../interfaces/IinventoryRepo.h"
#include "../../domain/inventory.h"
class InventoryRepo : public IinventoryRepo {
    public:
    void create(int prod_id,int initialStock) override {
        pqxx::work txn(PostgresConnection::getConnection());

        txn.exec_params(
            "INSERT INTO inventory (product_id, stock) "
            "VALUES ($1, $2)",
            prod_id,
            initialStock
        );
        txn.commit();
    }
    inventory findProductBy_id(int prod_id)override{
        pqxx::work txn(PostgresConnection::getConnection());

        pqxx::result r = txn.exec_params(
            "SELECT product_id, stock "
            "FROM inventory WHERE product_id = $1",
            prod_id
        );

        if (r.empty()) {
            throw std::runtime_error("Inventory not found");
        }

        return inventory(
            r[0]["product_id"].as<int>(),
            r[0]["stock"].as<int>()
        );
    }
    void update_stock(int prod_id,int new_stock)override{
        pqxx::work txn(PostgresConnection::getConnection());

        txn.exec_params(
            "UPDATE inventory SET stock = $1, updated_at = NOW() "
            "WHERE product_id = $2",
            new_stock,
            prod_id
        );
        txn.commit();
    }
    void removeProductBy_id(int prod_id)override{
        pqxx::work txn(PostgresConnection::getConnection());

        txn.exec_params(
            "DELETE FROM inventory WHERE product_id = $1",
            prod_id
        );
        txn.commit();
    }
};