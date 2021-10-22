#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>
#include <QtSql>
#include <QtWidgets>

#include "loginw.h"
#include "shopitemw.h"
#include "innershopw.h"
#include "shopcaritemw.h"
#include "payw.h"
#include "addressconw.h"
#include "orderitemw.h"
#include "userinfow.h"
#include "fooddetailw.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainW; }
QT_END_NAMESPACE

class MainW : public QMainWindow
{
    Q_OBJECT

public:
    MainW(QWidget *parent = nullptr);
    ~MainW();

protected:
    void paintEvent(QPaintEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:

    void on_actService_triggered();

    void on_actExit_triggered();

    void addShopTab(int shopNo);  //shopitemw发来的信号，建立新的tab
    void updateShopCar(int foodNo, int hot);  //selectw发来的信号，更新购物车
    void deleteCarItem(int No);  //shopcaritemw发送信号，让主窗口删除一个购物车物品
    void spinCarItem(int No, int num);  //shopcaritemw发送信号，加减物品，让主窗口更新总价与差价
    void whoLogin(int userNo);  //logw发来的信号，通知主窗口谁登录了
    void payOk();  //payw发来的信号，让主对话框下单
    void receOk(int orderNo);  //orderitemw发来的信号，确认收货

    void on_tabWidget_currentChanged(int index);

    void on_btnMakeOrder_clicked();

    void on_actiAddress_triggered();



    void on_actUserInfo_triggered();

    void on_hotList_itemClicked(QListWidgetItem *item);

private:
    Ui::MainW *ui;

    QLabel *copyright; //状态栏文字
    int carNo = 1;  //购物车标识，每一个物品唯一拥有一个数，子窗口发来信号删除一个购物车物品
    struct carItem{  //购物车内容
        int foodNo;
        int carNo;  //购物车标识，唯一标志一个物品，删除时靠这个来识别
        float price;
        int num = 1;
        int hot;
    };
    QList<carItem> OneCar;  //购物车
    int jumpShopFlag;  //跨店标志，添加一个物品后它就等于这个物品的店编号。购物车为空时置0
    int userNo;  //登录窗口设置这个值通知谁登录了

    void iniShopList();  //初始化shopList

    void iniHotList(); //初始化hotList，销量前多少的菜品展示出来

    void iniStateBar();  //初始化状态栏

    void updateMoney();   //更新总价格与差价

    void clearCar();  //清空购物车

    void updateAddress();  //更新地址

    void updateOrder();  //更新订单List


};
#endif // MAINW_H
