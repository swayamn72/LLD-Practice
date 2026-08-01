#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Product{
    protected:
        int id;
        string name;
        double price;
        int quantity;

    public:
        Product(int id, string name, double price, int quantity){
            this->id = id;
            this->name = name;
            this->price = price;
            this->quantity = quantity;
        }

        virtual void display() const = 0;

        double getInventoryValue() const{
            return price*quantity;
        }

        int getId() const{
            return id;
        }

        void addStock(int qty){
            quantity += qty;
        }

        void removeStock(int qty){
            if(qty<=quantity){
                quantity -= qty;
            }else{
                cout << "Not enough stock" << "\n";
            }
        }
        virtual ~Product(){}
};

class Laptop : public Product{
    string brand;
    int warranty;

    public:
        Laptop(int id, string name, double price, int quantity, string brand, int warranty)
         : Product(id,name,price,quantity){
            this->brand = brand;
            this->warranty=warranty;
         }

    void display() const override{
        cout << "Laptop\n";
        cout << "ID : " << id << endl;
        cout << "Name : " << name << endl;
        cout << "Brand : " << brand << endl;
        cout << "Warranty : " << warranty << " years\n";
        cout << "Price : " << price << endl;
        cout << "Quantity : " << quantity << endl;
        cout << "Inventory Value : "
             << getInventoryValue() << endl;
    }
};

class Smartphone : public Product{
    int battery;
    public:
    Smartphone(int id, string name, double price,
               int quantity, int battery)
        : Product(id, name, price, quantity) {
        this->battery = battery;
    }
    void display() const override {
        cout << "Smartphone\n";
        cout << "ID : " << id << endl;
        cout << "Name : " << name << endl;
        cout << "Battery : " << battery << " mAh\n";
        cout << "Price : " << price << endl;
        cout << "Quantity : " << quantity << endl;
        cout << "Inventory Value : "
             << getInventoryValue() << endl;
    }
};

class Book : public Product{
    string author;
    public:
    Book(int id, string name, double price, int quantity, string author) : 
    Product(id, name, price, quantity){
        this->author = author;
    }
    void display() const override {
        cout << "Book\n";
        cout << "ID : " << id << endl;
        cout << "Name : " << name << endl;
        cout << "Author : " << author << endl;
        cout << "Price : " << price << endl;
        cout << "Quantity : " << quantity << endl;
        cout << "Inventory Value : "
             << getInventoryValue() << endl;
    }
};

class Inventory{
    vector<Product*> products;
    
    public:
        void addProduct(Product* p){
            products.push_back(p);
        }
        
        void displayAllProducts() const{
            for(auto p : products){
                p->display();
            }
        }

        void displayProduct(int id) const{
            for(auto p : products){
                if(p->getId()==id){
                    p->display();
                    return;
                }
            }
            cout << "Product not found\n";
        }
        double inventoryValue() const{
            double total = 0.0;
            for(auto p : products){
                total += p->getInventoryValue();
            }
            return total;
        }
        void removeProduct(int id){
            for(auto it = products.begin(); it!=products.end(); it++){
                if((*it)->getId() == id){
                    delete *it;
                    products.erase(it);
                    cout << "Product removed\n" << "\n";
                    return;
                }
            }
            cout << "Product not found\n";
        }
        ~Inventory(){
            for(auto p : products){
                delete p;
            }
        }
};
int main(){
    Inventory inv;

    inv.addProduct(new Laptop(101,"Macbook air",95000,5,"Apple",2));
    inv.addProduct(new Smartphone(102,"Galaxy S25",70000,4,5000));

    inv.addProduct(new Book(
        103,
        "Clean Code",
        900,
        20,
        "Robert Martin"));

    inv.displayAllProducts();

    cout << "\nTotal Inventory Value : "
         << inv.inventoryValue()
         << endl;

    cout << "\nDisplaying Product 102\n\n";
    inv.displayProduct(102);

    inv.removeProduct(103);

}