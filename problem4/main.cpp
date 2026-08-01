#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class Product{
    protected:
        int id, quantity;
        string name; 
        double price;

    public:
        Product(int id, int quantity, string name, double price)
        : id(id), quantity(quantity), name(name), price(price){}

        virtual void display() const = 0;

        double calculateInventoryValue() const{
            return price*quantity;
        }
        int getProductId() const{
            return id;
        }
        void addStock(int qty){
            if(qty<0){
                cout << "Invalid quantity\n";
                return;
            }
            quantity -= qty;
        }
        void removeStock(int qty){
            if(qty<0){
                cout << "Invalid quantity\n";
                return;
            }
            if(qty>quantity){
                cout << "Not enough stock\n";
                return;
            }
            quantity -= qty;
        }
        virtual ~Product() = default;
};
class Laptop : public Product{
    private:
        string brand;
        int ram, warranty;

    public:
        Laptop(int id, int quantity, string name, double price, string brand, int ram, int warranty) : 
            Product(id,quantity,name,price), 
            brand(brand), 
            ram(ram), 
            warranty(warranty){}

        void display() const override{
            cout << "Laptop\n";
            cout << "ID : " << id << '\n';
            cout << "Name : " << name << '\n';
            cout << "Brand : " << brand << '\n';
            cout << "RAM : " << ram << " GB\n";
            cout << "Warranty : " << warranty << " Years\n";
            cout << "Price : " << price << '\n';
            cout << "Quantity : " << quantity << '\n';
            cout << "Inventory Value : "
                << calculateInventoryValue() << '\n';
            cout << "-----------------------------\n";
        }
};
class Smartphone : public Product {
private:
    int battery;
    int camera;

public:
    Smartphone(int id,
               string name,
               double price,
               int quantity,
               int battery,
               int camera)
        : Product(id, quantity, name, price),
          battery(battery),
          camera(camera) {}

    void display() const override {
        cout << "Smartphone\n";
        cout << "ID : " << id << '\n';
        cout << "Name : " << name << '\n';
        cout << "Battery : " << battery << " mAh\n";
        cout << "Camera : " << camera << " MP\n";
        cout << "Price : " << price << '\n';
        cout << "Quantity : " << quantity << '\n';
        cout << "Inventory Value : "
             << calculateInventoryValue() << '\n';
        cout << "-----------------------------\n";
    }
};
class SmartWatch : public Product {
private:
    bool waterResistant;
    string strapMaterial;

public:
    SmartWatch(int id,
               string name,
               double price,
               int quantity,
               bool waterResistant,
               string strapMaterial)
        : Product(id, quantity, name, price),
          waterResistant(waterResistant),
          strapMaterial(strapMaterial) {}

    void display() const override {
        cout << "SmartWatch\n";
        cout << "ID : " << id << '\n';
        cout << "Name : " << name << '\n';
        cout << "Water Resistant : "
             << (waterResistant ? "Yes" : "No") << '\n';
        cout << "Strap Material : "
             << strapMaterial << '\n';
        cout << "Price : " << price << '\n';
        cout << "Quantity : " << quantity << '\n';
        cout << "Inventory Value : "
             << calculateInventoryValue() << '\n';
        cout << "-----------------------------\n";
    }
};
class Store{
    private:
        vector<unique_ptr<Product>> products;

    public:
        void addProduct(unique_ptr<Product> p){
            products.push_back(move(p));
        }
        void displayAllProducts() const{
            if(products.empty()){
                cout << "Inventory empty\n";
                return;
            }
            for(const auto &p : products){
                // p->display(); both are same 
                (*p).display();
            }
        }
        void displayProduct(int id) const{
            for(const auto &p : products){
                if(p->getProductId() == id){
                    p->display();
                    return;
                }
            }
            cout << "Product not found\n";
        }
        double totalInventoryValue() const{
            double total = 0;
            for(const auto &p : products){
                total += p->calculateInventoryValue();
            }
            return total;
        }
        void updateStock(int id, int qty){
            for(auto &p : products){
                if(p->getProductId() == id){
                    p->removeStock(qty);
                    return;
                }
            }
        }
        void sellProduct(int id, int qty){
            for(auto &p : products){
                if(p->getProductId()==id){
                    p->removeStock(qty);
                    return;
                }
            }
            cout << "Product Not Found\n";
        }
        void removeProduct(int id){
            for(auto it = products.begin(); it!=products.end(); it++){
                if((*it)->getProductId()==id){
                    products.erase(it);
                }
            }
            cout << "Product Not Found\n";
        }
        ~Store() = default;
};
int main(){

    Store store;
    store.addProduct(
        make_unique<Laptop>(101,5,"Macbook pro",2000,"Apple",16,2)
    );
    store.addProduct(
        make_unique<Smartphone>(102,"iPhone 17",1000,10,5000,48)
    );
    store.addProduct(
        make_unique<SmartWatch>(103,"S3",800,10,true,"Leather")
    );
    store.displayAllProducts();
    cout << "\nTotal Inventory Value : "
         << store.totalInventoryValue()
         << '\n';

    cout << "\nSelling 2 Phones\n";
    store.sellProduct(102, 2);

    cout << "\nAdding 5 Watches\n";
    store.updateStock(103, 5);

    cout << "\nDisplaying Product 102\n";
    store.displayProduct(102);

    cout << "\nRemoving Product 101\n";
    store.removeProduct(101);

    cout << "\n=========== REMAINING PRODUCTS ===========\n";
    store.displayAllProducts();
    return 0;
}
