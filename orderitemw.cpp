#include "orderitemw.h"
#include "ui_orderitemw.h"

OrderItemW::OrderItemW(QWidget *parent, int orderNo) :
    QWidget(parent),
    ui(new Ui::OrderItemW)
{
    ui->setupUi(this);

    this->orderNo = orderNo;
    setValues(); //设置界面组件值

    //QSS引入
    QDir qssdir = QApplication::applicationDirPath();
    QString qsspath = qssdir.path() + "/allPages.qss";
    QFile qssFile(qsspath);
    qssFile.open(QFile::ReadOnly);
    QTextStream stream(&qssFile);
    QString stylesheet = stream.readAll();
    this->setStyleSheet(stylesheet);
    qssFile.close();
}

OrderItemW::~OrderItemW()
{
    delete ui;
}

void OrderItemW::setValues()  //设置界面组件值
{
    //查询order
    QSqlQuery queryOrder;
    QSqlRecord recordOrder;
    queryOrder.prepare("select * from orderAgent where No = ?");
    queryOrder.bindValue(0, orderNo);
    queryOrder.exec();
    queryOrder.first();
    recordOrder = queryOrder.record();

    //查询shopInfo
    QSqlQuery queryShop;
    QSqlRecord recordShop;
    queryShop.prepare("select Name, Picture from shopInfo where No = ?");
    queryShop.bindValue(0, recordOrder.value("shopNo"));
    queryShop.exec();
    queryShop.first();
    recordShop = queryShop.record();

    //设置值
    ui->labName->setText(recordShop.value("Name").toString());
    ui->labTime->setText(recordOrder.value("Time").toString());
    ui->labPrice->setText(QString::asprintf("￥%.2f", recordOrder.value("TotalPrice").toFloat()));

    QByteArray byte = recordShop.value("Picture").toByteArray();
    QPixmap map;
    map.loadFromData(byte);
    ui->labPic->setPixmap(map.scaled(0.5*height(), 0.5*height()));

    if(recordOrder.value("DeliveryState").toInt() == 0){
        ui->labState->setText("配送中");
        ui->btnComm->setVisible(false);
    }else{
        ui->labState->setText("已送达");
        ui->BtnSure->setVisible(false);
        //设置评论按钮是否显示
        QSqlQuery query;
        query.prepare("select CommentFlag from orderAgent where No = ?");
        query.bindValue(0, orderNo);
        query.exec();
        query.first();
        if(query.record().value("CommentFlag").toInt() == 0){
            ui->btnComm->setVisible(true);
        }else{
            ui->btnComm->setVisible(false);
        }
    }
}

void OrderItemW::on_BtnSure_clicked()  //收货
{
    emit receOk(orderNo);
}

void OrderItemW::on_btnComm_clicked()  //评价
{
    CommentW *co = new CommentW(this, orderNo);
    int ret = co->exec();

    if(ret == QDialog::Accepted){
        //更新订单信息
        setValues();
    }
}
