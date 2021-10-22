#ifndef ADDRESSCONW_H
#define ADDRESSCONW_H

#include <QDialog>
#include <QtSql>
#include <QtWidgets>

namespace Ui {
class AddressConW;
}

class AddressConW : public QDialog
{
    Q_OBJECT

public:
    explicit AddressConW(QWidget *parent = nullptr, int userNo = 1);
    ~AddressConW();

protected:
    bool eventFilter(QObject *watched, QEvent *event)override;
    void paintEvent(QPaintEvent *event)override;

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_BtnSure_clicked();

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_acDel_triggered();

    void on_acEdit_triggered();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::AddressConW *ui;
    int userNo;  //用户编号
    int whatFlag;  //0增加地址，具体数字修改地址(对应的地址编号)

    void loadAddress();  //加载地址
};

#endif // ADDRESSCONW_H
