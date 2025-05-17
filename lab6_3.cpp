#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Базовый класс продукта
struct Product {
    string name;
    double weight;
    virtual ~Product() {}
    virtual void accept(class NutritionVisitor& visitor) = 0;
};

// Конкретные продукты
struct Mushroom : Product {
    Mushroom(double w) { name = "Шампиньоны"; weight = w; }
    void accept(NutritionVisitor& visitor) override;
};

struct Shrimp : Product {
    Shrimp(double w) { name = "Креветки"; weight = w; }
    void accept(NutritionVisitor& visitor) override;
};

struct SourCream : Product {
    SourCream(double w) { name = "Сметана"; weight = w; }
    void accept(NutritionVisitor& visitor) override;
};

struct Cheese : Product {
    Cheese(double w) { name = "Сыр"; weight = w; }
    void accept(NutritionVisitor& visitor) override;
};

struct Dill : Product {
    Dill(double w) { name = "Укроп и пряности"; weight = w; }
    void accept(NutritionVisitor& visitor) override;
};