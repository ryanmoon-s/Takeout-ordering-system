#include "forgetpassw.h"
#include "ui_forgetpassw.h"

ForgetPassW::ForgetPassW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgetPassW)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);  //关闭销毁

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

ForgetPassW::~ForgetPassW()
{
    delete ui;
}

void ForgetPassW::paintEvent(QPaintEvent *event)  //绘图
{

    Q_UNUSED(event);
    QPainter painter(this);

    //底层背景
    QRect rect(0, 0, width(), height());
    QPixmap map(":/new/prefix1/res/findPassbk.jpg");
    painter.drawPixmap(rect, map);

    //灰布
    QRect rect2(0.1*width(), 0.1*height(), 0.8*width(), 0.8*height());

    QPen pen(Qt::NoPen);
    painter.setPen(pen);

    QBrush brush2;
    QColor brushCol(213, 213, 238, 90); //构造时可以rgba
    brush2.setColor(brushCol);
    brush2.setStyle(Qt::SolidPattern);
    painter.setBrush(brush2);

    painter.drawRoundRect(rect2, 4, 4);  //画带圆角的矩形


}

void ForgetPassW::on_BtnSure_clicked()  //提交找回密码
{
    //获取编辑框内容
    QString Fuser = ui->editUser->text();
    QString Fpay = ui->editPay->text();
    QString Fpass = ui->editPass->text();
    QString Frepass = ui->editRePass->text();

    if(Fuser.isEmpty() || Fpay.isEmpty() || Fpass.isEmpty() || Frepass.isEmpty()){
        QMessageBox::warning(this, "警告", "所有字段必填！请检查！");
        return;
    }

    //查询账号
    QSqlQuery query;
    query.prepare("select Account, PayPass from userInfo where Account = ?");
    query.bindValue(0, Fuser);
    query.exec();  //只检测语法错误
    query.first();
    if(!query.isValid()){  //从是否有结果 检查 是否有此账号
        QMessageBox::warning(this, "警告", "无此账号！请检查！");
        return;
    }

    //检查支付密码
    QSqlRecord record = query.record();
    QString storePay = record.value("PayPass").toString();
    if(Fpay != storePay){
        QMessageBox::warning(this, "警告", "支付密码错误！请检查！");
        return;
    }

    //密码是否合法
    int digitCount = 0, letterCount = 0;
    for (int i = 0; i < Fpass.size(); i++) {
        if(Fpass.at(i).isDigit())
            digitCount++;
        if(Fpass.at(i).isLetter())
            letterCount++;
    }
    if(digitCount == 0 || letterCount == 0){
        QMessageBox::warning(this, "警告", "密码格式不正确，请检查！");
        return;
    }

    //两次密码是否一致
    if(Fpass != Frepass){
        QMessageBox::warning(this, "警告", "两次密码输入不一致！请检查！");
        return;
    }

    //提示是否修改
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this, "询问", "新密码: " + Fpass + "\n是否修改？", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(result == QMessageBox::No){
        close();
        return;
    }else if(result == QMessageBox::Cancel){
        return;
    }

    //存入数据库
    QString encryPass = RegisterW::encrypt(Fpass);  //加密
    QByteArray bytePass;
    bytePass.append(encryPass);
    query.prepare("update userInfo set Password = ? where Account = ?");
    query.bindValue(0, bytePass);
    query.bindValue(1, Fuser);

    //提示修改成功
    QMessageBox::information(this, "提示", "密码修改成功！将为您自动跳转");
    emit successful(Fuser, Fpass);
    close();
}
