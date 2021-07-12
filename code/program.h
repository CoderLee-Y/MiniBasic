#ifndef PROGRAM_H
#define PROGRAM_H

#include <QString>
#include <QStringList>
#include "statement.h"
#include "tokenizer.h"

// error exception
class COMMANDerror{};
class PRINTFerror{};
class STRINGSUPPROTerror{};
class program
{
private:
    tokenizer Thistoken;
    statement *statementWords;

public:
    bool ISerror;
    bool WAIT_UPDATE;
    bool WAIT_INPUT;
    bool WAIT_INPUTS;
    bool HALT;
    QString NewResult;
    int GOTOline;

    // tree: string list
    QStringList tree;
    QString varToInput;
    EvaluationContext context;

public:
    program();
    void clear();
    void parse(QStringList command,QString origin);
    void run(QStringList command,QString origin_command);

    QString string_handler(QString origin,int &succ);
    QList<QString> printf_handler(QString origin);
};

#endif // PROGRAM_H
