#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "../interfaces/IsubscriptionRepo.h"
#include "PostgresConnection.h"

class SubscriptionRepo : public IsubscriptionRepo {
public:
    void subscribe(int prod_id, int user_id) override {
        pqxx::work txn(PostgresConnection::getConnection());
        txn.exec_params(
            "INSERT INTO subscriptions (product_id, user_id) VALUES ($1, $2)",
            prod_id,
            user_id
        );
        txn.commit();
    }

    void unsubscribe(int prod_id, int user_id) override {
        pqxx::work txn(PostgresConnection::getConnection());
        txn.exec_params(
            "DELETE FROM subscriptions WHERE product_id = $1 AND user_id = $2",
            prod_id,
            user_id
        );
        txn.commit();
    }

    std::vector<int> find_subscribers(int prod_id) override {
        pqxx::work txn(PostgresConnection::getConnection());
        std::vector<int> subscribers;

        pqxx::result r = txn.exec_params(
            "SELECT user_id FROM subscriptions WHERE product_id = $1",
            prod_id
        );

        for (const auto& row : r) {
            subscribers.push_back(row["user_id"].as<int>());
        }

        return subscribers;
    }
};
