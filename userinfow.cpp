#include "userinfow.h"
#include "ui_userinfow.h"

UserInfoW::UserInfoW(QWidget *parent, int userNo) :
    QDialog(parent),
    ui(new Ui::UserInfoW)
{
    ui->setupUi(this);

    //报窗口设置错误
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

    this->userNo = userNo;
    setValues();  //设置界面信息

    //QSS引入
    QDir qssdir = QApplication::applicationDirPath();
    QString qsspath = qssdir.path() + "/allPages.qss";
    QFile qssFile(qsspath);
    qssFile.open(QFile::ReadOnly);
    QTextStream stream(&qssFile);
    QString stylesheet = stream.readAll();
    this->setStyleSheet(stylesheet);
    qssFile.close();

    ui->labRepairHead->installEventFilter(this);
    ui->labRInfo->installEventFilter(this);
}

UserInfoW::~UserInfoW()
{
    delete ui;
}

void UserInfoW::paintEvent(QPaintEvent *event)  //绘制
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

bool UserInfoW::eventFilter(QObject *watched, QEvent *event)  //事件过滤
{
    //修改头像
    if(watched == ui->labRepairHead){
        if(event->type() == QEvent::MouseButtonPress){
            //选择图片
            QString path = QFileDialog::getOpenFileName(this, "选择一张图片", "", "图片文件(*.jpg *.png)");
            if(path.isEmpty()){
                return QWidget::eventFilter(watched, event);
            }else{
                //加载
                QFile file(path);
                file.open(QIODevice::ReadOnly);
                QByteArray bytePix = file.readAll();
                //装入数据库
                QSqlQuery query;
                query.prepare("update userInfo set Head = ? where No = ?");
                query.bindValue(0, bytePix);
                query.bindValue(1, userNo);
                query.exec();
                //更新界面
                setValues();
            }
        }
    }

    //修改资料
    if(watched == ui->labRInfo){
        if(event->type() == QEvent::MouseButtonPress){
            ui->editName->setEnabled(true);
            ui->editProvince->setEnabled(true);
            ui->editCity->setEnabled(true);
            ui->comSex->setEnabled(true);
            ui->BtnSure->setEnabled(true);
        }
    }

    return QWidget::eventFilter(watched, event);
}

void UserInfoW::setValues()  //设置属性
{
    //查询
    QSqlQuery query;
    QSqlRecord record;
    query.prepare("select * from userInfo where No = ?");
    query.bindValue(0, userNo);
    query.exec();
    query.first();
    record = query.record();

    //设置
    ui->labAccount->setText(record.value("Account").toString());
    ui->editName->setText(record.value("NickName").toString());
    ui->editProvince->setText(record.value("Province").toString());
    ui->editCity->setText(record.value("City").toString());

    if(record.value("Gender").toInt() == 1){
        ui->comSex->setCurrentIndex(0);
    }else{
        ui->comSex->setCurrentIndex(1);
    }

    QByteArray byt = record.value("Head").toByteArray();
    QPixmap map;
    map.loadFromData(byt);
    ui->labPic->setPixmap(map.scaled(0.3*height(), 0.3*height()));
}

void UserInfoW::on_BtnSure_clicked()  //提交修改资料
{
    //获取输入框的值
    QString name = ui->editName->text();
    QString province = ui->editProvince->text();
    QString city = ui->editCity->text();
    int sex = ui->comSex->currentIndex()==1?0:1;

    //存入数据库
    QSqlQuery query;
    query.prepare("update userInfo set NickName = ?, Province = ?, City = ?, Gender = ? where No = ?");
    query.bindValue(0, name);
    query.bindValue(1, province);
    query.bindValue(2, city);
    query.bindValue(3, sex);
    query.bindValue(4, userNo);
    query.exec();

    //失能
    ui->editName->setEnabled(false);
    ui->editProvince->setEnabled(false);
    ui->editCity->setEnabled(false);
    ui->comSex->setEnabled(false);
    ui->BtnSure->setEnabled(false);

    //更新界面
    setValues();
}
