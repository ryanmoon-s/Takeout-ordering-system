#ifndef SHOPCARITEMW_H
#define SHOPCARITEMW_H

#include <QWidget>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class ShopCarItemW;
}

class ShopCarItemW : public QWidget
{
    Q_OBJECT

public:
    explicit ShopCarItemW(QWidget *parent = nullptr, int carNo = 1, int foodNo = 1);
    ~ShopCarItemW();


protected:
    bool eventFilter(QObject *watched, QEvent *event)override;

signals:
    void deleteCarItem(int No);  //发送信号，让主窗口删除一个购物车物品
    void spinCarItem(int No, int num);  //加减物品，让主窗口更新总价与差价

private slots:
    void on_spinBox_valueChanged(int arg1);  //加减物品，让主窗口更新总价与差价

private:
    Ui::ShopCarItemW *ui;

    int carNo;  //每个物品有一个唯一的值，主窗口删除此物品的标识。绑定在每个item的data上
    int foodNo;  //食物编号
    float signalPri; //单价

    void setValues();  //设置名字与价格
};

#endif // SHOPCARITEMW_H
