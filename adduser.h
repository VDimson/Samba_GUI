#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class AddUser;
}

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = 0);
    ~AddUser();

void SetLogin(QString &login);
void SetStatus(QString status);
private slots:
void on_buttonBox_accepted();
void sborDataUser();





void on_pushButton_pressed();

void on_pushButton_released();

void on_lineEdit_pass_textChanged(const QString &arg1);

signals:
void sendDataUser(QStringList spisok);

private:
    Ui::AddUser *ui;
QString StatusForm;

};

#endif // ADDUSER_H
