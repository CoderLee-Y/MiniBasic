/*
 * File: exp.h
 * -----------
 * This interface defines a class hierarchy for arithmetic expressions.
 */

#ifndef _exp_h
#define _exp_h

#include <string>
#include <map>
#include <cmath>
#include "tokenizer.h"
#include "evalstate.h"

using namespace std;

// exception claim
class NOOP
{
};
class DIVIDEZERO
{
};
class NOIDEN
{
};

/* Forward reference */
class EvaluationContext;

/*
 * Type: ExpressionType
 * --------------------
 * This enumerated type is used to differentiate the three different
 * expression types: CONSTANT, IDENTIFIER, and COMPOUND.
 */

enum ExpressionType
{
    CONSTANT,
    IDENTIFIER,
    COMPOUND
};

/*
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Expression itself is an abstract class.  Every Expression object
 * is therefore created using one of the three concrete subclasses:
 * ConstantExp, IdentifierExp, or CompoundExp.
 */

class Expression
{
public:
    Expression();
    virtual ~Expression();
    virtual int eval(EvaluationContext &context){return 0;}
    virtual QString eval(){return "";}
    virtual QStringList generateEXPtree() = 0;
    virtual ExpressionType type() = 0;

    /* Getter methods for convenience */
    virtual int getConstantValue(){return 0;};
    virtual std::string getIdentifierName(){return "";};
    virtual std::string getOperator(){return "";};
    virtual Expression *getLHS(){return NULL;};
    virtual Expression *getRHS(){return NULL;};
};
/*
 * Class: ConstantExp
 * ------------------
 * This subclass represents a constant integer expression.
 */

class ConstantExp : public Expression
{
public:
    ConstantExp(int val);
    ~ConstantExp();
    int eval(EvaluationContext &context){return value;};
    QStringList generateEXPtree();
    ExpressionType type();

    int getConstantValue();

private:
    int value;
};
/*
 * Class: IdentifierExp
 * --------------------
 * This subclass represents a expression corresponding to a variable.
 */

class IdentifierExp : public Expression
{
public:
    IdentifierExp(string name);

    int eval(EvaluationContext &context);
    QStringList generateEXPtree();
    ExpressionType type();

    string getIdentifierName();

private:
    std::string name;
    int value;
    bool hasValue;
};
/*
 * Class: CompoundExp
 * ------------------
 * This subclass represents a compound expression.
 */

class CompoundExp : public Expression
{
public:
    CompoundExp(string op, Expression *lhs, Expression *rhs);
    ~CompoundExp();

    int eval(EvaluationContext &context);
    QStringList generateEXPtree();
    ExpressionType type();

    std::string getOperator();
    Expression *getLHS();
    Expression *getRHS();

private:
    std::string op;
    Expression *lhs, *rhs;
    int value;
    bool hasValue;
};


#endif
