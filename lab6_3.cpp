#include <iostream>
#include <vector>
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

// Посетитель для расчета характеристик
struct NutritionVisitor {
    double totalCalories = 0;
    double totalProteins = 0;
    double totalFats = 0;
    double totalCarbs = 0;
    double totalCost = 0;

    void visit(Mushroom* m) {
        double ratio = m->weight / 100;
        totalCalories += 27 * ratio;
        totalProteins += 4.4 * ratio;
        totalFats += 1 * ratio;
        totalCarbs += 0.2 * ratio;
        totalCost += 20 * ratio;
    }

    void visit(Shrimp* s) {
        double ratio = s->weight / 100;
        totalCalories += 83 * ratio;
        totalProteins += 18 * ratio;
        totalFats += 1 * ratio;
        totalCarbs += 0 * ratio;
        totalCost += 50 * ratio;
    }

    void visit(SourCream* sc) {
        double ratio = sc->weight / 100;
        totalCalories += 120 * ratio;
        totalProteins += 3.3 * ratio;
        totalFats += 10 * ratio;
        totalCarbs += 3.3 * ratio;
        totalCost += 14.4 * ratio;
    }

    void visit(Cheese* c) {
        double ratio = c->weight / 100;
        totalCalories += 345 * ratio;
        totalProteins += 25 * ratio;
        totalFats += 25 * ratio;
        totalCarbs += 0 * ratio;
        totalCost += 70 * ratio;
    }

    void visit(Dill* d) {
        double ratio = d->weight / 100;
        totalCalories += 37 * ratio;
        totalProteins += 3.3 * ratio;
        totalFats += 0 * ratio;
        totalCarbs += 7 * ratio;
        totalCost += 20 * ratio;
    }
};

// Реализация accept для каждого продукта
void Mushroom::accept(NutritionVisitor& visitor) { visitor.visit(this); }
void Shrimp::accept(NutritionVisitor& visitor) { visitor.visit(this); }
void SourCream::accept(NutritionVisitor& visitor) { visitor.visit(this); }
void Cheese::accept(NutritionVisitor& visitor) { visitor.visit(this); }
void Dill::accept(NutritionVisitor& visitor) { visitor.visit(this); }