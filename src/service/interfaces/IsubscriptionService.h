#pragma once
#include"domain/subscription.h"
#include"domain/user.h"
#include<string>
#include<vector>
using namespace std;

class IsubscriptionService
{
public:
    virtual void subscribeUser(int prod_id,int user_id)=0;
    virtual void unsubscribeUser(int prod_id,int user_id)=0;
    virtual vector<int> getSubscribers(int prod_id)=0;
    virtual ~IsubscriptionService()=default;
};

