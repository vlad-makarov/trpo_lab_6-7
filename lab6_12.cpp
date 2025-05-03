#include <iostream>
#include <string>
#include <cmath>

struct Transformer;

// Expression (базовый класс)
struct Expression {
    virtual ~Expression() {}
    virtual double evaluate() const = 0;
    virtual Expression* transform(Transformer* tr) const = 0;
    virtual void print(std::ostream& out) const = 0;
};

// Number
struct Number : Expression {
    explicit Number(double value) : value_(value) {}
    double value() const { return value_; }

    double evaluate() const override { return value_; }

    Expression* transform(Transformer* tr) const override;
    void print(std::ostream& out) const override { out << value_; }

private:
    double value_;
};

// Variable
struct Variable : Expression {
    explicit Variable(std::string const& name) : name_(name) {}
    std::string const& name() const { return name_; }

    double evaluate() const override { return 0.0; }

    Expression* transform(Transformer* tr) const override;
    void print(std::ostream& out) const override { out << name_; }

private:
    std::string const name_;
};