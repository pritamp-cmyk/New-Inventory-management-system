#pragma once
#include "domain/user.h"
#include<string>
using namespace std;

class IuserRepo
{
public:
    virtual int create(string name,string email,string role)=0;
    virtual user find_by_name(string name)=0;
    virtual user find_by_id(int user_id)=0;
    virtual user find_by_email(string email)=0;
    virtual ~IuserRepo()=default;
};