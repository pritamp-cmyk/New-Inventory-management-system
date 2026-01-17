#pragma once
#include<pqxx/pqxx>
#include <string>

class PostgresConnection {
public:
    static pqxx::connection& getConnection() {
        static pqxx::connection conn(
            "host=localhost port=5432 dbname=inventory_db "
            "user=inventory_user password=inventory_pass"
        );
        return conn;
    }
};
