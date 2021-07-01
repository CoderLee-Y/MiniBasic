#include "evalstate.h"

void EvaluationContext::setValue(std::string var, int value)
{
    if (symbolTable.count(var))
        symbolTable[var] = value;
    else
        symbolTable.insert(make_pair(var, value));

    // same varible name is not allowed.
    if(stringTable.count(var))
        stringTable.erase(var);
}

void EvaluationContext::setValue(std::string var, string value)
{
    if (stringTable.count(var))
        stringTable[var] = value;
    else
        stringTable.insert(make_pair(var, value));

    // same varible name is not allowed.
    if(symbolTable.count(var))
        symbolTable.erase(var);
}

int EvaluationContext::getValue(std::string var)
{
    if (symbolTable.count(var))
        return symbolTable[var];
    return -1;
}

string EvaluationContext::getStringValue(std::string var)
{
    if(stringTable.count(var))
        return stringTable[var];
    return "";
}

bool EvaluationContext::isDefined(std::string var)
{
    return symbolTable.count(var);
}

bool EvaluationContext::isStringVar(string var)
{
    return stringTable.count(var);
}
