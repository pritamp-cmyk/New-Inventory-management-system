#pragma once
#include<string>
#include<vector>
#include"domain/product.h"
using namespace std;

class IinventoryService{
    public:
    virtual int createProduct(string name,string description,int initial_stock)=0;
    virtual product getProduct(int prod_id)=0;
    virtual void updateProduct(int prod_id,string name,string description)=0;
    virtual void updateStock(int prod_id,int new_stock)=0;
    virtual void deleteProduct(int prod_id)=0;
    virtual vector<product> searchProduct(string name)=0;
    virtual ~IinventoryService()=default;
};