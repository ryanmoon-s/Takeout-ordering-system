#ifndef FOODDETAILW_H
#define FOODDETAILW_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>

#include "selectw.h"

namespace Ui {
class FoodDetailW;
}

class FoodDetailW : public QDialog
{
    Q_OBJECT

public:
    explicit FoodDetailW(QWidget *parent = nullptr, int foodNo = 0);
    ~FoodDetailW();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

private:
    Ui::FoodDetailW *ui;
    int foodNo;

    void setValues();  //设置界面
};

#endif // FOODDETAILW_H
