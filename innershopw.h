#ifndef INNERSHOPW_H
#define INNERSHOPW_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>
#include "fooditemw.h"
#include "fooddetailw.h"

namespace Ui {
class InnerShopW;
}

class InnerShopW : public QDialog
{
    Q_OBJECT

public:
    explicit InnerShopW(QWidget *parent = nullptr, int shopNo = 1);
    ~InnerShopW();

protected:
    void paintEvent(QPaintEvent *event) override;


private slots:
    void on_push1_clicked();

    void on_push2_clicked();

    void on_push3_clicked();


    void on_foodList_itemClicked(QListWidgetItem *item);

private:
    Ui::InnerShopW *ui;

    void shopListIni();  //List初始化
    void shopInfoIni(); //shop信息初始化
    void commentIni(); //评价初始化
    int shopNo;
};

#endif // INNERSHOPW_H
