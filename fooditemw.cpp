#include "fooditemw.h"
#include "ui_fooditemw.h"

FoodItemW::FoodItemW(QWidget *parent, int foodNo) :
    QWidget(parent),
    ui(new Ui::FoodItemW)
{
    ui->setupUi(this);
    this->foodNo = foodNo;
    setValues(); //页面组件赋值

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

FoodItemW::~FoodItemW()
{
    delete ui;
}

void FoodItemW::setValues()  //页面组件赋值
{
    //查询
    QSqlQuery query;
    query.prepare("select Picture, Name, MonthSale, Price from food where No = ?");
    query.bindValue(0, foodNo);
    query.exec();
    query.first();
    QSqlRecord record = query.record();

    //赋值
    QPixmap map;
    map.loadFromData(record.value("Picture").toByteArray());
    ui->labPic->setPixmap(map.scaled(0.88*height(), height()));
    ui->labName->setText(record.value("Name").toString());
    ui->labMon->setText(QString::asprintf("月售: %d 份", record.value("MonthSale").toUInt()));
    ui->labPrice->setText(QString::asprintf("%.2f", record.value("Price").toFloat()));
    price = QString::asprintf("￥ %.2f", record.value("Price").toFloat());
    foodName = record.value("Name").toString();
}

void FoodItemW::on_btnSelectSize_clicked()  //选规格
{
    SelectW *sel = new SelectW(nullptr, price, foodNo, foodName); //独立窗口，不能传父窗口给它了
    //connect让主对话框更新购物车，但是这已经涉及非常多的窗口，找不到主窗口指针了
    foreach(QWidget *w, qApp->topLevelWidgets()){
        if(QMainWindow *mainwin = qobject_cast<QMainWindow*>(w)){  //转换成QMainWindow(成功返回true)，再赋值给mainwin
            connect(sel, SIGNAL(updateShopCar(int, int)), mainwin, SLOT(updateShopCar(int, int)));
            break;
        }
    }
    sel->setAttribute(Qt::WA_DeleteOnClose); //退出即释放
    sel->setWindowFlags(Qt::FramelessWindowHint);  //无边框，独立窗口
    sel->setWindowModality(Qt::ApplicationModal);  //对于应用来说是模态的
    sel->show();
}
