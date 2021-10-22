#ifndef SHOPITEMW_H
#define SHOPITEMW_H

#include <QWidget>
#include <QtWidgets>
#include <QtSql>


namespace Ui {
class ShopItemW;
}

class ShopItemW : public QWidget
{
    Q_OBJECT

public:
    explicit ShopItemW(QWidget *parent = nullptr, int shopNo = 1);
    ~ShopItemW();

private slots:


    void on_BtnSure_clicked();

signals:
    void addShopTab(int shopNO);

private:
    Ui::ShopItemW *ui;

    //由于此对话框绑定到了mainw的listWidget的一个item上面，是与mainw完全隔离的，点击进店时并不知道是哪个店，所以在构造时传入店铺编号进来，建立联系。
    int shopNo;

    void setValues(); //赋值
};

#endif // SHOPITEMW_H
