#include "fooddetailw.h"
#include "ui_fooddetailw.h"

FoodDetailW::FoodDetailW(QWidget *parent, int foodNo) :
    QDialog(parent),
    ui(new Ui::FoodDetailW)
{
    ui->setupUi(this);
    this->foodNo = foodNo;

    //报窗口设置错误
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

    setValues();  //设置界面

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

FoodDetailW::~FoodDetailW()
{
    delete ui;
}

void FoodDetailW::paintEvent(QPaintEvent *event)  //绘制
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

    brushCol.setRgba(qRgba(171, 187, 198, 150));
    brush.setColor(brushCol);
    painter.setBrush(brush);

    painter.drawRect(rect);
}

void FoodDetailW::setValues()  //设置界面
{
    //查询
    QSqlQuery query;
    QSqlRecord record;
    query.prepare("select * from food where No = ?");
    query.bindValue(0, foodNo);
    query.exec();
    query.first();
    record = query.record();

    //设置值
    QByteArray pic = record.value("Picture").toByteArray();
    QString name = record.value("Name").toString();
    int people = record.value("PeopleNum").toInt();
    float price = record.value("Price").toFloat();
    QString taste = record.value("Taste").toString();
    QString des = record.value("Describle").toString();
    int hunsu = record.value("MeatOrVeg").toInt();
    QString mete = record.value("MainMaterial").toString();
    QString how = record.value("HowToMake").toString();
    int monthsale = record.value("MonthSale").toInt();

    QPixmap map;
    map.loadFromData(pic);
    ui->labPic->setPixmap(map.scaled(0.3*height(), 0.3*height()));
    ui->labTaste->setText(taste);
    ui->labDes->setText(des);
    ui->labMain->setText(mete);
    ui->labMake->setText(how);
    ui->labName->setText(name);
    ui->labPrice->setText(QString::asprintf("￥%.2f", price));
    ui->labPeople->setText(QString::asprintf("%d 人",people));
    if(hunsu == 1){
        ui->labHunsu->setText("荤");
    }else{
        ui->labHunsu->setText("素");
    }
    ui->labMonth->setText(QString::asprintf("%d 份", monthsale));
}

