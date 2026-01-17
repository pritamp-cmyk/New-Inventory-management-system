#include <string>
#include <vector>
using namespace std;
#include "../interfaces/IinventoryService.h"
#include "../../repository/interfaces/IproductRepo.h"
#include "../../repository/interfaces/IinventoryRepo.h"
#include "domain/inventory.h"
#include "domain/product.h"
class InventoryService : public IinventoryService{
    private:
    IproductRepo* product_repo;
    IinventoryRepo* inventory_repo;
    public:
    InventoryService(IproductRepo* product_repo,IinventoryRepo* inventory_repo):product_repo(product_repo),inventory_repo(inventory_repo){}

    int createProduct(string name, string description,int initial_stock)override{
        if(initial_stock <0){
            throw invalid_argument(" initial stock can not be negative");
        }
        int prod_id = product_repo->create(name,description);
        inventory_repo->create(prod_id,initial_stock);
        return prod_id;
    }

    product getProduct(int prod_id)override{
        return product_repo->find_by_id(prod_id);
    }

    void updateProduct(int prod_id,string name,string description)override{
        product_repo->update(prod_id,name,description);
    }
    void updateStock(int prod_id,int new_stock)override{
        if(new_stock <0){
            throw invalid_argument(" stock can not be negative");
        }
        inventory_repo->update_stock(prod_id,new_stock);
    };
    void deleteProduct(int prod_id)override{
        inventory_repo->removeProductBy_id(prod_id);
        product_repo->remove(prod_id);
    };
    vector<product> searchProduct(string name)override{
        return product_repo->find_by_name(name);
    };

};