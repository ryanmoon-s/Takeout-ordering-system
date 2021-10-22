#ifndef FORGETPASSW_H
#define FORGETPASSW_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>   //QtSql  (not QSql)

#include "registerw.h"   //MD5加密

namespace Ui {
class ForgetPassW;
}

class ForgetPassW : public QDialog
{
    Q_OBJECT

public:
    explicit ForgetPassW(QWidget *parent = nullptr);
    ~ForgetPassW();
signals:
    void successful(QString acc, QString pass);    //注册成功，发送信号让登录窗口填充刚注册的密码

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

    void on_BtnSure_clicked();

private:
    Ui::ForgetPassW *ui;
};

#endif // FORGETPASSW_H
