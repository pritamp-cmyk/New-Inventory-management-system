#pragma once
#include<pqxx/pqxx>
#include <string>
#include <memory>

class PostgresConnection {
private:
    static thread_local std::unique_ptr<pqxx::connection> threadConn;
    
public:
    static pqxx::connection& getConnection();
};
