#ifndef SELECTW_H
#define SELECTW_H

#include <QWidget>
#include <QtWidgets>

namespace Ui {
class SelectW;
}

class SelectW : public QWidget
{
    Q_OBJECT

public:
    explicit SelectW(QWidget *parent = nullptr, QString price = "", int foodNO = 1, QString foodName = "");
    ~SelectW();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void updateShopCar(int foodNo, int hot);  //让主程序更新购物车

private slots:
    void on_btnCancel_clicked();

    void on_btnSel2_clicked();

    void on_btnSel3_clicked();

    void on_btnSel4_clicked();

    void on_btnSel5_clicked();

    void on_btnSel6_clicked();

    void on_btnSel1_clicked();

    void on_btnAddCar_clicked();

private:
    Ui::SelectW *ui;

    int foodNo; //向主对话框发送信号，foodNO+所选信息
    int hot = 1;

};

#endif // SELECTW_H
