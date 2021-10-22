#ifndef ORDERITEMW_H
#define ORDERITEMW_H

#include <QWidget>
#include <QtWidgets>
#include <QtSql>

#include "commentw.h"

namespace Ui {
class OrderItemW;
}

class OrderItemW : public QWidget
{
    Q_OBJECT

public:
    explicit OrderItemW(QWidget *parent = nullptr, int orderNo = 0);
    ~OrderItemW();

signals:
    void receOk(int orderNo);  //发给主对话框，确认收货

private slots:
    void on_BtnSure_clicked();

    void on_btnComm_clicked();

private:
    Ui::OrderItemW *ui;

    int orderNo;  //订单编号

    void setValues();  //设置界面组件值
};

#endif // ORDERITEMW_H
