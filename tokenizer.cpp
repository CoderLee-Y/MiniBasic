#include "tokenizer.h"

QString token::getOp()
{
    switch (type)
    {
    case PLUS:
        return "+";
        break;
    case MINUS:
        return "-";
        break;
    case MULTIPLE:
        return "*";
        break;
    case DIVIDE:
        return "/";
        break;
    case NEGATIVE:
        return "-";
        break;
    case EQUAL:
        return "=";
        break;
    case LEFT:
        return "(";
        break;
    case RIGHT:
        return ")";
        break;
    case POWER:
        return "**";
        break;
    default:
        return "";
    }
}

QString token::getIdenifier()
{
    if (type == IDEN)
        return identifier;
    return "";
}

// 将command 分为可以被其他组件识别的token
// 要求command除去所有命令部分
// 只将表达式带入分析
QList<token *> tokenizer::tokenize(QStringList command)
{
    QList<token *> ret;
    token *appended;

    QStringList::iterator iter = command.begin();

    for (; iter != command.end(); ++iter)
    {
        int pos = 0;

        // current command
        QString current;
        current = *iter;

        while (pos < current.length())
        {
            if ((current[pos] >= 'a' && current[pos] <= 'z') ||
                (current[pos] >= 'A' && current[pos] <= 'Z'))
            {
                QString idenName = "";
                // push back and move position on until not AZ / 09
                idenName.push_back(current[pos++]);
                for (; pos < current.length(); ++pos)
                {
                    if ((current[pos] >= '0' && current[pos] <= '9') ||
                        (current[pos] >= 'a' && current[pos] <= 'z') ||
                        (current[pos] >= 'A' && current[pos] <= 'Z'))
                        idenName.push_back(current[pos]);
                    else
                        break;
                }

                appended = new token(IDEN, idenName);
                ret.push_back(appended);
                continue;
            }

            // begin with number, seen as constant
            else if (current[pos] >= '0' && current[pos] <= '9')
            {
                QString numberStr = "";
                numberStr.push_back(current[pos++]);
                while (pos < current.length())
                {
                    if (current[pos] >= '0' && current[pos] <= '9')
                        numberStr.push_back(current[pos++]);
                    else
                        break;
                }
                appended = new token(NUMBER, numberStr.toInt());
                ret.push_back(appended);
                continue;
            }
            else
            {
                if (current[pos] == '+')
                {
                    appended = new token(PLUS, current[pos++]);
                    ret.push_back(appended);
                    continue;
                }
                if (current[pos] == '-')
                {
                    QChar prev;
                    bool isNeg = false;

                    // is negative or minus?
                    // find previous char, if is frontest, negative
                    if (iter == command.begin() && pos == 0)
                        isNeg = true;
                    else if (pos == 0)
                        prev = (*(iter - 1)).back();
                    else
                        prev = current[pos - 1];

                    if (isNeg || prev == '(' ||
                        (prev == '='))
                        appended = new token(NEGATIVE, current[pos++]);
                    else
                        appended = new token(MINUS, current[pos++]);

                    ret.push_back(appended);
                    continue;
                }

                if (current[pos] == '*')
                {
                    if (current[pos + 1] == '*')
                    {
                        appended = new token(POWER, current[pos++]);
                        ++pos;
                        ret.push_back(appended);
                        continue;
                    }
                    else
                    {
                        appended = new token(MULTIPLE, current[pos++]);
                        ret.push_back(appended);
                        continue;
                    }
                }
                if (current[pos] == '/')
                {
                    appended = new token(DIVIDE, current[pos++]);
                    ret.push_back(appended);
                    continue;
                }
                if (current[pos] == '(')
                {
                    appended = new token(LEFT, current[pos++]);
                    ret.push_back(appended);
                    continue;
                }
                if (current[pos] == ')')
                {
                    appended = new token(RIGHT, current[pos++]);
                    ret.push_back(appended);
                    continue;
                }
                if (current[pos] == '=')
                {
                    appended = new token(EQUAL, current[pos++]);
                    ret.push_back(appended);
                    continue;
                }
                if (current[pos] == '>')
                {
                    appended = new token(GREATER, current[pos++]);
                    ret.push_back(appended);
                    continue;
                }
                if (current[pos] == '<')
                {
                    appended = new token(LESS, current[pos++]);
                    ret.push_back(appended);
                    continue;
                }
                throw tokenERROR();
            }
        }
    }
    return ret;
}
