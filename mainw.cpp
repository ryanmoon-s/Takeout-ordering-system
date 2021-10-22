#include "mainw.h"
#include "ui_mainw.h"

MainW::MainW(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainW)
{
    ui->setupUi(this);

    //关闭就删除内存，当在主窗口中退出登录时，如果登录窗口拒绝了，则直接删除此窗口，而不是隐藏
    setAttribute(Qt::WA_DeleteOnClose);

    //初始化
    iniStateBar();  //初始化状态栏
    iniShopList(); //初始化shopList
    iniHotList();  //初始化hotList
    QIcon iocn1(":new/prefix1/res/热门菜.png");
    QIcon iocn2(":new/prefix1/res/商家们.png");
    ui->tabWidget->setTabIcon(0, iocn1);
    ui->tabWidget->setTabIcon(1, iocn2);
    //updateAddress();  //更新地址--先构造mainw，再进入登录界面发射信号获得userNo值。不然发不过来信号。(所以转在信号处理函数里面初始化地址)
    //updateOrder();  //更新订单List--前提是知道userNo，和上面一样的道理，在发过来的信号处理里面做


    //QSS引入
    QDir qssdir = QApplication::applicationDirPath();
    QString qsspath = qssdir.path() + "/allPages.qss";
    QFile qssFile(qsspath);
    qssFile.open(QFile::ReadOnly);
    QTextStream stream(&qssFile);
    QString stylesheet = stream.readAll();
    this->setStyleSheet(stylesheet);
    qssFile.close();

    ui->labClearCar->installEventFilter(this);
}

MainW::~MainW()
{
    delete ui;
}

void MainW::paintEvent(QPaintEvent *event)  //绘图
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //抗锯齿

    //底层背景
    QRect rect(0, 0, width(), height());
    QPixmap map(":new/prefix1/res/mainBak.jpg");
    painter.drawPixmap(rect, map);

    //大蒙板
    rect.setRect(0, 0, width(), height());
    QPen noPen(Qt::NoPen);
    painter.setPen(noPen);

    QBrush brush;
    QColor brushCol(205, 213, 238, 150); //构造时可以rgba
    brush.setColor(brushCol);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRect(rect);

    /***********************购物车**********************/
    //底层蓝色
    int groupW = ui->groupShopCar->width();
    int groupH = ui->groupShopCar->height();
    rect.setRect(width() - groupW - 11, 11, groupW, groupH);  //布局后和边框固定距11

    brushCol.setRgba(qRgba(18, 150, 219, 100));
    brush.setColor(brushCol);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRoundRect(rect, 3, 3);

    //二层白色
    int gridIconH = ui->gridShopCarIcon->geometry().height();
    rect.setRect(width() - groupW - 11, 22 + gridIconH, groupW, groupH - 11 - gridIconH);

    brushCol.setRgba(qRgba(255, 255, 255, 180));
    brush.setColor(brushCol);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRoundRect(rect, 3, 3);

    /***********************左上角地址选择**********************/
    groupH = ui->groupAddress->height();
    groupW = ui->groupAddress->width();
    rect.setRect(ui->toolBar->width() + 11, 11, groupW, groupH - 1);

    brushCol.setRgba(qRgba(18, 150, 219, 100));
    brush.setColor(brushCol);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRoundRect(rect, 3, 3);


    /***********************工具栏**********************/
    groupH = ui->groupAddress->height();
    groupW = ui->groupAddress->width();
    rect.setRect(0, 0, ui->toolBar->width(), ui->toolBar->height());

    brushCol.setRgba(qRgba(89, 117, 120, 80));
    brush.setColor(brushCol);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRect(rect);

    /***********************订单**********************/
    //底层蓝色
    groupW = ui->groupOrder->width();
    groupH = ui->groupOrder->height();
    int groupCarH = ui->groupShopCar->height();
    rect.setRect(width() - groupW - 11, groupCarH + 11 + 6, groupW, groupH);  //布局后和边框固定距11

    brushCol.setRgba(qRgba(18, 150, 219, 100));
    brush.setColor(brushCol);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRoundRect(rect, 3, 3);

    //二层白色
    gridIconH = ui->gridOrder->geometry().height();
    rect.setRect(width() - groupW - 11, groupCarH + 11 + 7 + 10 + gridIconH, groupW, groupH - 11 - gridIconH);

    brushCol.setRgba(qRgba(255, 255, 255, 180));
    brush.setColor(brushCol);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRoundRect(rect, 3, 3);
}

void MainW::closeEvent(QCloseEvent *event)  //关闭窗口
{
    Q_UNUSED(event);
    exit(3);
}

bool MainW::eventFilter(QObject *watched, QEvent *event)  //事件过滤
{
    //清空购物车
    if(watched == ui->labClearCar){
        if(event->type() == QEvent::MouseButtonPress){
            clearCar();
            QMessageBox::information(this, "提示", "购物车已清空");
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MainW::iniShopList()  //初始化shopList
{
    ShopItemW *shopItemW;
    QListWidgetItem *item;

    //查询
    QSqlQuery query;
    query.exec("select No from shopInfo");
    query.first();
    QSqlRecord record = query.record();
    int No = record.value("No").toInt();

    //插入第一个
    item = new QListWidgetItem();
    shopItemW = new ShopItemW(this, No);   //shopNo
    connect(shopItemW, SIGNAL(addShopTab(int)), this, SLOT(addShopTab(int))); //建立新的tab
    item->setSizeHint(QSize(shopItemW->width(), shopItemW->height()));
    ui->shopList->addItem(item);
    ui->shopList->setItemWidget(item, shopItemW);

    //插入后续
    while(query.next()) {
        record = query.record();
        No = record.value("No").toInt();
        item = new QListWidgetItem();
        shopItemW = new ShopItemW(this, No);   //shopNo
        connect(shopItemW, SIGNAL(addShopTab(int)), this, SLOT(addShopTab(int))); //建立新的tab
        item->setSizeHint(QSize(shopItemW->width(), shopItemW->height()));
        ui->shopList->addItem(item);
        ui->shopList->setItemWidget(item, shopItemW);
    }
}

void MainW::iniHotList()  //初始化hotList
{
    FoodItemW *foodItemW;
    QListWidgetItem *item;

    //查询
    QSqlQuery query;
    query.exec("select No, MonthSale from food");
    query.first();
    QSqlRecord record = query.record();

    /***********************找出销量排行前10*************************/
    struct hotVa{
      int No;
      int SaleNum;
    };
    hotVa arr[100];
    int insertIndex = 1;
    //入数组
    arr[0].No = record.value("No").toInt();
    arr[0].SaleNum = record.value("MonthSale").toInt();
    while(query.next()){
        record = query.record();
        arr[insertIndex].No = record.value("No").toInt();
        arr[insertIndex++].SaleNum = record.value("MonthSale").toInt();
    }
    //选择排序 倒序
    hotVa temp;
    for(int i = 0; i < insertIndex - 1; i++){
        int max = i;
        //find
        for(int j = i; j < insertIndex; j++){
            if(arr[j].SaleNum > arr[max].SaleNum){
                max = j;
            }
        }
        //swap
        temp = arr[i];
        arr[i] = arr[max];
        arr[max] = temp;
    }

    //插入界面，前十条
    for(int i = 0; i < 10; i++) {
        item = new QListWidgetItem();
        foodItemW = new FoodItemW(this, arr[i].No);  //foodNo   -->fooditemw-->selectw-->shopcaritemw
        item->setData(Qt::WhatsThisRole, arr[i].No);  //mainw单击时，识别物品的foodNo
        item->setSizeHint(QSize(foodItemW->width(), foodItemW->height()));
        ui->hotList->addItem(item);
        ui->hotList->setItemWidget(item, foodItemW);
    }
}

void MainW::iniStateBar()  //初始化状态栏
{
    copyright = new QLabel();
    copyright->setText("DESIGNED BY SHUN ALL RIGHTS RESERVED");
    ui->statusbar->addWidget(copyright);
}

void MainW::updateMoney()  //更新总价格与差价
{
    //更新总价格
    float totalPrice = 0;
    carItem pri;
    foreach(pri, OneCar){
        totalPrice += (pri.price*pri.num);
    }
    ui->labTolPrice->setText(QString::asprintf("￥%.2f", totalPrice));

    //更新差多少起送----不能跨店点，跨店直接清空购物车
    QSqlQuery query;
    QSqlRecord record;
    query.prepare("select DeleveryBeginPrice from food, shopInfo where food.No = ? and food.shopNo = shopInfo.No");
    query.bindValue(0, pri.foodNo);
    query.exec();
    query.first();
    record = query.record();
    float diff = totalPrice - record.value("DeleveryBeginPrice").toFloat();
    if(diff >= 0){
        ui->labBegPrice->setText("￥0");
    }else{
        ui->labBegPrice->setText(QString::asprintf("￥%.2f", -diff));
    }
}

void MainW::clearCar() //清空购物车
{
    //从内存中清除
    OneCar.clear();

    //表上删除
    ui->carList->clear();

    //更新价格
    ui->labBegPrice->setText("￥0");
    ui->labTolPrice->setText("￥0");
}

void MainW::updateAddress()  //更新地址
{
    ui->comAddress->clear();
    ui->comAddress->addItem("请先在此选择收货地址!");  //初始化地址

    //查询
    QSqlQuery query;
    QSqlRecord record;
    query.prepare("select Address from address where userNo = ?");
    query.bindValue(0, userNo);
    query.exec();
    query.first();
    if(!query.isValid())
        return;
    record = query.record();

    ui->comAddress->addItem(record.value("Address").toString());
    while(query.next()){
        record = query.record();
        ui->comAddress->addItem(record.value("Address").toString());
    }
}

void MainW::updateOrder()  //更新订单List
{
    ui->orderList->clear();
    OrderItemW *orderWindow;
    QListWidgetItem *item;
    int count = 0;

    //查询
    QSqlQuery query;
    QSqlRecord record;
    query.prepare("select No from orderAgent where UserNo = ?");
    query.bindValue(0, userNo);
    query.exec();
    query.first();
    if(!query.isValid())
        return;
    record = query.record();
    int No = record.value("No").toInt();

    //加入第一条
    orderWindow = new OrderItemW(this, No);
    connect(orderWindow, SIGNAL(receOk(int)), this, SLOT(receOk(int)));  //收货信号
    item = new QListWidgetItem();
    item->setSizeHint(QSize(orderWindow->width(), orderWindow->height()));
    ui->orderList->addItem(item);
    ui->orderList->setItemWidget(item, orderWindow);
    count++;

    //加入后续
    while(query.next()) {
        record = query.record();
        No = record.value("No").toInt();

        orderWindow = new OrderItemW(this, No);
        connect(orderWindow, SIGNAL(receOk(int)), this, SLOT(receOk(int)));  //收货信号
        item = new QListWidgetItem();
        item->setSizeHint(QSize(orderWindow->width(), orderWindow->height()));
        ui->orderList->addItem(item);
        ui->orderList->setItemWidget(item, orderWindow);
        count++;
    }

    //更新订单数
    ui->labOrderNum->setText(QString::asprintf("订单数[%d]", count));
}


void MainW::on_actService_triggered()  //客服
{
    QMessageBox::information(this, "提示", "抱歉，此功能尚在开发中！敬请期待！");
}

void MainW::on_actExit_triggered()  //退出登录
{
    LoginW login;
    int ret = login.exec();
    if(ret == QDialog::Rejected){
        exit(2);
    }
}

void MainW::addShopTab(int shopNo)  //shopitemw发来的信号，建立新的tab
{
    InnerShopW *inn = new InnerShopW(this, shopNo);
    inn->setAttribute(Qt::WA_DeleteOnClose);  //关闭释放内存
    int curindex = ui->tabWidget->addTab(inn, "新展开店铺");
    ui->tabWidget->setCurrentIndex(curindex);
    QIcon iocn(":new/prefix1/res/当前商家.png");
    ui->tabWidget->setTabIcon(curindex, iocn);
}

void MainW::updateShopCar(int foodNo, int hot) //selectw发来的信号，更新购物车
{
    QSqlQuery query;
    QSqlRecord record;

    //如果跨店了，清空购物车
    query.prepare("select shopNo from food where No = ?");
    query.bindValue(0, foodNo);
    query.exec();
    query.first();
    record = query.record();
    int shopNNNo = record.value("shopNo").toInt();
    if(OneCar.count() == 0){
        jumpShopFlag = shopNNNo;
    }else{
        if(jumpShopFlag != shopNNNo){
            //清空购物车
            jumpShopFlag = shopNNNo;  //换成新商品的店铺号
            clearCar();
            //不退出，继续添加菜品
        }
    }

    //添加到内存(购物车)
    carItem caritem;
    caritem.foodNo = foodNo;
    caritem.hot = hot;
    caritem.carNo = carNo;

    query.prepare("select Price from food where No = ?");
    query.bindValue(0, foodNo);
    query.exec();
    query.first();
    record = query.record();
    caritem.price = record.value("Price").toFloat();

    OneCar.append(caritem);

    //设置菜品数
    ui->labFoodNum->setText(QString::asprintf("菜品数:%d", OneCar.count()));

    //添加到carList
    QListWidgetItem *item = new QListWidgetItem();
    ShopCarItemW *itemWindow = new ShopCarItemW(this, carNo, foodNo);  //carNo初始为1
    connect(itemWindow, SIGNAL(deleteCarItem(int)), this, SLOT(deleteCarItem(int)));  //删除物品信号
    connect(itemWindow, SIGNAL(spinCarItem(int, int)), this, SLOT(spinCarItem(int, int)));  //加减物品

    item->setData(Qt::WhatsThisRole, carNo++);
    item->setSizeHint(QSize(itemWindow->width(), itemWindow->height()));

    ui->carList->addItem(item);
    ui->carList->setItemWidget(item, itemWindow);

    //更新总价格与差价
    updateMoney();
}

void MainW::deleteCarItem(int No)  //shopcaritemw发送信号，让主窗口删除一个购物车物品
{
    //List上移除
    for (int i = 0; i < ui->carList->count(); i++) {
        if(ui->carList->item(i)->data(Qt::WhatsThisRole).toInt() ==No){
            ui->carList->takeItem(i);
        }
    }
    //内存上移除 foreach 只能用于遍历
    QList<carItem>::iterator it;
    for (it = OneCar.begin(); it != OneCar.end(); it++) {
        if((*it).carNo == No){
            //erase才是删除，remove是移到最后。erase后位置变动，迭代器失效不能再++操作，但是这个函数会返回一个iterator作为下一次迭代的位置
            it = OneCar.erase(it);
            break;
        }
    }
    //设置菜品数
    ui->labFoodNum->setText(QString::asprintf("菜品数:%d", OneCar.count()));

    //更新总价与差价
    if(OneCar.count() == 0){
        ui->labBegPrice->setText("￥0");
        ui->labTolPrice->setText("￥0");
    }else{
        //更新总价格与差价
        updateMoney();
    }
}

void MainW::spinCarItem(int No, int num)  //shopcaritemw发送信号，加减物品，让主窗口更新总价与差价
{
    //更新商品数量，foreach不能用于改变值
    carItem item;
    QList<carItem>::iterator it;
    for (it = OneCar.begin(); it != OneCar.end(); it++) {
        if((*it).carNo == No){
            (*it).num = num;
            break;
        }
    }

    //更新总价与差价
    updateMoney();
}

void MainW::whoLogin(int userNo)  //logw发来的信号，通知主窗口谁登录了
{
    this->userNo = userNo;

    updateAddress();  //更新地址
    updateOrder();  //更新订单List
}

void MainW::payOk()  //payw发来的信号，让主对话框下单
{
    //计算总价
    float totalPrice = 0;
    carItem pri;
    foreach(pri, OneCar){
        totalPrice += (pri.price*pri.num);
    }

    //获取时间
    QDateTime  datetime = QDateTime::currentDateTime();
    QString curtime = datetime.toString("yyyy-MM-dd hh:mm:ss");

    /*********************************加入数据库*************************************/
    QSqlQuery query;
    QSqlRecord record;

    //orderAgent
    query.prepare("insert into orderAgent values(NULL, ?, ?, ?, 0, ?, ?, ?, 0)");
    query.bindValue(0, userNo);

    QSqlQuery qu;
    qu.prepare("select ShopNo from food where No = ?");
    qu.bindValue(0, OneCar.at(0).foodNo);
    qu.exec();
    qu.first();

    query.bindValue(1, qu.record().value("shopNo").toInt());
    query.bindValue(2, ui->comAddress->currentText());
    query.bindValue(3, ui->editExtra->text());
    query.bindValue(4, curtime);
    query.bindValue(5, totalPrice);

    query.exec();

    //orderContent
    query.exec("select No from orderAgent where DeliveryState = 0");
    query.last();  //获取刚刚插入的那条记录(No自增,所有last是刚才插入那条)
    record = query.record();
    int orderAgentNo = record.value("No").toInt();
    carItem carit;
    foreach(carit, OneCar){  //多条记录
        query.prepare("insert into orderContent values(NULL, ?, ?, ?)");
        query.bindValue(0, carit.foodNo);
        query.bindValue(1, orderAgentNo);
        query.bindValue(2, carit.hot);
        query.exec();
    }

    //提示成功
    QMessageBox::information(this, "提示","已成功下单!");

    //更新订单List
    updateOrder();
}

void MainW::receOk(int orderNo)  //orderitemw发来的信号，确认收货
{
    //更新数据库
    QSqlQuery query;
    QSqlRecord record;

    query.prepare("update orderAgent set DeliveryState = 1 where No = ?");
    query.bindValue(0, orderNo);
    query.exec();

    //重新更新订单
    updateOrder();

    //提示
    QMessageBox::information(this, "提示", "收货成功!");
}

void MainW::on_tabWidget_currentChanged(int index)  //tab切换时，自动删除店铺页
{
    Q_UNUSED(index);
    if(index < 2 && ui->tabWidget->count() > 2){
        QWidget *del = ui->tabWidget->widget(ui->tabWidget->count() - 1);
        del->close();
    }
}

void MainW::on_btnMakeOrder_clicked()  //下单
{
    if(ui->comAddress->currentIndex() == 0){
        QMessageBox::information(this, "提示", "您未选择收货地址! 如未配置请到地址管理页面填写。");
        return;
    }

    if(OneCar.count() > 0){
        PayW *payw = new PayW(this, userNo, ui->editExtra->text());
        connect(payw, SIGNAL(payOk()), this, SLOT(payOk()));  //让主对话框下单
        int ret = payw->exec();
        if(ret == QDialog::Rejected){
            QMessageBox::information(this, "提示", "您已取消支付!");
        }
    }else{
        QMessageBox::information(this, "提示", "您未选择菜品!");
    }
}

void MainW::on_actiAddress_triggered()  //地址管理
{
    AddressConW *add = new AddressConW(this, userNo);
    add->exec();

    //更新地址
    updateAddress();
}

void MainW::on_actUserInfo_triggered()  //用户信息
{
    UserInfoW *us = new UserInfoW(this, userNo);
    us->exec();
}

void MainW::on_hotList_itemClicked(QListWidgetItem *item)  //单击进入菜品信息
{
    int No = item->data(Qt::WhatsThisRole).toInt();
    FoodDetailW *foodw = new FoodDetailW(this, No);
    foodw->exec();
}

