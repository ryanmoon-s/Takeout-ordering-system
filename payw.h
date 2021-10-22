#ifndef PAYW_H
#define PAYW_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class PayW;
}

class PayW : public QDialog
{
    Q_OBJECT

public:
    explicit PayW(QWidget *parent = nullptr, int userNo = 0, QString extra = "");
    ~PayW();

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void payOk();  //让主对话框下单

private slots:
    void on_btnPay_clicked();

private:
    Ui::PayW *ui;

    int userNo;
};

#endif // PAYW_H
