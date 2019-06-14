#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QList"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_process=new QProcess(this);
    m_process2=new QProcess(this);
    m_process3=new QProcess(this);
    m_process_del=new QProcess(this);
    m_process_resurs1=new QProcess(this);
    connect(m_process,SIGNAL(readyReadStandardOutput()),SLOT(StdOutProcess()));
    connect(m_process2,SIGNAL(readyReadStandardOutput()),SLOT(StdOutProcess2()));
    connect(m_process3,SIGNAL(readyReadStandardOutput()),SLOT(StdOutProcess3()));
    connect(m_process_del,SIGNAL(readyReadStandardOutput()),SLOT(StdOutProcessDel()));
    connect(m_process_resurs1,SIGNAL(readyReadStandardOutput()),SLOT(StdOutProcessResurs1()));
    QStringList lst;
    lst << "Пользователи" << "Логин" << "UID" << "Полное имя";
    ui->twg->setHeaderLabels(lst);
    ui->twg->setColumnWidth(0,120);
    ui->twg->setColumnWidth(1,120);
ui->actionSamba->setDisabled(true);
ui->action->setDisabled(true);
ui->action_2->setDisabled(true);
ui->action_6->setDisabled(true);

ui->twg_resurs->setColumnWidth(2,120);
ui->twg_resurs->setColumnWidth(3,170);
ui->twg_resurs_2->setColumnWidth(3,145);
ui->twg_resurs->header()->setVisible(true);
ui->twg_resurs_2->header()->setVisible(true);
int defaul_zakladka=ui->tabWidget->currentIndex();//получаем номер закладки на форме по умолчанию
on_tabWidget_currentChanged(defaul_zakladka);//функция настравиает форму в зависимости от активной закладки формы
ui->radioButton_net->setChecked(true);
//настройка автосортировки в таблицах по колонкам!
ui->twg_resurs_2->header()->sortIndicatorOrder();
ui->twg_resurs_2->sortByColumn(0,Qt::AscendingOrder);
ui->twg_resurs_2->setSortingEnabled(true);

ui->twg_resurs->header()->sortIndicatorOrder();
ui->twg_resurs->sortByColumn(0,Qt::AscendingOrder);
ui->twg_resurs->setSortingEnabled(true);

ui->twg->header()->sortIndicatorOrder();
ui->twg->sortByColumn(0,Qt::AscendingOrder);
ui->twg->setSortingEnabled(true);
 GetUser();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::StdOutProcessResurs1()
{
    QString vihlop=m_process_resurs1->readAllStandardOutput();
    SpisokUserResurs=vihlop.split("\n",QString::SkipEmptyParts);
}
void MainWindow::SetPassword(QString login,QString password)
{

    QString StrPass;
    StrPass=password+"\n";
    const char* pass; //определяем символьную переменную
    pass=StrPass.toStdString().c_str();//перевод строки в символную переменную
    QProcess procces_setPass;
    procces_setPass.start("smbpasswd",QStringList()<<"-s"<<"-U"<< login);
    //procces_setPass.waitForReadyRead();
    //QMessageBox::information(0,"Сообщение1!","1этап");

    procces_setPass.write(pass); //запись в консоль
    procces_setPass.waitForBytesWritten();//ждем пока запишется
    //QMessageBox::information(0,"Сообщение!2",pass);

    //procces_setPass.waitForReadyRead();//читаем ответ
    //QMessageBox::information(0,"Сообщение3!",procces_setPass.readAll());

    //ui->statusBar->showMessage(process_addUser.readAll()); можно вывести
    procces_setPass.write(pass);//шлем повтор пароля
    //QMessageBox::information(0,"Сообщение4!",pass);
    procces_setPass.waitForBytesWritten();//ждем пока запишется
    //QMessageBox::information(0,"Сообщение5!",procces_setPass.readAll());

    procces_setPass.waitForFinished();//ждем ответ

    //QMessageBox::information(0,"Сообщение6!",procces_setPass.readAll());


     if (procces_setPass.exitCode()==0)

    {
        QString mess;
    mess="Пароль установлен успешно!";
    QMessageBox::information(0,"Сообщение!",mess);
  //секция второй части при добавлении -редактирование
    }
    else
    {
    QMessageBox::warning(0,"Внимание!","Произошла ошибка в операции pdbedit -t !!!");
return;
    }
}
void MainWindow::SetFullNameUser(QString login,QString fullName)
{
    QProcess procces_setFullName;
    procces_setFullName.start("pdbedit",QStringList()<<"-f"<< fullName << "-u"<< login);
    procces_setFullName.waitForFinished();
    if (procces_setFullName.exitCode()==0)
    {
        QString mess;
    mess="Установлено полное имя: "+ fullName+" успешно!";
    QMessageBox::information(0,"Сообщение!",mess);
  //секция второй части при добавлении -редактирование
    }
    else
    {
    QMessageBox::warning(0,"Внимание!","Произошла ошибка в операции pdbedit -f !!!");

    }


}
void MainWindow::ReceievDataUser(QStringList spisok)
{
 QString status;
 status=spisok.at(3);
 if (status=="fullname")
 {
 SetFullNameUser(spisok.at(0),spisok.at(1));
  on_action_3_triggered();//кнопка обновить!
  return;
 }
 if (status=="pass")
 {
 SetPassword(spisok.at(0),spisok.at(2));
  return;
 }

if (status=="full")
{
 ui->statusBar->showMessage(spisok.at(0) +"/"+spisok.at(1)+"/"+spisok.at(2));
 QProcess process_addUser(this);

 process_addUser.start("pdbedit",QStringList() << "-a" << spisok.at(0));//консольная команда по созданию ползователя
 process_addUser.waitForReadyRead();//ждем пока прочитается
 //ui->statusBar->showMessage(process_addUser.readAll());//вывод ответа в статусбар
 QString StrPass;
 StrPass=spisok.at(2)+"\n"; //формируем ввод пароля
 const char* pass; //определяем символьную переменную

 pass=StrPass.toStdString().c_str();//перевод строки в символную переменную

 process_addUser.write(pass); //запись в консоль
 process_addUser.waitForBytesWritten();//ждем пока запишется
 process_addUser.waitForReadyRead();//читаем ответ
 //ui->statusBar->showMessage(process_addUser.readAll()); можно вывести
 process_addUser.write(pass);//шлем повтор пароля
 process_addUser.waitForBytesWritten();//ждем пока запишется
 process_addUser.waitForReadyRead();//ждем ответ

 process_addUser.waitForFinished();//ждем окончания процесса

  if (process_addUser.exitCode()==0)
  {
      QString mess;
  mess="Пользователь: "+ spisok.at(0)+" добавлен успешно в БД Samba";
  QMessageBox::information(0,"Сообщение!",mess);

  //секция второй части при добавлении -редактирование
  QString FName;

  FName=spisok.at(1);

  if (!FName.isEmpty())
  {
      SetFullNameUser(spisok.at(0),spisok.at(1));
  }
  }
  else
  {
  QMessageBox::warning(0,"Внимание!","Произошла ошибка в операции pdbedit -a !!!");

  }
  //добавим в таблицу на форме сделаем рефреш!!!
    on_action_3_triggered();//кнопка обновить!
    QList<QTreeWidgetItem*> findItems=ui->twg->findItems(spisok.at(0),Qt::MatchContains | Qt::MatchRecursive,0);
    if (findItems.count()!=0)
    {
    ui->twg->setCurrentItem(findItems.at(0));
    }

}
}
void MainWindow::InsertUser()
{
QStringList SpisokUserSysTransit;

SpisokUserSysTransit.clear();

ui->twg->clear();
QTreeWidgetItem* twgUser = new QTreeWidgetItem(ui->twg);//добавили глваную ветвь !
twgUser->setText(0,"Samba");//главная ветвь
twgUser->setIcon(0,QIcon(":/ikonka/image/user.png"));
for (int i=0; i<SpisokUserSys.count(); ++i)
{
flag=false;
   for (int j=0; j<SpisokUserSamba.count(); ++j)
   {
    QString  Stroka=SpisokUserSamba.at(j);
    QStringList ListItemUserSamba=Stroka.split(":",QString::KeepEmptyParts);

if (SpisokUserSys.at(i) == ListItemUserSamba.at(0))

// QString str1,str2;
// str1=SpisokUserSys.at(i);
// str2=ListItemUserSamba.at(0);
// if (str1.compare(str2)==0)

    {
flag=true;
    QTreeWidgetItem* StrokaTabl=0;
    StrokaTabl=new QTreeWidgetItem(twgUser);
StrokaTabl->setText(0,SpisokUserSys.at(i));
StrokaTabl->setText(1,ListItemUserSamba.at(0));
StrokaTabl->setText(2,ListItemUserSamba.at(1));
StrokaTabl->setText(3,ListItemUserSamba.at(2));
}

}
if (flag==false)
{
SpisokUserSysTransit.append(SpisokUserSys.at(i));
}
}
QTreeWidgetItem* twgUserSys = new QTreeWidgetItem(ui->twg);//добавим  ветвь для системных пользователей !
twgUserSys->setText(0,"Unix");//главная ветвь
twgUserSys->setIcon(0,QIcon(":/ikonka/image/system_config_display.png"));
for (int k=0; k<SpisokUserSysTransit.count(); ++k )
{
    QTreeWidgetItem* StrokaTabl=0;
    StrokaTabl=new QTreeWidgetItem(twgUserSys);
StrokaTabl->setText(0,SpisokUserSysTransit.at(k));

}


}

void MainWindow::InsertUserResurs()
{
ui->twg_resurs_2->clear();
ui->twg_resurs->clear();

    QStringList SpisokItemUserResurs;
    QString Stroka,Stroka1;
    for (int i=3; i<SpisokUserResurs.count();++i)
    {

    Stroka=SpisokUserResurs.at(i);
    SpisokItemUserResurs=Stroka.split(" ",QString::SkipEmptyParts);
    user_twg_resurs = new QTreeWidgetItem(ui->twg_resurs);
    user_twg_resurs->setIcon(0,QIcon(":/ikonka/image/user.png"));
     QString login=SpisokItemUserResurs.at(1);
     login=login.trimmed();//убираем по краея пробелы ,перевод строки и другого говна !
    user_twg_resurs->setText(0,login);//главная ветвь
     QString pid=SpisokItemUserResurs.at(0);
     pid=pid.trimmed();//убираем по краея пробелы ,перевод строки и другого говна !
    user_twg_resurs->setText(1,pid);
     QString NamePC=SpisokItemUserResurs.at(3);
     NamePC=NamePC.trimmed();
    user_twg_resurs->setText(2,NamePC);
    QString ip_addr=SpisokItemUserResurs.at(4);
    ip_addr=ip_addr.trimmed();
    user_twg_resurs->setText(3,ip_addr);



    if (login.isEmpty())
    {
        continue; //если поле путое , то перейти на следующую итерацию, хотя по сути ее не долнжно быть ! Перестраховка
    }
    QProcess proc_Resurs2;
    proc_Resurs2.start("smbstatus",QStringList()<<"-S"<<"-u"<<login);
    proc_Resurs2.waitForFinished();
    Stroka1=proc_Resurs2.readAll();
    QStringList SpisokStroka1=Stroka1.split("\n",QString::SkipEmptyParts);
    for (int j=2; j<SpisokStroka1.count();++j)
    {
        QString ss=SpisokStroka1.at(j);
        QStringList ssList=ss.split(" ",QString::SkipEmptyParts);
        QString ServisFolder=ssList.at(0);
        ServisFolder=ServisFolder.trimmed();
        QTreeWidgetItem* user_twg_resurs2=new QTreeWidgetItem(user_twg_resurs) ;
        user_twg_resurs2->setText(4,ServisFolder);
        QString timeResurs=ssList.at(5)+" "+ssList.at(4)+" "+ssList.at(7)+" "+ssList.at(6);
        user_twg_resurs2->setText(5,timeResurs);

    }
    QProcess proc_Resurs3;
        proc_Resurs3.start("smbstatus",QStringList()<<"-n"<<"-u"<<login);
       //  proc_Resurs3.start("smbstatus",QStringList()<<"-n");
        proc_Resurs3.waitForFinished();
        QString VihlopResurs=proc_Resurs3.readAll();
        QStringList SpisokStrokResurs=VihlopResurs.split("\n",QString::SkipEmptyParts);
        int startInex=0;
        for (int f=0; f<SpisokStrokResurs.count();++f)
        {
            QString ff=SpisokStrokResurs.at(f);
            QStringList ffList=ff.split(" ",QString::SkipEmptyParts);
            QString fff=ffList.at(0);
            fff=fff.trimmed();
            if (fff=="Pid")
            {
                startInex=f+2;
            }
            else
            {continue;}


        }
        if(startInex!=0)
        {



            QTreeWidgetItem* user_twg_resurs3;
            //glav_user=new QTreeWidgetItem(ui->twg_resurs_2);
            flagLogin="";
            //flagResurs="";

        for (int k=startInex; k<SpisokStrokResurs.count();++k)
       {
            QString sss=SpisokStrokResurs.at(k);
            QStringList sssList=sss.split(" ",QString::SkipEmptyParts);


//--------------------------------------------------------------------------------

            if (ui->radioButton_user->isChecked())

  {
                if (flagLogin!=login)
                {
                    glav_user=new QTreeWidgetItem(ui->twg_resurs_2);
                }
    user_twg_resurs3=new QTreeWidgetItem(glav_user) ;
    glav_user->setText(0,login);
    flagLogin=login;

 }
            else
  {

                if (!ui->radioButtonre_resurs->isChecked())
                {
                 user_twg_resurs3=new QTreeWidgetItem(ui->twg_resurs_2) ;
                user_twg_resurs3->setText(0,login);}

  }


            int kolichecnwo=sssList.count();
            QString pathResursa="";

            for (int q=6;q<kolichecnwo-5;++q )
            {
                pathResursa=pathResursa+sssList.at(q)+"/";
            }
            if (ui->radioButtonre_resurs->isChecked())
            {



                    QList<QTreeWidgetItem*> findPathResursa=ui->twg_resurs_2->findItems(pathResursa,Qt::MatchContains |Qt::MatchRecursive,6);
                    if (findPathResursa.count()==0){


                    glav_resurs=new QTreeWidgetItem(ui->twg_resurs_2);
                    user_twg_resurs3=new QTreeWidgetItem(glav_resurs) ;
                    glav_resurs->setText(6,pathResursa);
                    user_twg_resurs3->setText(0,login);

                    }
                    else
                    {

                        QTreeWidgetItem* items=findPathResursa.at(0);


                       user_twg_resurs3 = new QTreeWidgetItem(items);
                       user_twg_resurs3->setText(0,login);


                    }


            }
            else
  {

                user_twg_resurs3->setText(6,pathResursa);

  }

             //user_twg_resurs3->setText(6,pathResursa);


                   user_twg_resurs3->setText(2,sssList.at(0));
                   user_twg_resurs3->setText(1,sssList.at(1));
                   user_twg_resurs3->setText(4,sssList.at(2));
                   user_twg_resurs3->setText(5,sssList.at(5));
                   QString tmeResurs=sssList.at(kolichecnwo-3)+" "+sssList.at(kolichecnwo-4)+" "+sssList.at(kolichecnwo-1)+" "+sssList.at(kolichecnwo-2);
 user_twg_resurs3->setText(3,tmeResurs);



       }
        }


    }

    }
    //запускаем процессы по запуску консольных команд для чтения данных их БД САМБА и passwd!
    void MainWindow::GetUser()
    {
    m_process->start("pdbedit",QStringList() << "-L");
    m_process3->start("cat",QStringList() << "/etc/passwd");
    m_process_resurs1->start("smbstatus",QStringList()<<"-b");
    if (m_process_resurs1->waitForReadyRead())
    {
    InsertUserResurs();
    }
    if (m_process3->waitForFinished() && m_process->waitForFinished())

    {
       // ui->statusBar->showMessage("сработал!");  //отладочная инфа !!!
    InsertUser();
    }
    else
    {
        ui->statusBar->showMessage("ошибка запроса! cat /etc/passwd :" + QString::number(m_process3->exitCode())+" pdbedit-L :"+QString::number(m_process->exitCode()));

    }
    }
    //из процесса получаем список пользоватлей САМБА!
    void MainWindow::StdOutProcess3()
    {
        QString vihlop=m_process3->readAllStandardOutput();
    QStringList  SpisokUser=vihlop.split("\n",QString::SkipEmptyParts);
    QStringList SpisokItemUser;
    QString Stroka;
    for (int i=0; i<SpisokUser.count();++i)
    {
    Stroka=SpisokUser.at(i);
    SpisokItemUser=Stroka.split(":",QString::SkipEmptyParts);
    //сделаем проверку на UID полльзователя , если меньше 1000, то системный , их будем исключать из списка !
    QString str_uid_user=SpisokItemUser.at(2);
    bool ok;
    int uid_user=str_uid_user.toInt(&ok,10);
    if (uid_user >= 1000)
    {
    SpisokUserSys.append(SpisokItemUser.at(0));
    }
    }

//for (int j=0 ; j< SpisokUserSys.count();++j)
//{
//    ui->textEdit->append(SpisokUserSys.at(j));
//}
}
void MainWindow::StdOutProcessDel()
{
ui->statusBar->showMessage(m_process_del->readAllStandardError());
}
//сбор данных выхлопа о подробных сведениях пользователя процесс2
void MainWindow::StdOutProcess2()
{   ui->textEdit->clear();
    ui->textEdit->append(m_process2->readAllStandardOutput());
    ui->statusBar->showMessage(m_process2->readAllStandardError());

}
//из процесса получаем список пользоватлей САМБА!
void MainWindow::StdOutProcess()
{
QString vihlop=m_process->readAllStandardOutput();
SpisokUserSamba=vihlop.split("\n",QString::SkipEmptyParts);
m_process->close();

}
void MainWindow::on_pushButton_clicked()
{

    QMessageBox::StandardButton reply = QMessageBox::question(this,"Внимание","Хотите закрыть программу?",QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes)
        {QApplication::quit();}
}
//кнопка добавить
void MainWindow::on_actionSamba_triggered()
{
addUs=new AddUser(this);//сформируем в памяти дочернюю форму
//подключим сигнал обртаной связи из дочерней формы
connect(addUs,SIGNAL(sendDataUser(QStringList)),this,SLOT(ReceievDataUser(QStringList)));
addUs->show();//покажим форму

//передадим в дочернюю форму login через паблик функцию SetLogin;
QString log=ui->twg->currentItem()->text(0);

addUs->SetLogin(log);
addUs->SetStatus("full");

//addUs->login=log;
}

void MainWindow::on_twg_itemClicked(QTreeWidgetItem *item, int column)
{    
    ui->textEdit->clear();
m_process2->start("pdbedit",QStringList() << "-L" << "-v" << item->text(1));

QString element=item->text(0);
    QString kolonka=QString::number(column);
    //ui->statusBar->showMessage(element);

    if (! item->text(1).isEmpty())
    {
      ui->action->setDisabled(false);
      ui->action_2->setDisabled(false);
      ui->action_6->setDisabled(false);
    }
    else
    {
         ui->action->setDisabled(true);
         ui->action_2->setDisabled(true);
         ui->action_6->setDisabled(true);
    }
 if ((! item->text(0).isEmpty()) && (item->text(1).isEmpty()) && (item->childCount()==0))

    {
        ui->actionSamba->setDisabled(false);
    }
            else
    {
        ui->actionSamba->setDisabled(true);
}
}


//обновить данные из системы
void MainWindow::on_action_3_triggered()
{

    SpisokUserSamba.clear();
    SpisokUserSys.clear();
    GetUser();

}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Внимание","Хотите закрыть программу?",QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes)
    {QApplication::quit();}
}

void MainWindow::on_action_triggered()
{
if (ui->twg->currentItem()->text(1).isEmpty())
{
   ui->statusBar->showMessage("Не выбран элемент!");
   return;
}

 ui->statusBar->showMessage(ui->twg->currentItem()->text(1));
 QString mess;


 mess=ui->twg->currentItem()->text(1);
 QMessageBox::StandardButton reply = QMessageBox::question(this,"Внимание","Вы действительно хотите удалить пользователя: "+mess+" из БД ?",QMessageBox::Yes | QMessageBox::No);
 if (reply==QMessageBox::Yes)
 {ui->statusBar->showMessage("Процедура удаления!"+mess);
     m_process_del->start("pdbedit",QStringList() << "-x" << "-u" << mess);

m_process_del->waitForFinished();

delete ui->twg->currentItem();
on_action_3_triggered();//кнопка обновить!
QList<QTreeWidgetItem*> findItems=ui->twg->findItems(mess,Qt::MatchContains | Qt::MatchRecursive,0);
if (findItems.count()!=0)
{
ui->twg->setCurrentItem(findItems.at(0));
}
//получить родителя в таблице
//StrokaTabl=new QTreeWidgetItem(twgUserSys);
//StrokaTabl->setText(0,mess);
 }
}

//задать пароль
void MainWindow::on_action_6_triggered()
{
    addUs=new AddUser(this);//сформируем в памяти дочернюю форму
    //подключим сигнал обртаной связи из дочерней формы
    connect(addUs,SIGNAL(sendDataUser(QStringList)),this,SLOT(ReceievDataUser(QStringList)));


    //передадим в дочернюю форму login через паблик функцию SetLogin;
    QString log=ui->twg->currentItem()->text(0);

    addUs->SetLogin(log);
    addUs->SetStatus("pass");
    addUs->show();//покажим форму
}
//редактировать полное имя
void MainWindow::on_action_2_triggered()
{
    addUs=new AddUser(this);//сформируем в памяти дочернюю форму
    //подключим сигнал обртаной связи из дочерней формы
    connect(addUs,SIGNAL(sendDataUser(QStringList)),this,SLOT(ReceievDataUser(QStringList)));


    //передадим в дочернюю форму login через паблик функцию SetLogin;
    QString log=ui->twg->currentItem()->text(0);

    addUs->SetLogin(log);
    addUs->SetStatus("fullname");
    addUs->show();//покажим форму



}



void MainWindow::on_action_vers_triggered()
{
    QProcess process_vers;
    process_vers.start("pdbedit",QStringList()<<"-V");
    process_vers.waitForFinished();
    if (process_vers.exitCode()==0)
    {
        QString mess;
    mess=process_vers.readAll();
    QMessageBox::information(0,"Сообщение!",mess);
  //секция второй части при добавлении -редактирование
    }
    else
    {
    QMessageBox::warning(0,"Внимание!","Произошла ошибка в операции pdbedit -V !!!");

    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index==1)
    {
      ui->action_dell_pid->setVisible(true);
      ui->action_dell_pid->setDisabled(true);
      ui->actionSamba->setVisible(false);
      ui->action->setVisible(false);
      ui->action_2->setVisible(false);
      ui->action_4->setVisible(false);
      ui->action_6->setVisible(false);
    }
    if (index==0)
    {
            ui->action_dell_pid->setVisible(false);
            ui->actionSamba->setVisible(true);
            ui->action->setVisible(true);
            ui->action_2->setVisible(true);
            ui->action_4->setVisible(true);
            ui->action_6->setVisible(true);
}
   if (index==2)
   {
       ui->action_dell_pid->setVisible(false);
       ui->action_dell_pid->setDisabled(true);
       ui->actionSamba->setVisible(false);
       ui->action->setVisible(false);
       ui->action_2->setVisible(false);
       ui->action_4->setVisible(false);
       ui->action_6->setVisible(false);
   }
}





void MainWindow::on_twg_resurs_itemClicked(QTreeWidgetItem *item, int column)
{
if (!item->text(0).isEmpty())
{
    ui->action_dell_pid->setDisabled(false);
}
else
{
    ui->action_dell_pid->setDisabled(true);
}

}

void MainWindow::on_action_dell_pid_triggered()
{
    QString pid_proc=ui->twg_resurs->currentItem()->text(1);
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Внимание","Хотите завершить процесс PID:"+pid_proc+" ?",QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes)
    {
//        bool ok;
//        int pid_proc=str_uid_user.toInt(&ok,10);
      QProcess kill_pid;
      kill_pid.start("kill",QStringList() << pid_proc);
      ui->centralWidget->setCursor(Qt::WaitCursor);
      kill_pid.waitForFinished();
      ui->centralWidget->setCursor(Qt::ArrowCursor);
      on_action_3_triggered();//обновить
      kill_pid.close();

    }

}

void MainWindow::on_radioButton_user_clicked()
{
    on_action_3_triggered();
}

void MainWindow::on_radioButton_net_clicked()
{
    on_action_3_triggered();
}

void MainWindow::on_radioButtonre_resurs_clicked()
{
    on_action_3_triggered();
}


