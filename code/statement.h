#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <QString>
#include <QStringList>
#include "exp.h"
#include "evalstate.h"
#include "parser.h"
#include "tokenizer.h"
class IFerror
{
};
class LETerror
{
};
class INPUTerror
{
};

enum statementSort
{
    REMSTAT,
    LETSTAT,
    PRINTSTAT,
    INPUTSTAT,
    GOTOSTAT,
    ENDSTAT,
    IFSTAT
};

class statement
{
public:
    statement() {}
    virtual ~statement() {}
    virtual statementSort getType() = 0;

    // 用于生成命令树
    virtual QStringList treeGenerator() = 0;

public:
    //各条statement不同的一些特性函数
    virtual int getVal() { return -1; }
    virtual bool isTrue() { return false; }
    virtual int getTar() { return -1; }
    virtual void execute(EvaluationContext &context) {context.isDefined("warning out");}
};

class IfStat : public statement
{
private:
    bool flag;         // IS TRUE?
    int tarLine;       // THEN tarline
    tokenType current; // IF (A)current(B)THEN
    Expression *leftEXP, *rightEXP;
    Expression *thenEXP;

public:
    IfStat(QList<token *> left, QList<token *> right);
    QStringList treeGenerator();
    statementSort getType() { return IFSTAT; }
    parser StateParser;
    bool isTrue() { return flag; };
    void execute(EvaluationContext &context);
};

class LetStat : public statement
{
private:
    parser StateParser;
    Expression *parserResult;

public:
    ~LetStat() {}

    LetStat(QList<token *> tokenList);
    QStringList treeGenerator();
    void execute(EvaluationContext &context) { parserResult->eval(context); }
    statementSort getType() { return LETSTAT; }
};

class PrintStat : public statement
{
private:
    parser StateParser;
    Expression *parserResult;
    int val;

public:
    PrintStat(QList<token *> tokenList) { parserResult = StateParser.parse(tokenList); }
    QStringList treeGenerator();
    void execute(EvaluationContext &context) { val = parserResult->eval(context); }
    int getVal() { return val; }
    statementSort getType() { return PRINTSTAT; }
};

class GotoStat : public statement
{
private:
    int tarLine;
    Expression *target;

public:
    parser StateParser;
    GotoStat(QList<token *> current);
    QStringList treeGenerator();
    statementSort getType() { return GOTOSTAT; }
    int getTar() { return tarLine; };
    void execute(EvaluationContext &context) { tarLine = target->eval(context); }
};

#endif // STATEMENT_H
