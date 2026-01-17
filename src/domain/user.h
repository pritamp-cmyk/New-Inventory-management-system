#pragma once
#include<string>
using namespace std;
class user
{
private:
    int user_id;
    string email;
    string name;
    string role;  //admin or user
public:
    user(int id,  string name, string email,string role){
        this->user_id=id;
        this->name=name;
        this->email=email;
        this->role=role;
    }
    int get_id()const{
       return this->user_id; 
    }
    string get_name()const{return this->name;}
    string get_email()const{return this->email;}
    string get_role()const {return this->role;}
    ~user();
};