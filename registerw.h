#ifndef REGISTERW_H
#define REGISTERW_H

#include <QDialog>
#include <QtWidgets>
#include <QPaintEvent>
#include <QtSql>

namespace Ui {
class RegisterW;
}

class RegisterW : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterW(QWidget *parent = nullptr);
    ~RegisterW();

    static QString encrypt(const QString &str);  //MD5加密

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void successful(QString acc, QString pass);    //注册成功，发送信号让登录窗口填充刚注册的密码

private slots:

    void on_BtnSure_clicked();

private:
    Ui::RegisterW *ui;
    QSqlDatabase DB; //数据库

    bool CheckInfo(QString account, QString pass, QString pay, QString nick, QString pro, QString city);  //信息检查
};

#endif // REGISTERW_H
