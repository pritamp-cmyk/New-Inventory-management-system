#pragma once
#include<string>
#include "domain/inventory.h"
using namespace std;

class IinventoryRepo
{

public:
    virtual void create(int prod_id,int initialStock)=0;
    virtual inventory findProductBy_id(int prod_id)=0;
    virtual void update_stock(int prod_id,int new_stock)=0;
    virtual void removeProductBy_id(int prod_id)=0;
    virtual ~IinventoryRepo()=default;
};
