#ifndef PARSER_H
#define PARSER_H
#include "exp.h"
#include "tokenizer.h"
#include <QStack>

class parser
{
private:
    QStack<Expression *> oprand;
    QStack<token> oprator;
    Expression *rightOprand, *leftOprand;
    Expression *ret, *tmp;

public:
    parser();
    Expression *parse(QList<token *> cmd);
    void simplify(token currentOp);
};

#endif // PARSER_H
