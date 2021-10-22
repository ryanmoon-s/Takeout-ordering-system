#ifndef USERINFOW_H
#define USERINFOW_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class UserInfoW;
}

class UserInfoW : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoW(QWidget *parent = nullptr, int userNo = 0);
    ~UserInfoW();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_BtnSure_clicked();

private:
    Ui::UserInfoW *ui;
    int userNo;  //用户编号

    void setValues();
};

#endif // USERINFOW_H
