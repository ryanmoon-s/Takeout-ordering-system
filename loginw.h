#ifndef LOGINW_H
#define LOGINW_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>
#include <QDesktopWidget>

#include "registerw.h"
#include "forgetpassw.h"
#include "mainw.h"

namespace Ui {
class LoginW;
}

class LoginW : public QDialog
{
    Q_OBJECT

public:
    explicit LoginW(QWidget *parent = nullptr);
    ~LoginW();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void whoLogin(int userNo);  //发信号通知主窗口谁登录了

private slots:
    void on_registerSucc(QString acc, QString pass);  //注册成功发来的信号，填充账号与密码

    void on_BtnSureLogin_clicked();


    void on_labRegister_linkActivated(const QString &link);

private:
    Ui::LoginW *ui;

    QSqlDatabase DB;
    int inputCount = 0;  //输入次数
    QString PSWD;  //加密后的密码，记住密码时，读入这里面

};

#endif // LOGINW_H
