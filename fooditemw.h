#ifndef FOODITEMW_H
#define FOODITEMW_H

#include <QWidget>
#include <QtWidgets>
#include <QtSql>
#include "selectw.h"

namespace Ui {
class FoodItemW;
}

class FoodItemW : public QWidget
{
    Q_OBJECT

    //int foodNo = 0, QString name = "", float price = 0, int people = 1, QString taste = "",
    //QString describle = "", int hunsu = 1, QString mete = "", QString how = "", QByteArray *pic = nullptr
public:
    explicit FoodItemW(QWidget *parent = nullptr, int foodNo = 0);
    ~FoodItemW();

private slots:
    void on_btnSelectSize_clicked();

private:
    Ui::FoodItemW *ui;

    int foodNo;  //发送信号，让主对话框更新购物车
    QString price;  //价格
    QString foodName;

    void setValues();  //页面组件赋值
};

#endif // FOODITEMW_H
