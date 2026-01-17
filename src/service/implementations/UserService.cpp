
#include <string>
#include <vector>
using namespace std;
#include "../interfaces/IuserService.h"
#include "../../repository/interfaces/IuserRepo.h"

class UserService : public IuserService{
    private:
    IuserRepo* user_repo;
    public:
    UserService(IuserRepo* user_repo):user_repo(user_repo){}
    int createUser(string name,string email,string role)override{
        if(name.empty() || email.empty() || role.empty()){
            throw invalid_argument("name, email and role cannot be empty");
        }
        return user_repo->create(name,email,role);
    }
    user getUser(int user_id)override{
        return user_repo->find_by_id(user_id);
    }
};