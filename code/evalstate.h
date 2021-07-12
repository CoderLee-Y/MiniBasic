#ifndef EVALSTATE_H
#define EVALSTATE_H

#include "exp.h"
#include <bits/stdc++.h>

using namespace std;

/*
 * Class: EvaluationContext
 * ------------------------
 * This class encapsulates the information that the evaluator needs to
 * know in order to evaluate an expression.
 */

class EvaluationContext
{

public:
    void setValue(std::string var, int value);
    void setValue(std::string var, string value);
    int getValue(std::string var);
    string getStringValue(std::string var);
    bool isDefined(std::string var);
    bool isStringVar(std::string var);
    void clear(){symbolTable.clear();stringTable.clear();}

    map<std::string, int> symbolTable;
    map<string,string> stringTable;
};

#endif // EVALSTATE_H
