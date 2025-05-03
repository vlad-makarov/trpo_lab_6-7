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

// BinaryOperation
struct BinaryOperation : Expression {
    enum { PLUS = '+', MINUS = '-', MUL = '*', DIV = '/' };

    BinaryOperation(Expression const* left, int op, Expression const* right)
        : left_(left), right_(right), op_(op) {
    }

    ~BinaryOperation() {
        delete left_;
        delete right_;
    }

    double evaluate() const override {
        switch (op_) {
        case PLUS: return left_->evaluate() + right_->evaluate();
        case MINUS: return left_->evaluate() - right_->evaluate();
        case MUL: return left_->evaluate() * right_->evaluate();
        case DIV: return left_->evaluate() / right_->evaluate();
        default: return 0.0;
        }
    }

    Expression* transform(Transformer* tr) const override;
    void print(std::ostream& out) const override {
        out << "(";
        left_->print(out);
        out << " " << static_cast<char>(op_) << " ";
        right_->print(out);
        out << ")";
    }

    Expression const* left() const { return left_; }
    Expression const* right() const { return right_; }
    int operation() const { return op_; }

private:
    Expression const* left_;
    Expression const* right_;
    int op_;
};

// FunctionCall
struct FunctionCall : Expression {
    FunctionCall(std::string const& name, Expression const* arg)
        : name_(name), arg_(arg) {
    }

    ~FunctionCall() { delete arg_; }

    double evaluate() const override {
        double val = arg_->evaluate();
        if (name_ == "sqrt") return std::sqrt(val);
        if (name_ == "abs") return std::fabs(val);
        return 0.0;
    }

    Expression* transform(Transformer* tr) const override;
    void print(std::ostream& out) const override {
        out << name_ << "(";
        arg_->print(out);
        out << ")";
    }

    std::string const& name() const { return name_; }
    Expression const* arg() const { return arg_; }

private:
    std::string const name_;
    Expression const* arg_;
};

// Transformer (Visitor interface)
struct Transformer {
    virtual ~Transformer() {}
    virtual Expression* transformNumber(Number const*) = 0;
    virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
    virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
    virtual Expression* transformVariable(Variable const*) = 0;
};

// Реализация transform()
Expression* Number::transform(Transformer* tr) const { return tr->transformNumber(this); }
Expression* Variable::transform(Transformer* tr) const { return tr->transformVariable(this); }
Expression* BinaryOperation::transform(Transformer* tr) const { return tr->transformBinaryOperation(this); }
Expression* FunctionCall::transform(Transformer* tr) const { return tr->transformFunctionCall(this); }