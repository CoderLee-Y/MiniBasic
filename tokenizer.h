#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <QString>
#include <QStringList>
#include <QList>
class tokenERROR
{
};
class stringERROR
{
};

// 12 token sorts.
enum tokenType
{
    NUMBER,
    IDEN,
    PLUS,
    MINUS,
    MULTIPLE,
    DIVIDE,
    NEGATIVE,
    EQUAL,
    LEFT,
    RIGHT,
    GREATER,
    LESS,
    POWER
};

class token
{
private:
    tokenType type;
    int number;

    // sort: operator or identifier?
    QChar op;
    // identifier name
    QString identifier;
    // string value
    QString str_iden;

public:
    token() {}
    token(tokenType currenttype, int num) : type(currenttype), number(num) {}
    token(tokenType currenttype, QString str) : type(currenttype), identifier(str) {}
    token(tokenType currenttype, QChar str) : type(currenttype), op(str) {}
    tokenType getType() { return type; }
    int getNum() { return number; }
    QString getIdenifier();
    QString getOp();
};

// tool class.
class tokenizer
{
public:
    tokenizer() {}
    QList<token *> tokenize(QStringList command);
};

#endif // TOKENIZER_H
