#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon("icon.png"));
    ui->setupUi(this);
    this->setWindowTitle("MiniBasic");
    ui->inputData->setPlaceholderText("input your code here.");
    ui->inputData->setFocus();
    mode = NORMALMODE;
}

MainWindow::~MainWindow()
{
    ProgramCounter.clear();
    return;
}

void MainWindow::on_loadButton_2_clicked()
{
    on_clearButton_clicked();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Choose a code file"));
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setGeometry(10, 30, 300, 200);
    fileDialog->setDirectory("./");
    if (fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::about(NULL, "error", "FILE OPEN ERROR");
            return;
        }
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            line = line.simplified();
            QStringList splitLine = line.split(' ');
            if (splitLine[0].front() >= '0' && splitLine[0].front() <= '9')
            {
                int lineNum = splitLine[0].toInt();
                code.insert(lineNum, line);
            }
        }
        file.close();
        showAll();
        return;
    }
    QMessageBox::about(NULL, "error", "FILE OPEN ERROR");
    return;
}

void MainWindow::on_runbutton_clicked()
{
    showAll();
    QMap<int, QString>::iterator iter = code.begin();
    QMap<int, QString>::iterator iter_backup = code.begin();

    if(mode==DEBUGMODE)
    {
        mode = NORMALMODE;
        ui->loadButton_2->setEnabled(true);
        ui->clearButton->setEnabled(true);
        iter=debug_iter;
        iter_backup=debug_iter;
        debug_iter=code.begin();
    }

    QStringList SplitedCommand;
    int currentLine;

    // parse to tree first.
    ui->showTree->clear();

    // 下面对语法错误进行高亮显示
    // 每次都要先将red line清掉
    // 0 green 1 red
    red_line.clear();

    while (iter != code.end())
    {
        SplitedCommand = iter.value().split(' ');
        ProgramCounter.parse(SplitedCommand, iter.value());
        for (int i = 0; i < ProgramCounter.tree.length(); ++i)
            ui->showTree->append(ProgramCounter.tree[i]);

        if (ProgramCounter.tree[0] == "Failed to parse: Error command.")
            add_color_line(iter,1,iter.key());
        ++iter;
    }

    iter = iter_backup;

    // execute then
    while (iter != code.end())
    {
        currentLine = iter.key();
        SplitedCommand = iter.value().split(' ');

        int result = run_command(currentLine, SplitedCommand, iter.value());

        switch (result)
        {
        case -1:
        case -2:
            return;
        case -3:
            ++iter;
            break;
        default:
            iter = code.find(result);
            break;
        }
    }
    return;
}

void MainWindow::on_clearButton_clicked()
{
    ui->currentVarible_2->clear();
    ui->showCode->clear();
    ui->showTree->clear();
    ui->showResult->clear();
    ui->currentVarible_2->clear();
    ProgramCounter.clear();
    red_line.clear();
    extra.clear();
    mode=NORMALMODE;
    code.clear();
}

void MainWindow::on_inputData_editingFinished()
{
    QString input = ui->inputData->text();

    ui->inputData->clear();
    input = input.trimmed();

    if (input.front() <= '9' && input.front() >= '0')
        // line number begins
        processCode(input);
    else
    {
        QStringList splitedLine = input.split(' ');
        if(ProgramCounter.WAIT_INPUTS==1)
        {
            if (input[0] == '?')
            {
                input = input.remove(0, 1);
                ProgramCounter.context.setValue(ProgramCounter.varToInput.toStdString(),
                                                    input.toStdString());
                return;
            }
            else
                throw INPUTerror();
        }
        if (splitedLine.length() == 1)
        {
            {
                if (input == "RUN")
                {
                    this->on_runbutton_clicked();
                    return;
                }
                if (input == "LOAD")
                {
                    this->on_loadButton_2_clicked();
                    return;
                }
                if (input == "LIST")
                {
                    this->showAll();
                    return;
                }
                if (input == "CLEAR")
                {
                    this->on_clearButton_clicked();
                    return;
                }
                if (input == "HELP")
                {
                    this->showInfo();
                    return;
                }
                if (input == "QUIT")
                {
                    this->~MainWindow();
                    return;
                }
                if (input[0] == '?')
                {
                    input = input.remove(0, 1);
                    if (ProgramCounter.WAIT_INPUT)
                        ProgramCounter.context.setValue(ProgramCounter.varToInput.toStdString(),
                                                        input.toInt());
                    else if (ProgramCounter.WAIT_INPUTS)
                        ProgramCounter.context.setValue(ProgramCounter.varToInput.toStdString(),
                                                        input.toStdString());
                    else
                        throw COMMANDerror();
                    //ui->showResult->append(QString::number(ProgramCounter.context.getValue(ProgramCounter.varToInput.toStdString())));
                    return;
                }
            }
        }
        else
        {
            splitedLine.push_front("DERECT");
            run_command(-1, splitedLine, input);
        }
    }
}

// return: END MEET: -2
// return: ERROR occur: -1
// return: all right: -3
// return: GOTO LINE: non-negative
int MainWindow::run_command(int currentLine, QStringList command, QString origin_command)
{
    try
    {
        if (ProgramCounter.GOTOline == -1)
        {
            ProgramCounter.run(command, origin_command);
            showVar();
            if (ProgramCounter.WAIT_INPUT == true || ProgramCounter.WAIT_INPUTS == true)
            {
                QString inputNum;
                ui->inputData->clear();
                ui->inputData->setText("?");
                ui->inputData->setFocus();
                    ui->debug->setEnabled(false);
                    ui->runbutton->setEnabled(false);
                    ui->loadButton_2->setEnabled(false);
                QEventLoop loop;
                connect(ui->inputData, SIGNAL(editingFinished()), &loop, SLOT(quit()));
                connect(ui->inputData, SIGNAL(on_clearButton_clicked()), &loop, SLOT(quit()));
                loop.exec();
                ProgramCounter.WAIT_INPUT = false;
                ProgramCounter.WAIT_INPUTS = false;
                    ui->debug->setEnabled(true);
                    ui->runbutton->setEnabled(true);
                if(mode!=DEBUGMODE)
                {
                    ui->loadButton_2->setEnabled(true);
                }
                showVar();
                return -3;
            }
            if (ProgramCounter.WAIT_UPDATE == true)
            {
                ui->showResult->append(ProgramCounter.NewResult);
                ProgramCounter.WAIT_UPDATE = false;
                showVar();
                return -3;
            }
            if (ProgramCounter.HALT == true)
            {
                ProgramCounter.HALT=false;
                ProgramCounter.ISerror = false;
                ProgramCounter.WAIT_INPUT = false;
                ProgramCounter.WAIT_INPUTS = false;
                ProgramCounter.WAIT_UPDATE = false;
                ProgramCounter.HALT = false;
                ProgramCounter.varToInput = "";
                ProgramCounter.GOTOline = -1;
                ProgramCounter.NewResult = "";
                return -2;
            }
        }
        else
        {
            if (code.count(ProgramCounter.GOTOline) != 0)
            {
                int ret = ProgramCounter.GOTOline;
                ProgramCounter.GOTOline = -1;
                return ret;
            }
            else
                throw GOTOerror();
        }
        return -3;
    }
    catch (NOIDEN)
    {
        QString error = "error(01): some variable not defined.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (NOOP)
    {
        QString error = "error(02):Illegal operator occurs.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (DIVIDEZERO)
    {
        QString error = "error(03):Divided by zero.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (GOTOerror)
    {
        QString error = "error(04):a goto statement points to an inexistent line.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (COMMANDerror)
    {
        QString error = "error(05):Invalid command.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (tokenERROR)
    {
        QString error = "error(06):Invalid tokens.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (IFerror)
    {
        QString error = "error(07):IF statement error occurs.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (LETerror)
    {
        QString error = "error(08):LET statement error occurs.\nerror line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (INPUTerror)
    {
        QString error = "error(09):INPUT statement error occurs.\n error line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (stringERROR)
    {
        QString error = "error(10):string error occurs(may expect \" or ').\n";
        error += "error line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch (PRINTFerror)
    {
        QString error = "error(11):printf error occurs, PRINTF {} <-> one parameter.\n";
        error += "error line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
    catch(STRINGSUPPROTerror)
    {
        QString error = "error(12):String only support print & let & inputs\n";
        error += "error line number:  ";
        error += currentLine == -1 ? "current line" : QString::number(currentLine);
        QMessageBox::about(NULL, "error", error);
        return -1;
    }
}

void MainWindow::showAll()
{
    showVar();
    ui->showCode->clear();
    QMap<int, QString>::iterator iter = code.begin();
    for (; iter != code.end(); ++iter)
    {
        ui->showCode->append(iter.value());
    }
}

void MainWindow::processCode(QString tar)
{
    tar = tar.simplified();
    QStringList splitLine = tar.split(' ');
    if (splitLine[0].front() >= '0' && splitLine[0].front() <= '9')
    {
        int lineNum = splitLine[0].toInt();
        if (splitLine.length() != 1)
        {
            if (code.count(lineNum))
                code[lineNum] = tar;
            else
                code.insert(lineNum, tar);
            showAll();
        }
        else
        {
            // delete code.
            code.remove(lineNum);

            showAll();
        }
    }
}

void MainWindow::showInfo()
{
    QMessageBox::about(NULL, "About Minibasic", "Minibasic Version 2.0.\nFinished by Li Yiyan, whose ID is 519021911103.");
}

void MainWindow::on_debug_clicked()
{
    QStringList SplitedCommand;

    if (mode == NORMALMODE)
    {
        mode = DEBUGMODE;
        ui->loadButton_2->setEnabled(false);
        ui->clearButton->setEnabled(false);
        ProgramCounter.HALT=false;
        ProgramCounter.ISerror = false;
        ProgramCounter.WAIT_INPUT = false;
        ProgramCounter.WAIT_INPUTS = false;
        ProgramCounter.WAIT_UPDATE = false;
        ProgramCounter.HALT = false;
        ProgramCounter.varToInput = "";
        ProgramCounter.GOTOline = -1;
        ProgramCounter.NewResult = "";
        ProgramCounter.tree.clear();
        ProgramCounter.context.clear();
        ui->showTree->clear();
        ui->showResult->clear();
        ui->currentVarible_2->clear();

        debug_iter = code.begin();

        // parse to tree first.
        ui->showTree->clear();

        SplitedCommand = debug_iter.value().split(' ');
        ProgramCounter.parse(SplitedCommand, debug_iter.value());

        add_color_line(debug_iter,0,debug_iter.key());
        if (ProgramCounter.tree[0] == "Failed to parse: Error command.")
            add_color_line(debug_iter,1,debug_iter.key());

        for (int i = 0; i < ProgramCounter.tree.length(); ++i)
            ui->showTree->append(ProgramCounter.tree[i]);
    }
    else if (mode == DEBUGMODE)
    {
        add_color_line(debug_iter,0,debug_iter.key());
        int result = run_command(debug_iter.key(),
                                 debug_iter.value().simplified().split(' '),
                                 debug_iter.value());
        showVar();
        switch (result)
        {
        case -1:
        {
            mode = NORMALMODE;
            add_color_line(debug_iter,1,debug_iter.key());
            ui->loadButton_2->setEnabled(true);
            ui->clearButton->setEnabled(true);
            QString error = "Program EXITS unexpectedly.\n";
            QMessageBox::about(NULL, "error", error);
            return;
        }
        case -2:
            break;
        case -3:
            ++debug_iter;
            add_color_line(debug_iter,0,debug_iter.key());
            break;
        default:
            debug_iter = code.find(result);
            add_color_line(debug_iter,0,debug_iter.key());
            break;
        }

        if (debug_iter == code.end() || result == -2)
        {
            mode = NORMALMODE;
            ui->loadButton_2->setEnabled(true);
            ui->clearButton->setEnabled(true);
            QString info = "Program Meets END Normally\n";
            debug_iter=code.begin();
            QMessageBox::about(NULL, "END", info);
            return;
        }

        // parse to tree first.
        ui->showTree->clear();
        SplitedCommand = debug_iter.value().split(' ');


        ProgramCounter.parse(SplitedCommand, debug_iter.value());
        for (int i = 0; i < ProgramCounter.tree.length(); ++i)
            ui->showTree->append(ProgramCounter.tree[i]);

        add_color_line(debug_iter,0,debug_iter.key());
        if (ProgramCounter.tree[0] == "Failed to parse: Error command.")
            add_color_line(debug_iter,1,debug_iter.key());
    }
}

void MainWindow::showVar()
{
    map<string,int>::iterator iter1=
            ProgramCounter.context.symbolTable.begin();
    map<string,string>::iterator iter2=
            ProgramCounter.context.stringTable.begin();

    ui->currentVarible_2->clear();
    QString str="";
    while(iter1!=ProgramCounter.context.symbolTable.end())
    {
        str+=QString::fromStdString(iter1->first);
        str+=" #TYPE(INT)# VALUE: ";
        str+=QString::number(iter1->second);
        ui->currentVarible_2->append(str);
        str.clear();
        iter1++;
    }
    while(iter2!=ProgramCounter.context.stringTable.end())
    {
        str+=QString::fromStdString(iter2->first);
        str+=" #TYPE(STRING)# VALUE: ";
        str+=QString::fromStdString(iter2->second);
        ui->currentVarible_2->append(str);
        str.clear();
        iter2++;
    }
}

void MainWindow::add_color_line(QMap<int,QString>::iterator iter,int color,int line_num)
{
    QList<QColor> color_list = {
        QColor(100, 255, 100), // green
        QColor(255, 100, 100),  // red
        QColor(255,255,255) // origin
    };

    ui->showCode->setTextBackgroundColor(color_list[2]);
    extra.clear();

    int index=0;
    while(iter--!=code.begin())
        ++index;

    QTextBlock line_block=ui->showCode->document()->findBlockByLineNumber(index);

    if(color==0&&mode==DEBUGMODE)
    {
        green_line.cursor=QTextCursor(line_block);
        green_line.line_number=line_num;
        QTextEdit::ExtraSelection tmp;
        tmp.cursor = green_line.cursor;
        tmp.format.setProperty(QTextFormat::FullWidthSelection, true);
        tmp.format.setBackground(color_list[color]);
        extra.append(tmp);

    }
    else if(color==1)
    {
        QTextCursor plot=QTextCursor(line_block);
        color_line cl(plot,line_num);
        red_line.push_back(cl);
        for (auto line : red_line)
        {
            QTextEdit::ExtraSelection tmp;
            tmp.cursor = line.cursor;
            tmp.format.setProperty(QTextFormat::FullWidthSelection, true);
            tmp.format.setBackground(color_list[color]);
            extra.append(tmp);
        }
    }

    ui->showCode->setExtraSelections(extra);
}
