#include <string>
#include <vector>
using namespace std;
#include "../interfaces/IsubscriptionService.h"
#include "../../repository/interfaces/IsubscriptionRepo.h"


class SubscriptionService : public IsubscriptionService{
    private:
    IsubscriptionRepo* subscription_repo;
    public:
    SubscriptionService(IsubscriptionRepo* subscription_repo):subscription_repo(subscription_repo){}

    void subscribeUser(int prod_id,int user_id)override{
        if(prod_id<=0 || user_id<=0){
            throw invalid_argument("Invalid product id or user id");
        }
        subscription_repo->subscribe(prod_id,user_id);
    }
   void unsubscribeUser(int prod_id,int user_id)override{
        if(prod_id<=0 || user_id<=0){
            throw invalid_argument("Invalid product id or user id");
        }
        subscription_repo->unsubscribe(prod_id,user_id);
   }
//    vector<int> getSubscribers(int prod_id)override{
//     if(prod_id<=0){
//             throw invalid_argument("Invalid product id");
//         }
//     return subscription_repo->find_subscribers(prod_id);
//    }
};