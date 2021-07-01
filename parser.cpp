#include "parser.h"

parser::parser()
{
    rightOprand = NULL, leftOprand = NULL;
    ret = NULL, tmp = NULL;
}

Expression *parser::parse(QList<token *> cmd)
{
    int cmdLen = cmd.length();
    for (int i = 0; i < cmdLen; ++i)
    {
        token current = (*(cmd[i]));
        tokenType currentType = current.getType();

        switch (currentType)
        {
        case NUMBER:
        {
            tmp = new ConstantExp(current.getNum());
            oprand.push(tmp);
            break;
        }
        case IDEN:
        {
            tmp = new IdentifierExp(current.getIdenifier().toStdString());
            oprand.push(tmp);
            break;
        }
        case PLUS:
        case MINUS:
        {
            while (!oprand.empty() && !oprator.empty() &&
                   (oprator.top().getType() != EQUAL &&
                    oprator.top().getType() != LEFT &&
                    oprator.top().getType() != GREATER &&
                    oprator.top().getType() != LESS))
            {
                token currentOp = oprator.pop();
                simplify(currentOp);
            }
            oprator.push(current);
            break;
        }
        case MULTIPLE:
        case DIVIDE:
        {
            while (!oprand.empty() && !oprator.empty() &&
                   (oprator.top().getType() == MULTIPLE ||
                    oprator.top().getType() == DIVIDE ||
                    oprator.top().getType() == POWER))
            {
                token currentOp = oprator.pop();
                simplify(currentOp);
            }
            oprator.push(current);
            break;
        }
        case NEGATIVE:
        {
            oprator.push(current);
            break;
        }
        case POWER:
        {
            oprator.push(current);
            break;
        }
        case EQUAL:
        {
            while (!oprand.empty() && !oprator.empty())
            {
                token currentOp = oprator.pop();
                simplify(currentOp);
            }
            oprator.push(current);
            break;
        }

        case LEFT:
            oprator.push(current);
            break;
        case RIGHT:
        {
            token currentOp = oprator.pop();
            while (currentOp.getType() != LEFT)
            {
                simplify(currentOp);
                currentOp = oprator.pop();
            }
            break;
        }
        default:
            throw tokenERROR();
        }
    }

    if (oprator.empty() && oprand.length() == 1)
    {
        ret = oprand.pop();
    }
    else
    {
        token currentOp;
        while (!oprand.empty() && !oprator.empty())
        {
            if (oprator.empty())
                throw tokenERROR();
            currentOp = oprator.pop();
            simplify(currentOp);
            if (oprand.length() == 1 && oprator.empty())
            {
                ret = tmp;
                break;
            }
        }
    }
    return ret;
}

void parser::simplify(token currentOp)
{
    if (currentOp.getType() == NEGATIVE)
    {
        if (oprand.empty())
            throw tokenERROR();
        rightOprand = oprand.pop();
        Expression *zero = new ConstantExp(0);
        tmp = new CompoundExp(currentOp.getOp().toStdString(),
                              zero, rightOprand);
    }
    else
    {
        if (oprand.empty())
            throw tokenERROR();
        rightOprand = oprand.pop();
        if (oprand.empty())
            throw tokenERROR();
        leftOprand = oprand.pop();
        tmp = new CompoundExp(currentOp.getOp().toStdString(),
                              leftOprand, rightOprand);
    }
    oprand.push(tmp);
}
