#pragma once
#include<string>
#include"domain/user.h"
using namespace std;

class IuserService
{
public:
   virtual int createUser(string name,string email,string role)=0;
   virtual user getUser(int user_id)=0; 
   virtual ~IuserService()=default;
};


