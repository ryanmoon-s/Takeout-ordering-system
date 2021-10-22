#include "commentw.h"
#include "ui_commentw.h"

CommentW::CommentW(QWidget *parent, int orderNo) :
    QDialog(parent),
    ui(new Ui::CommentW)
{
    ui->setupUi(this);
    this->orderNo = orderNo;


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

CommentW::~CommentW()
{
    delete ui;
}

void CommentW::paintEvent(QPaintEvent *event)  //绘制
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

void CommentW::on_BtnSure_clicked()  //评价提交
{
    //获取内容
    QString contents = ui->textEdit->toPlainText();
    if(contents.isEmpty())
        QMessageBox::warning(this, "警告", "请输入评价内容");

    //加入数据库
    QSqlQuery query;
    QSqlRecord record;

    //获取shopNo
    query.prepare("select shopNo, UserNo from orderAgent where No = ?");
    query.bindValue(0, orderNo);
    query.exec();
    query.first();
    int shopNo = query.record().value("ShopNo").toInt();
    int userNo = query.record().value("UserNo").toInt();

    //存入评价内容
    query.prepare("insert into comment values(NULL, ?, ?, ?)");
    query.bindValue(0, contents);
    query.bindValue(1, shopNo);
    query.bindValue(2, userNo);
    query.exec();

    //更新orderAgent的是否评价Flag
    query.prepare("update orderAgent set CommentFlag = 1 where No = ?");
    query.bindValue(0, orderNo);
    query.exec();

    //提示
    QMessageBox::information(this, "提示", "评价成功!");

    accept();
}
