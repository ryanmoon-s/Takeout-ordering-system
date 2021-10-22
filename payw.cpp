#include "payw.h"
#include "ui_payw.h"

PayW::PayW(QWidget *parent, int userNo, QString extra) :
    QDialog(parent),
    ui(new Ui::PayW)
{
    ui->setupUi(this);

    this->userNo = userNo;
    ui->labExtra->setText(extra);

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

PayW::~PayW()
{
    delete ui;
}

void PayW::paintEvent(QPaintEvent *event)  //绘制
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

void PayW::on_btnPay_clicked()  //确认付款
{
    //取出支付密码
    QString paypass = ui->lineEdit->text();

    //验证支付密码
    QSqlQuery query;
    QSqlRecord record;
    query.prepare("select PayPass from userInfo where No = ?");
    query.bindValue(0, userNo);
    query.exec();
    query.first();
    record = query.record();
    if(paypass != record.value("PayPass")){
        QMessageBox::critical(this, "错误!", "支付密码输入错误");
        return;
    }

    //让主对话框下单
    emit payOk();
    accept();
}
