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

// Посетитель для расчета характеристик
struct NutritionVisitor {
    struct NutritionInfo {
        double calories = 0;
        double proteins = 0;
        double fats = 0;
        double carbs = 0;
        double cost = 0;
    } info;

    void visit(Mushroom* m) {
        const double gramsToKilograms = m->weight / 100.0;
        info.calories += 27 * gramsToKilograms;
        info.proteins += 4.4 * gramsToKilograms;
        info.fats += 1 * gramsToKilograms;
        info.carbs += 0.2 * gramsToKilograms;
        info.cost += 20 * gramsToKilograms;
    }

    void visit(Shrimp* s) {
        const double gramsToKilograms = s->weight / 100.0;
        info.calories += 83 * gramsToKilograms;
        info.proteins += 18 * gramsToKilograms;
        info.fats += 1 * gramsToKilograms;
        info.carbs += 0 * gramsToKilograms;
        info.cost += 50 * gramsToKilograms;
    }

    void visit(SourCream* sc) {
        const double gramsToKilograms = sc->weight / 100.0;
        info.calories += 120 * gramsToKilograms;
        info.proteins += 3.3 * gramsToKilograms;
        info.fats += 10 * gramsToKilograms;
        info.carbs += 3.3 * gramsToKilograms;
        info.cost += 14.4 * gramsToKilograms;
    }

    void visit(Cheese* c) {
        const double gramsToKilograms = c->weight / 100.0;
        info.calories += 345 * gramsToKilograms;
        info.proteins += 25 * gramsToKilograms;
        info.fats += 25 * gramsToKilograms;
        info.carbs += 0 * gramsToKilograms;
        info.cost += 70 * gramsToKilograms;
    }

    void visit(Dill* d) {
        const double gramsToKilograms = d->weight / 100.0;
        info.calories += 37 * gramsToKilograms;
        info.proteins += 3.3 * gramsToKilograms;
        info.fats += 0 * gramsToKilograms;
        info.carbs += 7 * gramsToKilograms;
        info.cost += 20 * gramsToKilograms;
    }
};