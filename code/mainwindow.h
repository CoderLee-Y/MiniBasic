#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QEventLoop>
#include <QTextBlock>
#include <QTextCursor>
#include <qtextstream.h>
#include <QTextEdit>
#include "program.h"

class GOTOerror{};
#define NORMALMODE 0
#define DEBUGMODE 1
struct color_line{
    QTextCursor cursor;
    int line_number;

    color_line(QTextCursor c,int l):cursor(c),line_number(l){}
    color_line(){}
};

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showAll();
    void processCode(QString tar);
    void showInfo();
    void showVar();

private slots:
    void on_inputData_editingFinished();
    void on_loadButton_2_clicked();
    void on_runbutton_clicked();
    void on_clearButton_clicked();
    int run_command(int currentLine,QStringList command,QString originCommand);
    void on_debug_clicked();
    void add_color_line(QMap<int,QString>::iterator,int color,int line_num);

private:
    Ui::MainWindow *ui;
    program ProgramCounter;
    QMap<int, QString> code;
    int mode;
    QMap<int,QString>::iterator debug_iter;

    // important global varibles:
    QList<color_line> red_line;
    color_line green_line;
    QList<QTextEdit::ExtraSelection> extra;
};
#endif // MAINWINDOW_H
