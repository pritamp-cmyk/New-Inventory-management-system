#pragma once
#include <vector>

class IsubscriptionRepo {
public:
    virtual void subscribe(int prod_id, int user_id) = 0;
    virtual void unsubscribe(int prod_id, int user_id) = 0;
    virtual std::vector<int> find_subscribers(int prod_id) = 0;

    virtual ~IsubscriptionRepo() = default;
};
