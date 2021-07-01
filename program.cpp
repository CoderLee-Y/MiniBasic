#include "program.h"

program::program()
{
    ISerror = false;
    WAIT_INPUT = false;
    WAIT_INPUTS = false;
    WAIT_UPDATE = false;
    HALT = false;
    varToInput = "";
    GOTOline = -1;
    NewResult = "";
    tree.clear();
    context.clear();
}

// 需要保证command 长度至少为2
void program::parse(QStringList command, QString origin)
{
    tree.clear();

    // 首先生成行号与命令名称
    QString PreTree = "";
    PreTree.push_back(command[0]);
    PreTree.push_back(" ");
    PreTree.push_back(command[1]);

    command.removeFirst();
    QString type = command[0].simplified();
    type = type.toUpper();
    command.removeFirst();

    //若为注释，在第二行缩进全部打出
    if (type == "REM")
    {
        tree.push_back(PreTree);
        QString rem = "";
        rem.push_back('\t');
        for (int i = 0; i < command.length(); ++i)
        {
            rem.push_back(command[i]);
            rem.push_back(" ");
        }
        tree.push_back(rem);
        return;
    }

    QList<token *> TokenList;
    try
    {
        if (type == "LET")
        {
            // possible string definition
            int cursor = 0;
            // if cursor=-1, no string exists
            // else cut qstring;
            QString str = string_handler(origin, cursor);
            if (cursor != -1)
            {
                origin = origin.left(cursor);
                // 30 LET A="DJ"
                command = origin.simplified().split(' ');

                command.removeFirst();
                command.removeFirst();
                // command="A="/"A""="
                TokenList = Thistoken.tokenize(command);

                QString str_iden = TokenList[0]->getIdenifier();

                if (TokenList[1]->getOp() != '=')
                    throw LETerror();

                // [LINE] LET =
                PreTree.append(" =");
                tree.push_back(PreTree);

                str_iden.prepend("\t");
                tree.push_back(str_iden);

                str.prepend("\t\"");
                str.append('"');
                tree.push_back(str);

                return;
            }

            TokenList = Thistoken.tokenize(command);

            statementWords = new LetStat(TokenList);
            QStringList letTree = statementWords->treeGenerator();

            // [LINE] LET =
            PreTree.append(letTree[0]);
            tree.push_back(PreTree);

            for (int i = 1; i < letTree.length(); ++i)
            {
                letTree[i].remove(0, 1);
                tree.push_back(letTree[i]);
            }
            delete statementWords;
            return;
        }

        if (type == "INPUT" || type == "INPUTS")
        {
            if (command.length() > 1)
                throw INPUTerror();

            TokenList = Thistoken.tokenize(command);
            if (TokenList[0]->getType() != IDEN || TokenList.length() > 1)
                throw INPUTerror();

            tree.push_back(PreTree);
            tree.push_back(command[0]);
            tree.back().prepend("\t");
            return;
        }

        if (type == "PRINT")
        {
            tree.push_back(PreTree);

            TokenList = Thistoken.tokenize(command);
            statementWords = new PrintStat(TokenList);

            QStringList printTree = statementWords->treeGenerator();
            tree << printTree;
            delete statementWords;
            return;
        }

        if (type == "PRINTF")
        {
            tree.push_back(PreTree);
            // format print
            QList<QString> strlist= printf_handler(origin);
            for(int i=0;i<strlist.length();++i)
            {
                tree.append(strlist[i]);
                tree.back().prepend('\t');
            }
            return;
        }

        if (type == "GOTO")
        {
            tree.push_back(PreTree);

            TokenList = Thistoken.tokenize(command);
            statementWords = new GotoStat(TokenList);

            QStringList gotoTree = statementWords->treeGenerator();
            tree << (gotoTree);
            return;
        }

        if (type == "IF")
        {
            tree.push_back(PreTree);

            QStringList ifEXP, thenEXP = command;

            while (thenEXP.front().simplified().toUpper() != "THEN")
            {
                ifEXP << (thenEXP.front().simplified());
                thenEXP.removeFirst();
                if (thenEXP.empty())
                    throw IFerror();
            }

            if (thenEXP.length() <= 1 || thenEXP.front() != "THEN")
                throw IFerror();

            thenEXP.removeFirst();

            QList<token *> thenList;
            TokenList = Thistoken.tokenize(ifEXP);
            thenList = Thistoken.tokenize(thenEXP);

            statementWords = new IfStat(TokenList, thenList);

            QStringList IFList = statementWords->treeGenerator();
            tree << IFList;

            QString tarLine = QString::number(thenList[0]->getNum());
            tarLine.push_front('\t');
            tree << tarLine;

            return;
        }

        if (type == "END")
        {
            tree.push_back(PreTree);
            return;
        }

        throw COMMANDerror();
    }
    catch (...)
    {
        tree.clear();
        tree.push_back("Failed to parse: Error command.");
    }
}

// usage: never update tree, only execute command.
// usage: if no line number, just push front "DIRECT"
void program::run(QStringList command, QString origin_command)
{
    // "DIRECT" "PRINT" "A"
    // directly run
    bool isDirectRun = false;
    if (command[0] == "DIRECT")
        isDirectRun = true;

    command.removeFirst();

    QString type = command[0].simplified();
    type = type.toUpper();
    command.removeFirst();

    QList<token *> TokenList;

    if (type == "LET")
    {
        // possible string definition
        int cursor = 0;
        // if cursor=-1, no string exists
        // else cut qstring;
        QString str = string_handler(origin_command, cursor);
        if (cursor != -1)
        {
            origin_command = origin_command.left(cursor);
            // 30 LET A="DJ"
            command = origin_command.simplified().split(' ');

            command.removeFirst();
            command.removeFirst();
            // command="A="/"A""="
            TokenList = Thistoken.tokenize(command);

            QString str_iden = TokenList[0]->getIdenifier();
            if (TokenList[1]->getOp() != '=')
                throw LETerror();
            context.setValue(str_iden.toStdString(), str.toStdString());
            return;
        }

        TokenList = Thistoken.tokenize(command);
        statementWords = new LetStat(TokenList);

        statementWords->execute(context);
        delete statementWords;
        return;
    }

    if (type == "INPUT" || type == "INPUTS")
    {
        if (type == "INPUT")
            WAIT_INPUT = true;
        else
            WAIT_INPUTS = true;

        TokenList = Thistoken.tokenize(command);
        if (TokenList[0]->getType() != IDEN || TokenList.length() > 1)
            throw INPUTerror();

        if (command.length() > 1)
            throw INPUTerror();

        varToInput = command[0].simplified();
        return;
    }

    if (type == "PRINT")
    {
        TokenList = Thistoken.tokenize(command);
        QString iden = TokenList[0]->getIdenifier();
        if (context.isStringVar(iden.toStdString()))
        {
            NewResult = QString::fromStdString(context.getStringValue(iden.toStdString()));
            WAIT_UPDATE = true;
            return;
        }

        statementWords = new PrintStat(TokenList);
        statementWords->execute(context);
        NewResult = QString::number(statementWords->getVal());
        WAIT_UPDATE = true;
        delete statementWords;
        return;
    }

    if (type == "PRINTF")
    {
        // TODO: print to new result
        // list 3 parts:
        // 1. number
        // 2. "" begins and ends string
        // 3. string without starting with " -> varibles

        WAIT_UPDATE = true;
        QList<QString> strlist= printf_handler(origin_command);

        QString output="";
        for(int i=0;i<strlist.length();++i)
        {
            if(strlist[i][0]=="\"")
            {
                strlist[i].remove(0,1);
                strlist[i].remove(strlist[i].length()-1,1);

                output.append(strlist[i]);
            }
            else if(strlist[i][0]>='0'&&strlist[i][0]<='9')
                output+=strlist[i];
            else
            {
                if(context.isStringVar(strlist[i].toStdString()))
                    output+=QString::fromStdString(context.getStringValue(strlist[i].toStdString()));
                else if(context.isDefined(strlist[i].toStdString()))
                    output+=QString::number(context.getValue(strlist[i].toStdString()));
                else
                    throw NOIDEN();
            }
        }
        NewResult=output;
        return;
    }

    // only accept 3 commands
    if (isDirectRun)
        throw COMMANDerror();

    if (type == "REM")
        return;

    if (type == "GOTO")
    {
        TokenList = Thistoken.tokenize(command);

        statementWords = new GotoStat(TokenList);
        statementWords->execute(context);
        GOTOline = statementWords->getTar();

        delete statementWords;
        return;
    }

    if (type == "IF")
    {
        QStringList ifEXP, thenEXP = command;

        while (thenEXP.front().simplified().toUpper() != "THEN")
        {
            ifEXP << (thenEXP.front().simplified());
            thenEXP.removeFirst();
            if (thenEXP.empty())
                throw IFerror();
        }

        if (thenEXP.length() <= 1 || thenEXP.front() != "THEN")
            throw IFerror();

        thenEXP.removeFirst();
        QList<token *> thenList;


        TokenList = Thistoken.tokenize(ifEXP);
        thenList = Thistoken.tokenize(thenEXP);

        // NO STRING!
        for(auto a:thenList)
        {
            if(a->getType()==IDEN)
                if(context.isStringVar(a->getIdenifier().toStdString()))
                    throw STRINGSUPPROTerror();
        }
        for(auto a:TokenList)
        {
            if(a->getType()==IDEN)
                if(context.isStringVar(a->getIdenifier().toStdString()))
                    throw STRINGSUPPROTerror();
        }

        statementWords = new IfStat(TokenList, thenList);
        statementWords->execute(context);

        if (statementWords->isTrue() &&
            thenList.length() == 1 &&
            thenList[0]->getType() == NUMBER)
            GOTOline = thenList[0]->getNum();

        return;
    }

    if (type == "END")
    {
        HALT = true;
        return;
    }
    throw COMMANDerror();
}

void program::clear()
{
    ISerror = false;
    WAIT_INPUT = false;
    WAIT_INPUTS = false;
    WAIT_UPDATE = false;
    HALT = false;
    varToInput = "";
    GOTOline = -1;
    NewResult = "";
    tree.clear();
    context.clear();
}

QString program::string_handler(QString origin, int &succ)
{
    int pos = 0;
    origin = origin.simplified();
    while (pos < origin.length())
    {
        if (origin[pos] == '"' || origin[pos] == '\'')
        {
            succ = pos;
            QChar termination = origin[pos++];
            QString str = "";

            while (pos < origin.length() && origin[pos] != termination)
            {
                if (origin[pos] == '"' || origin[pos] == '\'')
                    throw stringERROR();
                str += origin[pos++];
            }

            if (pos != origin.length() - 1)
                throw stringERROR();
            return str;
        }
        ++pos;
    }
    succ = -1;
    return "";
}

QList<QString> program::printf_handler(QString origin)
{
    // origin: [LINE] PRINTF "{}{}ABC",12,10
    origin = origin.simplified();

    while(origin.front()>='0'&&origin.front()<='9')
        origin.remove(0,1);

    while(origin.front()==' ')
        origin.remove(0,1);


    if (!origin.startsWith("PRINTF "))
        throw PRINTFerror();
    origin.remove(0, 7);

    // origin: "{}BC{}",xx,xx
    QChar termination;
    if(origin[0]=='"'||origin[0]=='\'')
        termination=origin[0];

    QList<QString> ret;
    QString str = "";

    int cursor = 1, expect_para = 0,factual_para=0;

    // handle {} before.
    while (cursor < origin.length())
    {
        if (origin[cursor] == '{')
        {
            ++cursor;
            if (origin[cursor++] != '}')
                throw PRINTFerror();
            ++expect_para;
            ret.push_back("");
        }
        else if(origin[cursor]==termination)
            break;
        else
        {
            while(origin[cursor]!=termination && origin[cursor]!="{")
            {
                str.append(origin[cursor++]);
                if(str.back()=='"'||str.back()=='\'')
                    throw stringERROR();
            }
            str.prepend("\"");
            str.append("\"");
            ret.push_back(str);
            str.clear();
        }
    }

    while(origin[++cursor]==' ');

    if(expect_para>0 && origin[cursor]!=',')
        throw PRINTFerror();

    // cursor in ','
    while (cursor < origin.length())
    {
        QString str = "";
        while(origin[cursor]==' ')
            ++cursor;

        if (origin[cursor++] != ',')
            throw PRINTFerror();
        while(origin[cursor]==' ')
            ++cursor;
        ++factual_para;

        // num
        if (origin[cursor] >= '0' && origin[cursor] <= '9')
        {
            while (cursor < origin.length() && origin[cursor] != ',')
            {
                if (!(origin[cursor] >= '0' && origin[cursor] <= '9'))
                    throw PRINTFerror();
                str.append(origin[cursor++]);
            }
            int index=ret.indexOf("");
            if(index==-1)throw PRINTFerror();
            ret.replace(index,str);
            str.clear();
        }

        // str
        else if(origin[cursor]=="\""||origin[cursor]=="'")
        {
            QChar termination=origin[cursor++];
            str.append(termination);

            while (cursor < origin.length() && origin[cursor] != termination)
            {
                if (origin[cursor]=="\""||origin[cursor]=="'")
                    throw PRINTFerror();
                str.append(origin[cursor++]);
            }

            if(cursor==origin.length()&&origin[cursor-1]!=termination)
                throw stringERROR();

            str.append(termination);
            int index=ret.indexOf("");
            if(index==-1)throw PRINTFerror();
            ret.replace(index,str);

            // from termination to next possible ','
            ++cursor;
            str.clear();
        }

        // varibles
        else if(origin[cursor].toLower()>='a'&&origin.toLower()<='z')
        {
            while (cursor < origin.length() && origin[cursor] != ',')
                str.append(origin[cursor++]);

            int index=ret.indexOf("");
            if(index==-1)throw PRINTFerror();
            ret.replace(index,str);
            str.clear();
        }
    }

    if (factual_para != expect_para)
        throw PRINTFerror();
    return ret;
}
