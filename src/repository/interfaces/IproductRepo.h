#pragma once
#include "domain/product.h"
#include<string>
#include<vector>
using namespace std;

class IproductRepo
{
public:
   virtual int create(string name,string description)=0;
   virtual product find_by_id(int prod_id)=0;
   virtual vector<product> find_by_name(string name)=0;
   virtual void update(int prod_id,string name,string description)=0;
   virtual void remove(int prod_id)=0;
   virtual ~IproductRepo()=default;
};