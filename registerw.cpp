#include "registerw.h"
#include "ui_registerw.h"

RegisterW::RegisterW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterW)
{
    ui->setupUi(this);

    //为label安装事件过滤器--点击事件
    ui->labExit->installEventFilter(this);

    //编辑框默认文字
    ui->editPhone->setPlaceholderText("11位手机号码");
    ui->editPass->setPlaceholderText("8-12位数字与英文组合");
    ui->editPay->setPlaceholderText("6位纯数字");
    ui->editNickName->setPlaceholderText("至多10个汉字");
    ui->editProvince->setPlaceholderText("订餐省份");
    ui->editCity->setPlaceholderText("订餐城市");

    //窗口参数
    //由于不需要向上级返回accept or reject ，所以设置参数，退出就销毁窗口(而不是隐藏)，释放内存   退出时close
    setAttribute(Qt::WA_DeleteOnClose);

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

RegisterW::~RegisterW()
{
    delete ui;
}

void RegisterW::paintEvent(QPaintEvent *event)  //绘图
{
    Q_UNUSED(event);
    QPainter painter(this);

    //底层背景
    QRect rect(0, 0, width(), height());
    QPixmap map(":/new/prefix1/res/register.jpg");
    painter.drawPixmap(rect, map);

    //二层蒙板
    QRect secondRec(0.00474*width(), 0.0048*height(), 0.993*width(), 0.993*height());
    QPen SecondPen(Qt::NoPen);
    painter.setPen(SecondPen);
    QBrush secondBrush;
    QColor secondCol(125,125,125,150);
    secondBrush.setStyle(Qt::SolidPattern);
    secondBrush.setColor(secondCol);
    painter.setBrush(secondBrush);
    painter.drawRect(secondRec);
}

bool RegisterW::eventFilter(QObject *watched, QEvent *event)  //事件过滤
{
    //已有账号，立即登录
    if(watched == ui->labExit){
        if(event->type() == QEvent::MouseButtonPress){
            this->close();  //销毁窗口，释放内存
        }
    }
    //交给父类处理
    return QWidget::eventFilter(watched, event);
}

QString RegisterW::encrypt(const QString &str)  //MD5加密
{
    QByteArray byte;
    byte.append(str);

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(byte);
    QByteArray result = hash.result();
    return result.toHex();
}

void RegisterW::on_BtnSure_clicked()  //注册按钮
{
    //获取编辑框信息
    QString Fuser = ui->editPhone->text();
    QString Fpass = ui->editPass->text();
    QString Fpay = ui->editPay->text();
    QString Fnick = ui->editNickName->text();
    QString Fpro = ui->editProvince->text();
    QString Fcity = ui->editCity->text();
    int gender;
    gender = ui->radioBoy->isChecked()?1:0;

    //输入检查
    if(!CheckInfo(Fuser, Fpass, Fpay, Fnick, Fpro, Fcity))
        return;

    //询问是否保存
    QMessageBox::StandardButton result;
    QString content = "账号: " + Fuser + "\n密码: " + Fpass + "\n支付密码: " + Fpay + "\n是否确定注册？";
    result = QMessageBox::question(this, "询问", content, QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(result == QMessageBox::No){
        close();  //不管怎么关闭，都要把这个函数执行完所以加个return         最好还是close+deleteOnClose 释放内存
        return;
    }else if(result == QMessageBox::Cancel){
        return;
    }

    //检查是否已有此账号
    QSqlQuery queryAcc;
    queryAcc.prepare("select Account from userInfo where Account = :ACC");
    queryAcc.bindValue(":ACC", Fuser);
    queryAcc.exec();
    if(queryAcc.isValid()){
        QMessageBox::warning(this, "警告", "此账号已被注册！请找回密码！");
        return;
    }

    //密码加密
    QString encryPass = encrypt(Fpass);
    QByteArray bytePass;
    bytePass.append(encryPass);

    //存入数据库
    QSqlQuery query;
    query.prepare("insert into userInfo values(NULL, ?, ?, ?, ?, ?, ?, ?, NULL)");
    query.bindValue(0, Fuser);
    query.bindValue(1, bytePass);
    query.bindValue(2, Fpay);
    query.bindValue(3, Fnick);
    query.bindValue(4, gender);
    query.bindValue(5, Fpro);
    query.bindValue(6, Fcity);
    query.exec();  //只检测语法错误
    //isvalid只有查询语句时才有效？  多半是没有query.first()

    //成功提示
    QMessageBox::information(this, "提示", "注册成功！将为您自动跳转");
    emit successful(Fuser, Fpass);
    close();
}

bool RegisterW::CheckInfo(QString account, QString pass, QString pay, QString nick, QString pro, QString city)  //信息错误检查
{
    //账号
    if(account.isEmpty()){
        QMessageBox::warning(this, "警告", "账号为空，请检查！");
        return false;
    }
    for (int i = 0; i < account.size(); i++) {
        if(!account.at(i).isDigit()){
            QMessageBox::warning(this, "警告", "账号有非数字内容，请检查！");
            return false;
        }
    }
    if(account.size() != 11){
        QMessageBox::warning(this, "警告", "账号长度错误，请检查！");
        return false;
    }

    //密码
    if(pass.isEmpty()){
        QMessageBox::warning(this, "警告", "密码为空，请检查！");
        return false;
    }
    int digitCount = 0, letterCount = 0;
    for (int i = 0; i < pass.size(); i++) {
        if(pass.at(i).isDigit())
            digitCount++;
        if(pass.at(i).isLetter())
            letterCount++;
    }
    if(digitCount == 0 || letterCount == 0){
        QMessageBox::warning(this, "警告", "密码格式不正确，请检查！");
        return false;
    }

    //支付密码
    if(pay.isEmpty()){
        QMessageBox::warning(this, "警告", "支付密码为空，请检查！");
        return false;
    }
    for (int i = 0; i < pay.size(); i++) {
        if(!pay.at(i).isDigit()){
            QMessageBox::warning(this, "警告", "支付密码有非数字内容，请检查！");
            return false;
        }
    }
    if(pay.size() != 6){
        QMessageBox::warning(this, "警告", "支付密码长度错误，请检查！");
        return false;
    }

    //个性名称
    if(nick.isEmpty()){
        QMessageBox::warning(this, "警告", "个性昵称为空，请检查！");
        return false;
    }
    if(nick.size() > 10){  //unicode编码，汉字占一位
        QMessageBox::warning(this, "警告", "个性昵称过长，请检查！");
        return false;
    }

    //省份
    if(pro.isEmpty()){
        QMessageBox::warning(this, "警告", "省份为空，请检查！");
        return false;
    }
    if(pro.size() > 10){  //unicode编码，汉字占一位
        QMessageBox::warning(this, "警告", "省份过长，请检查！");
        return false;
    }

    //城市
    if(city.isEmpty()){
        QMessageBox::warning(this, "警告", "城市为空，请检查！");
        return false;
    }
    if(city.size() > 10){  //unicode编码，汉字占一位
        QMessageBox::warning(this, "警告", "城市过长，请检查！");
        return false;
    }

    //是否同意用户协议
    if(!ui->checkAgree->isChecked()){
        QMessageBox::warning(this, "警告", "您还未勾选同意用户协议，请检查！");
        return false;
    }

    return true;
}
