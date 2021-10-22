#include "shopcaritemw.h"
#include "ui_shopcaritemw.h"

ShopCarItemW::ShopCarItemW(QWidget *parent, int carNo, int foodNo) :
    QWidget(parent),
    ui(new Ui::ShopCarItemW)
{
    ui->setupUi(this);
    this->carNo = carNo;
    this->foodNo = foodNo;
    ui->labDel->installEventFilter(this);

    setValues(); //设置名字与价格
}

ShopCarItemW::~ShopCarItemW()
{
    delete ui;
}

bool ShopCarItemW::eventFilter(QObject *watched, QEvent *event)  //事件过滤
{
    //删除图片按下效果
    QPixmap mapblack(":/new/prefix1/res/删除(黑色).png");
    QPixmap mapblue(":/new/prefix1/res/删除(蓝色).png");
    if(watched == ui->labDel){
        if(event->type() == QEvent::MouseButtonPress){
            //换背景图片
            ui->labDel->setPixmap(mapblack);
            //发送信号，让主窗口删除此物品
            emit deleteCarItem(carNo);
        }else if(event->type() == QEvent::MouseButtonRelease){
            ui->labDel->setPixmap(mapblue);
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ShopCarItemW::setValues() //设置名字与价格
{
    QSqlQuery query;
    query.prepare("select Name, Price from food where No = ?");
    query.bindValue(0, foodNo);
    query.exec();
    query.first();
    QSqlRecord record = query.record();

    signalPri = record.value("Price").toFloat();
    ui->labName->setText(record.value("Name").toString());
    ui->labPrice->setText(QString::asprintf("￥%.2f", signalPri));
}

void ShopCarItemW::on_spinBox_valueChanged(int arg1)  //加减物品
{
    //更新商品价格
    ui->labPrice->setText(QString::asprintf("￥%.2f", signalPri*arg1));

    //让主窗口更新总价与差价
    emit spinCarItem(carNo, arg1);
}
