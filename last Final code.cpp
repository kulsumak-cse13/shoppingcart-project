
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

// ==================== Product Class ====================
class Product {
public:
    int id;
    string name;
    string category;
    double price;
    int stock;

    Product(int i, string n, string c, double p, int s) {
        id = i;
        name = n;
        category = c;
        price = p;
        stock = s;
    }

    void display() {
        cout << id << " | " << name << " | " << category
             << " | Price: " << price << " | Stock: " << stock << endl;
    }
};

// ==================== Customer Class ====================
class Customer {
public:
    string name;
    string phone;
    string address;

    Customer(string n, string ph, string a) {
        name = n;
        phone = ph;
        address = a;
    }
};

// ==================== Shopping Cart Class ====================
class ShoppingCart {
private:
    vector<Product> products;
    vector<Product> cart;

public:
    // Add Product to Shop
    void addProductToShop(Product p) {
        products.push_back(p);
        cout << p.name << " added to shop ✅" << endl;
    }

    // Show all products
    void showProducts() {
        cout << "\n📦 Product List:\n";
        for (auto &p : products) p.display();
    }

    // Search Product
    Product* searchProduct(int id) {
        for (auto &p : products) {
            if (p.id == id) return &p;
        }
        return nullptr;
    }

    // Add product to cart
    void addToCart(int id, int qty) {
        Product* p = searchProduct(id);
        if (p != nullptr && p->stock >= qty) {
            cart.push_back(Product(p->id, p->name, p->category, p->price, qty));
            p->stock -= qty; // reduce stock
            cout << qty << " " << p->name << " added to cart 🛒" << endl;
        } else {
            cout << "❌ Product not available or insufficient stock!" << endl;
        }
    }

    // Remove product from cart
    void removeFromCart(int id) {
        bool found = false;
        for (int i = 0; i < cart.size(); i++) {
            if (cart[i].id == id) {
                cout << cart[i].name << " removed from cart ❌" << endl;
                cart.erase(cart.begin() + i);
                found = true;
                break;
            }
        }
        if (!found) cout << "Product not found in cart!" << endl;
    }

    // View Cart + Bill
    double viewCart(double discount = 0.0, double taxRate = 0.0) {
        cout << "\n🛒 Your Cart:\n";
        double subtotal = 0;
        for (auto &p : cart) {
            p.display();
            subtotal += p.price * p.stock;
        }
        double discountAmt = subtotal * (discount / 100.0);
        double taxAmt = (subtotal - discountAmt) * (taxRate / 100.0);
        double total = subtotal - discountAmt + taxAmt;

        cout << "----------------------" << endl;
        cout << "Subtotal: " << subtotal << " TK" << endl;
        cout << "Discount: -" << discountAmt << " TK" << endl;
        cout << "Tax: +" << taxAmt << " TK" << endl;
        cout << "TOTAL: " << total << " TK" << endl;

        return total;
    }

    // Generate Invoice
    void generateInvoice(Customer cust, double discount, double tax) {
        time_t now = time(0);
        char* dt = ctime(&now);

        ofstream file("invoice.txt", ios::app);
        file << "================ INVOICE ================\n";
        file << "Customer: " << cust.name << " | Phone: " << cust.phone << "\n";
        file << "Address: " << cust.address << "\n";
        file << "Date: " << dt << "\n";
        file << "-----------------------------------------\n";
        double total = viewCart(discount, tax);
        file << "TOTAL BILL: " << total << " TK\n";
        file << "=========================================\n\n";
        file.close();

        cout << "📄 Invoice saved to invoice.txt\n";
    }

    // Sales Report
    void salesReport() {
        ifstream file("invoice.txt");
        if (!file) {
            cout << "No sales record found!" << endl;
            return;
        }
        cout << "\n📊 Sales Report:\n";
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};

// ==================== Main Program ====================
int main() {
    ShoppingCart shop;
    int choice;

    // Sample Products
    shop.addProductToShop(Product(1, "Saree", "Clothes", 1500, 10));
    shop.addProductToShop(Product(2, "Bangle", "Jewelry", 200, 20));
    shop.addProductToShop(Product(3, "Ring", "Jewelry", 500, 15));

    do {
        cout << "\n===== Shopping System Menu =====" << endl;
        cout << "1. Show Products" << endl;
        cout << "2. Add to Cart" << endl;
        cout << "3. Remove from Cart" << endl;
        cout << "4. View Cart" << endl;
        cout << "5. Checkout & Invoice" << endl;
        cout << "6. Sales Report" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            shop.showProducts();
        }
        else if (choice == 2) {
            int id, qty;
            cout << "Enter Product ID: ";
            cin >> id;
            cout << "Enter Quantity: ";
            cin >> qty;
            shop.addToCart(id, qty);
        }
        else if (choice == 3) {
            int id;
            cout << "Enter Product ID to remove: ";
            cin >> id;
            shop.removeFromCart(id);
        }
        else if (choice == 4) {
            shop.viewCart(10, 5); // example: 10% discount, 5% tax
        }
        else if (choice == 5) {
            string name, phone, addr;
            cout << "Enter Customer Name: ";
            cin >> name;
            cout << "Enter Phone: ";
            cin >> phone;
            cout << "Enter Address: ";
            cin.ignore();
            getline(cin, addr);
            Customer c(name, phone, addr);
            shop.generateInvoice(c, 10, 5);
        }
        else if (choice == 6) {
            shop.salesReport();
        }
    } while (choice != 0);

    return 0;
}


