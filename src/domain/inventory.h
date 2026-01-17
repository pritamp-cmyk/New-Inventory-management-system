#pragma once
#include<string>
#include<iostream>
using namespace std;

class inventory{
private:
int prod_id;
int stock;
time_t updated_at;
public:
inventory(int prod_id,int stock){
    this->prod_id=prod_id;
    this->stock=stock;
}
int get_id()const{
    return this->prod_id;
}
int get_stock()const{
    return this->stock;
}
int get_updated_at()const{
    return this->updated_at;
}
};