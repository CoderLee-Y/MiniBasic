#include "statement.h"

IfStat::IfStat(QList<token *> left, QList<token *> right)
{
    QList<token *> left_left, left_right;

    for (int i = 0; i < left.length(); ++i)
    {
        current = left[i]->getType();
        if (current != EQUAL && current != GREATER && current != LESS)
        {
            left_left.append(left[i]);
            if (i + 1 == left.length())
                throw IFerror();
        }
        else
        {
            while (++i < left.length())
                left_right.append(left[i]);
            break;
        }
    }

    leftEXP = StateParser.parse(left_left);
    rightEXP = StateParser.parse(left_right);
    thenEXP = StateParser.parse(right);
}

void IfStat::execute(EvaluationContext &context)
{
    int leftVal = leftEXP->eval(context), rightVal = rightEXP->eval(context);
    switch (current)
    {
    case EQUAL:
        flag = (leftVal == rightVal);
        break;
    case GREATER:
        flag = (leftVal > rightVal);
        break;
    case LESS:
        flag = (leftVal < rightVal);
        break;
    default:
        throw IFerror();
    }
    tarLine = thenEXP->eval(context);
}

QStringList IfStat::treeGenerator()
{
    QStringList tree;
    tree << leftEXP->generateEXPtree();
    switch (current)
    {
    case EQUAL:
    {
        QString tmp = "=";
        tree.append(tmp);
    }
    break;
    case GREATER:
    {
        QString tmp = ">";
        tree.append(tmp);
    }
    break;
    case LESS:
    {
        QString tmp = "<";
        tree.append(tmp);
    }
    break;
    default:
        throw IFerror();
    }

    tree << rightEXP->generateEXPtree();
    for (int i = 0; i < tree.length(); ++i)
        tree[i].insert(0, '\t');
    return tree;
}

LetStat::LetStat(QList<token *> tokenList)
{
    parserResult = StateParser.parse(tokenList);
    // is a let statement?
    if (parserResult->type() != COMPOUND)
        throw LETerror();
    if (parserResult->getOperator() != "=")
        throw LETerror();
}

QStringList LetStat::treeGenerator()
{
    QStringList statementTree;
    statementTree << parserResult->generateEXPtree();
    for (int i = 0; i < statementTree.length(); ++i)
        statementTree[i].insert(0, '\t');
    return statementTree;
}

QStringList PrintStat::treeGenerator()
{
    QStringList statementTree;
    statementTree << parserResult->generateEXPtree();
    for (int i = 0; i < statementTree.length(); ++i)
        statementTree[i].insert(0, '\t');
    return statementTree;
}

GotoStat::GotoStat(QList<token *> current)
{
    target = StateParser.parse(current);
    if (target->type() != CONSTANT)
        throw tokenERROR();
}

QStringList GotoStat::treeGenerator()
{
    QStringList Tree;
    Tree << target->generateEXPtree();
    for (int i = 0; i < Tree.length(); ++i)
        Tree[i].insert(0, '\t');
    return Tree;
}
