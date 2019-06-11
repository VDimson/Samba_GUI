#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTreeWidgetItem>
#include <QStringList>
#include <adduser.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
void StdOutProcessResurs1();
void StdOutProcess();
void StdOutProcess2();
void StdOutProcess3();
void StdOutProcessDel();
void ReceievDataUser(QStringList spisok);
private:
void InsertUser();//вставляет сортируя данные в таблицу главной формы
void InsertUserResurs();//вставляет данные в таблицу ресурсы-вторая закладка формы
void GetUser();//читает данные из выхлопа консольных команд и формирует списки SpisokUserSamba и SpisokUserSys
void SetFullNameUser(QString login,QString fullName); //запускает процесс редактирования поля полное имя черз pdbedit -f -u=
void SetPassword(QString login,QString password);//запускает процесс установки пароля pdbedit -t -u=

private slots:
    void on_pushButton_clicked();

    void on_actionSamba_triggered();

    void on_twg_itemClicked(QTreeWidgetItem *item, int column);

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_triggered();



    void on_action_6_triggered();

    void on_action_2_triggered();



    void on_action_vers_triggered();

    void on_tabWidget_currentChanged(int index);





    void on_twg_resurs_itemClicked(QTreeWidgetItem *item, int column);

    void on_action_dell_pid_triggered();

    void on_radioButton_user_clicked();

    void on_radioButton_net_clicked();

    void on_radioButtonre_resurs_clicked();


private:
    Ui::MainWindow *ui;
    QProcess* m_process;
    QProcess* m_process2;
    QProcess* m_process3;
    QProcess* m_process_del;
    QProcess* m_process_resurs1;
    QStringList SpisokUserSamba;
    QStringList SpisokUserSys;
    QStringList SpisokUserResurs;
    QString flagLogin;
    QString flagResurs="";
    bool flag;
AddUser *addUs;
QTreeWidgetItem* user_twg_resurs ;
QTreeWidgetItem* glav_user ;
QTreeWidgetItem* glav_resurs ;
QTreeWidgetItem* all_resurs ;



};

#endif // MAINWINDOW_H
