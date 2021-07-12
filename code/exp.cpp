#include "exp.h"
Expression::Expression(){}
Expression::~Expression(){}
ConstantExp::ConstantExp(int val)
{
    value = val;
}

ConstantExp::~ConstantExp() {}

QStringList ConstantExp::generateEXPtree()
{
    QStringList ret;
    QString current=QString::number(value);
    ret.append(current);
    return ret;
}

ExpressionType ConstantExp::type()
{
    return CONSTANT;
}

int ConstantExp::getConstantValue()
{
    return value;
}

IdentifierExp::IdentifierExp(string iname)
{
    name = iname;
    hasValue = false;
}

int IdentifierExp::eval(EvaluationContext &context)
{
    if(context.isDefined(name))
        return context.getValue(name);
    else
        throw NOIDEN();
}

ExpressionType IdentifierExp::type()
{
    return IDENTIFIER;
}


QStringList IdentifierExp::generateEXPtree()
{
    QStringList ret;
    QString current=QString::fromStdString(name);
    ret.append(current);
    return ret;
}

string IdentifierExp::getIdentifierName()
{
    return name;
}

CompoundExp::CompoundExp(string opq, Expression *lhsr, Expression *rhsr)
{
    op = opq;
    lhs = lhsr, rhs = rhsr;
}

CompoundExp::~CompoundExp()
{
}


int CompoundExp::eval(EvaluationContext &context)
{
    int right = rhs->eval(context);
    if (op == "=")
    {
        string leftVar = lhs->getIdentifierName();
        context.setValue(leftVar, right);
        return right;
    }
    int left = lhs->eval(context);
    if (op == "+")
        return (left + right);
    if (op == "-")
        return (left - right);
    if (op == "*")
        return (left * right);
    if (op == "/")
    {
        if (right == 0)
            throw DIVIDEZERO();
        else
            return left / right;
    }
    if(op=="**")
        return pow(left,right);
    throw NOOP();
}

//实现复杂式子的缩进完成
QStringList CompoundExp::generateEXPtree()
{
    QStringList ret;
    QString current;
    current=QString::fromStdString(op);
    ret.append(current);
    QStringList currentList;
    currentList=lhs->generateEXPtree();
    for(int i=0;i<currentList.length();++i)
        currentList[i].insert(0,'\t');
    ret<<currentList;
    currentList=rhs->generateEXPtree();
    for(int i=0;i<currentList.length();++i)
        currentList[i].insert(0,'\t');
    ret<<currentList;
    return ret;
}

ExpressionType CompoundExp::type()
{
    return COMPOUND;
}

string CompoundExp::getOperator()
{
    return op;
}

Expression *CompoundExp::getLHS()
{
    return lhs;
}

Expression *CompoundExp::getRHS()
{
    return rhs;
}

