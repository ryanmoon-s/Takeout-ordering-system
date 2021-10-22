#include "shopitemw.h"
#include "ui_shopitemw.h"


ShopItemW::ShopItemW(QWidget *parent, int shopNo) :
    QWidget(parent),
    ui(new Ui::ShopItemW)
{
    ui->setupUi(this);
    this->shopNo = shopNo;  //店铺编号
    setValues(); //赋值

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

ShopItemW::~ShopItemW()
{
    delete ui;
}

void ShopItemW::on_BtnSure_clicked()  //进店
{
    emit addShopTab(shopNo);  //发送信号，让主窗口建立新的店铺tab
}

void ShopItemW::setValues() //赋值
{
    //查询
    QSqlQuery query;
    query.prepare("select * from shopInfo where No = ?");
    query.bindValue(0, shopNo);
    query.exec();
    query.first();
    QSqlRecord record = query.record();
    QByteArray pic = record.value("Picture").toByteArray();
    QString name = record.value("Name").toString();
    int isbook = record.value("IsBook").toInt();
    int isrest = record.value("IsRest").toInt();
    float beg = record.value("DeleveryBeginPrice").toFloat();

    //赋值
    ui->labName->setText(name);
    ui->labBegin->setText(QString::asprintf("￥ %.2f", beg));
    if(isrest == 0){
        ui->labRest->setText("营业中");
        ui->labRest->setStyleSheet("color:green");
        ui->labBook->hide();//营业中，不展示是否预定
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
    ui->labelPic->setPixmap(map.scaled(0.9*height(), height()));
}
