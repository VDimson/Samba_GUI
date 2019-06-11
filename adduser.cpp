#include "adduser.h"
#include "ui_adduser.h"
#include "QMessageBox"

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);
    ui->lineEdit_pass->setEchoMode(QLineEdit::Password);
    ui->lineEdit_pass2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_login->setDisabled(true);
    this->setModal(true);


}

AddUser::~AddUser()
{
    delete ui;
}
void AddUser::SetStatus(QString status)
{
StatusForm=status;

if (StatusForm=="pass")
{
ui->llineEdit_Full_Name->setVisible(false);
ui->label_Full_Name->setVisible(false);
ui->lineEdit_pass->setFocus();

}
if (StatusForm=="fullname")
{
ui->groupBox_Pass->setVisible(false);
}
}
void AddUser::SetLogin(QString &login)
{
ui->lineEdit_login->setText(login);
}
void AddUser::sborDataUser()
{
QStringList spisok;
spisok<<ui->lineEdit_login->text()<<ui->llineEdit_Full_Name->text()<<ui->lineEdit_pass->text()<<StatusForm;
emit sendDataUser(spisok);//посылаем сигнал главной форме с содержанием в списке логина, полного имени и пароля
}

void AddUser::on_buttonBox_accepted()
{

if ((!ui->lineEdit_pass->text().isEmpty()) && (ui->lineEdit_pass2->text().isEmpty()))
{
    QMessageBox::warning(0,"Внимание!","Повтор пароля не может быть пустым!");
    return;
}
if (ui->lineEdit_pass->text()!=ui->lineEdit_pass2->text())
    {
        QMessageBox::warning(0,"Внимание!","Повтор пароля не совпадает с заданным!");
ui->lineEdit_pass2->setFocus();
return;
    }
sborDataUser();

//    if (ui->llineEdit_Full_Name->text().isEmpty())
//    {
//        QMessageBox::warning(0,"Внимание!","Не заполнено поле : "+ui->label_Full_Name->text()+" !!!");

//    }
//    if (ui->lineEdit_pass->text().isEmpty())
//    {
//        QMessageBox::warning(0,"Внимание!","Пароль не может быть пустым!");
//    }
//    else
//    {
//    if (ui->lineEdit_pass->text()!=ui->lineEdit_pass2->text())
//    {
//        QMessageBox::warning(0,"Внимание!","Повтор пароля не совпадает с заданным!");

//        //QMessageBox::StandardButton reply = QMessageBox::question(this,"Повтор пароля не совпадает с заданным!",QMessageBox::Yes | QMessageBox::No);
//    }
//    }

}







void AddUser::on_pushButton_pressed()
{

    ui->lineEdit_pass->setEchoMode(QLineEdit::Normal);

    ui->lineEdit_pass2->setText(ui->lineEdit_pass->text());
}

void AddUser::on_pushButton_released()
{
    ui->lineEdit_pass->setEchoMode(QLineEdit::Password);

}

void AddUser::on_lineEdit_pass_textChanged(const QString &arg1)
{
    ui->lineEdit_pass2->clear();
}
