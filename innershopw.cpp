#include "innershopw.h"
#include "ui_innershopw.h"

InnerShopW::InnerShopW(QWidget *parent, int shopNo) :
    QDialog(parent),
    ui(new Ui::InnerShopW)
{
    ui->setupUi(this);
    this->shopNo = shopNo;

    //QSS引入
    QDir qssdir = QApplication::applicationDirPath();
    QString qsspath = qssdir.path() + "/allPages.qss";
    QFile qssFile(qsspath);
    qssFile.open(QFile::ReadOnly);
    QTextStream stream(&qssFile);
    QString stylesheet = stream.readAll();
    this->setStyleSheet(stylesheet);
    qssFile.close();

    shopListIni();  //List初始化
    shopInfoIni();  //shop信息初始化
    commentIni();  //评价初始化
}

InnerShopW::~InnerShopW()
{
    delete ui;
}

void InnerShopW::paintEvent(QPaintEvent *event)  //绘制
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPen noPen(Qt::NoPen);
    painter.setPen(noPen);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    QRect rect;
    QColor brushCol;
    painter.setRenderHint(QPainter::Antialiasing);  //抗锯齿

    /***********************最底层**********************/
    rect.setRect(0, 0, width(), height());

    brushCol.setRgba(qRgba(133, 191, 227, 200));
    brush.setColor(brushCol);
    painter.setBrush(brush);

    painter.drawRect(rect);

    /***********************倒数第二层**********************/
    rect.setRect(0, 0.1*height(), width(), 0.9*height());

    brushCol.setRgba(qRgba(255, 255, 255, 255));
    brush.setColor(brushCol);
    painter.setBrush(brush);

    painter.drawRect(rect);

    /***********************groupTop**********************/
    int groupW = ui->groupTop->width();
    int groupH = ui->groupTop->height();
    rect.setRect(13, 13, groupW, groupH);  //布局后和边框固定距11

    brushCol.setRgba(qRgba(250, 250, 250, 255));
    brush.setColor(brushCol);
    painter.setBrush(brush);

    painter.drawRoundRect(rect, 3, 3);

}

void InnerShopW::shopListIni()  //List初始化
{
    FoodItemW *foodItemW;
    QListWidgetItem *item;
    int i = 0;

    //查询
    QSqlQuery query;
    query.exec("select No from food where shopNo = ?");
    query.bindValue(0, shopNo);
    query.exec();
    query.first();
    QSqlRecord record = query.record();
    int No = record.value("No").toInt();

    //插入--套餐--
    item = new QListWidgetItem();
    QColor col(qRgb(171,0,0));
    item->setText("套餐");
    item->setTextColor(col);
    ui->foodList->addItem(item);

    //插入第一条
    item = new QListWidgetItem();
    foodItemW = new FoodItemW(this, No);
    item->setData(Qt::WhatsThisRole, No);
    item->setSizeHint(QSize(foodItemW->width(), foodItemW->height()));
    ui->foodList->addItem(item);
    ui->foodList->setItemWidget(item, foodItemW);

    //插入后续
    while (query.next()) {
        record = query.record();
        No = record.value("No").toInt();

        i++;
        if(i == 2){
           item = new QListWidgetItem();
           item->setText("单品");
           item->setTextColor(col);
           ui->foodList->addItem(item);
        }else if(i == 4){
            item = new QListWidgetItem();
            item->setText("其它");
            item->setTextColor(col);
            ui->foodList->addItem(item);
        }

        item = new QListWidgetItem();
        foodItemW = new FoodItemW(this, No);  //foodNo   -->fooditemw-->selectw-->shopcaritemw
        item->setData(Qt::WhatsThisRole, No);
        item->setSizeHint(QSize(foodItemW->width(), foodItemW->height()));
        ui->foodList->addItem(item);
        ui->foodList->setItemWidget(item, foodItemW);
    }
}

void InnerShopW::shopInfoIni()  //shop信息初始化
{
    //查询
    QSqlQuery query;
    query.prepare("select * from shopInfo where No = ?");
    query.bindValue(0, shopNo);
    query.exec();
    query.first();
    QSqlRecord record = query.record();

    //取值
    QByteArray pic = record.value("Picture").toByteArray();
    QString notice = record.value("Notice").toString();
    QString name = record.value("Name").toString();
    QString lab = record.value("Label").toString();
    QString detime = record.value("DeTime").toString();
    QString deser = record.value("DeService").toString();
    QString address = record.value("Address").toString();
    QString phone = record.value("Phone").toString();
    int isbook = record.value("IsBook").toInt();
    int isrest = record.value("IsRest").toInt();

    //设定
    if(notice.size()> 21 ){
        ui->labNotice->setText(notice.left(21) + " ...");  //21
    }else
        ui->labNotice->setText(notice);
    ui->labNotice_2->setText(notice);
    ui->labLabel->setText("@" + lab);
    ui->labName->setText(name);
    ui->labAddress->setText(address);
    ui->labPhone->setText(phone);
    ui->labTime->setText(detime);
    ui->labWhoDe->setText(deser);
    if(isrest == 0){
        ui->labRest->setText("营业中");
        ui->labRest->setStyleSheet("color:green");
        ui->labBook->hide();  //营业中，不展示是否预定
    }else{  //休息中
        ui->labRest->setText("休息中");
        ui->labRest->setStyleSheet("color:red");
        if(isbook == 1){
            ui->labBook->setStyleSheet("color:green");
            ui->labBook->setText("支持预订");
        }else{
            ui->labBook->setStyleSheet("color:red");
            ui->labBook->setText("不支持预定");
        }
    }
    QPixmap map;
    map.loadFromData(pic);
    ui->labPic->setPixmap(map.scaled(0.8*ui->groupTop->height(), 0.8*ui->groupTop->height()));
}

void InnerShopW::commentIni()  //评价初始化
{
    QSqlQuery query;
    QSqlRecord record;
    QSqlQuery userQu;
    QListWidgetItem *item;
    QString comment;

    //查询
    query.prepare("select * from comment where shopNo = ?");
    query.bindValue(0, shopNo);
    query.exec();
    query.first();
    if(!query.isValid())
        return;
    record = query.record();

    //设置第一条
    userQu.prepare("select NickName from userInfo where No = ?");
    userQu.bindValue(0, record.value("UserNo").toInt());
    userQu.exec();
    userQu.first();
    comment = "<" + userQu.value("NickName").toString() +"> :    " + record.value("Content").toString();
    item = new QListWidgetItem();
    item->setText(comment);
    ui->commList->addItem(item);

    //设置后续
    while(query.next()){ 
        record = query.record();
        userQu.prepare("select NickName from userInfo where No = ?");
        userQu.bindValue(0, record.value("UserNo").toInt());
        userQu.exec();
        userQu.first();
        comment = "<" + userQu.value("NickName").toString() +"> :    " + record.value("Content").toString();
        item = new QListWidgetItem();
        item->setText(comment);
        ui->commList->addItem(item);
    }
}

void InnerShopW::on_push1_clicked()  //to 0
{
    ui->foodList->setCurrentRow(0);
}

void InnerShopW::on_push2_clicked()  //to 5
{
    ui->foodList->setCurrentRow(5);
}

void InnerShopW::on_push3_clicked()  //to 15
{
    ui->foodList->setCurrentRow(15);
}


void InnerShopW::on_foodList_itemClicked(QListWidgetItem *item)  //单击显示菜品细节
{
    int No = item->data(Qt::WhatsThisRole).toInt();
    FoodDetailW *foodw = new FoodDetailW(this, No);
    foodw->exec();
}
