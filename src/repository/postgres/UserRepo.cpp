#include <pqxx/pqxx>
#include <string>
using namespace std;
#include "PostgresConnection.h"

#include "../../domain/product.h"
#include "../../domain/user.h"
#include "../interfaces/IuserRepo.h"

class UserRepo : public IuserRepo {
    public:
    int create(string name, string email, string role) override {
        pqxx::work txn(PostgresConnection::getConnection());

        pqxx::result r = txn.exec_params(
            "INSERT INTO users (name, email, role) "
            "VALUES ($1, $2, $3) RETURNING id",
            name,
            email,
            role
        );
        int userId = r[0][0].as<int>();
        txn.commit();
        return userId;
    }
    user find_by_id(int user_id)override{
       pqxx::work txn(PostgresConnection::getConnection());

        pqxx:: result r = txn.exec_params(
            "SELECT id, name, email, role "
            "FROM users WHERE id = $1",
            user_id
        );
        if(r.empty()){
            throw std::runtime_error("User not found");
        }
        return user(
            r[0]["id"].as<int>(),
            r[0]["name"].as<string>(),
            r[0]["email"].as<string>(),
            r[0]["role"].as<string>()
        );

    }
    user find_by_email(string email)override{
        pqxx::work txn(PostgresConnection::getConnection());

        pqxx:: result r = txn.exec_params(
            "SELECT id, name, email, role "
            "FROM users WHERE email = $1",
            email
        );
        if(r.empty()){
            throw std::runtime_error("User not found");
        }
        return user(
            r[0]["id"].as<int>(),
            r[0]["name"].as<string>(),
            r[0]["email"].as<string>(),
            r[0]["role"].as<string>()
        );
    }
    
    };