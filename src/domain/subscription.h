#pragma once
#include<string>
using namespace std;

class subscription
{
private:
    int id;
    int user_id;
    int prod_id;
    bool active;
    time_t created_at;
public:
    subscription(int id,int user_id,int prod_id,bool active=1):id(id),user_id(user_id),prod_id(prod_id),active(active),created_at(time (nullptr)){}
    int get_sub_id(){
        return this->id;
    }
    int get_user_id(){
        return this->user_id;
    }
    int get_prod_id(){
        return this->prod_id;
    }
    bool is_active(){
        return this->active;
    }
    void activate(){
        this->active=true;
    }
    void deactivate(){
        this->active=false;
    }
    ~subscription();
};


