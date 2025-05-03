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

// Копирование дерева
struct CopySyntaxTree : Transformer {
    Expression* transformNumber(Number const* number) override {
        return new Number(number->value());
    }

    Expression* transformBinaryOperation(BinaryOperation const* binop) override {
        return new BinaryOperation(
            binop->left()->transform(this),
            binop->operation(),
            binop->right()->transform(this)
        );
    }

    Expression* transformFunctionCall(FunctionCall const* fcall) override {
        return new FunctionCall(
            fcall->name(),
            fcall->arg()->transform(this)
        );
    }

    Expression* transformVariable(Variable const* var) override {
        return new Variable(var->name());
    }
};

// Сворачивание констант
struct FoldConstants : Transformer {
    Expression* transformNumber(Number const* number) override {
        return new Number(number->value());
    }

    Expression* transformVariable(Variable const* var) override {
        return new Variable(var->name());
    }

    Expression* transformBinaryOperation(BinaryOperation const* binop) override {
        Expression* l = binop->left()->transform(this);
        Expression* r = binop->right()->transform(this);

        Number* ln = dynamic_cast<Number*>(l);
        Number* rn = dynamic_cast<Number*>(r);

        if (ln && rn) {
            double result = 0.0;
            switch (binop->operation()) {
            case BinaryOperation::PLUS: result = ln->value() + rn->value(); break;
            case BinaryOperation::MINUS: result = ln->value() - rn->value(); break;
            case BinaryOperation::MUL: result = ln->value() * rn->value(); break;
            case BinaryOperation::DIV: result = ln->value() / rn->value(); break;
            }
            delete l;
            delete r;
            return new Number(result);
        }

        return new BinaryOperation(l, binop->operation(), r);
    }

    Expression* transformFunctionCall(FunctionCall const* fcall) override {
        Expression* arg = fcall->arg()->transform(this);
        Number* n = dynamic_cast<Number*>(arg);

        if (n) {
            double result = 0.0;
            if (fcall->name() == "sqrt") result = std::sqrt(n->value());
            else if (fcall->name() == "abs") result = std::fabs(n->value());
            else return new FunctionCall(fcall->name(), arg);

            delete arg;
            return new Number(result);
        }

        return new FunctionCall(fcall->name(), arg);
    }
};

// Проверка
int main() {
    Number* n32 = new Number(32.0);
    Number* n16 = new Number(16.0);
    BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
    FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
    Variable* var = new Variable("var");
    BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
    FunctionCall* callAbs = new FunctionCall("abs", mult);

    std::cout << "Original: ";
    callAbs->print(std::cout);
    std::cout << " = " << callAbs->evaluate() << "\n";

    CopySyntaxTree CST;
    Expression* copied = callAbs->transform(&CST);
    std::cout << "Copied:   ";
    copied->print(std::cout);
    std::cout << " = " << copied->evaluate() << "\n";

    FoldConstants FC;
    Expression* folded = callAbs->transform(&FC);
    std::cout << "Folded:   ";
    folded->print(std::cout);
    std::cout << " = " << folded->evaluate() << "\n";

    delete callAbs;
    delete copied;
    delete folded;

    return 0;
}