#include "PostgresConnection.h"

// Define thread_local storage for each thread
thread_local std::unique_ptr<pqxx::connection> PostgresConnection::threadConn = nullptr;

pqxx::connection& PostgresConnection::getConnection() {
    // Create connection per thread (thread-local storage)
    if (!threadConn) {
        threadConn = std::make_unique<pqxx::connection>(
            "host=localhost port=5432 dbname=inventory_db "
            "user=inventory_user password=inventory_pass"
        );
    }
    return *threadConn;
}
