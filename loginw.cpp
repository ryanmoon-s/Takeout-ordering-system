#include "loginw.h"
#include "ui_loginw.h"

LoginW::LoginW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginW)
{
    ui->setupUi(this);

    //窗口设定
    setFixedSize(width(), height());  //固定大小
    QDesktopWidget *desktop = QApplication::desktop();
    setGeometry((desktop->width() - width())/2, (desktop->height() - height())/2, width(), height());  //窗口居中

    //输入框初始化
    ui->editUser->setPlaceholderText("user...");     //占位文字
    ui->editPass->setPlaceholderText("password...");

    //记住密码读取
    QFile file(QApplication::applicationDirPath() + "temp");
    if(file.exists()){
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream stream(&file);
        QString str = stream.readAll();
        ui->editUser->setText(str.section(" ", 0, 0));  //空格之前
        ui->editPass->setText("123456789");  //乱输入的，占位用
        PSWD = str.section(" ", 1, 1);  //空格之后
        ui->rememberPass->setChecked(true);
    }else
        PSWD.clear();

    //为忘记密码与注册 label 安装事件过滤器
    ui->labRegister->installEventFilter(this);
    ui->labForget->installEventFilter(this);


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

LoginW::~LoginW()
{
    delete ui;
}

void LoginW::paintEvent(QPaintEvent *event)  //绘制
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  //抗锯齿

    //底层背景
    QRect rect(0, 0, width(), height());
    QPixmap map(":new/prefix1/res/cover.jpg");
    painter.drawPixmap(rect, map);

    //登录背景2灰布
    QRect rect2(0.647*width(), 0.24*height(), 0.28*width(), 0.5*height());

    QPen pen(Qt::NoPen);
    painter.setPen(pen);

    QBrush brush2;
    QColor brushCol(213, 213, 238, 100); //构造时可以rgba
    brush2.setColor(brushCol);
    brush2.setStyle(Qt::SolidPattern);
    painter.setBrush(brush2);

    painter.drawRoundRect(rect2, 5, 5);  //画带圆角的矩形

}

bool LoginW::eventFilter(QObject *watched, QEvent *event)  //事件过滤器--忘记密码 注册账号
{
    //注册密码
    if(watched == ui->labRegister){
        if(event->type() == QEvent::MouseButtonPress){
            RegisterW *regi = new RegisterW(this);
            connect(regi, SIGNAL(successful(QString, QString)), this, SLOT(on_registerSucc(QString, QString)));
            regi->exec();
        }
    }
    //忘记密码
    if(watched == ui->labForget){
        if(event->type() == QEvent::MouseButtonPress){
            ForgetPassW *forget = new ForgetPassW(this);
            connect(forget, SIGNAL(successful(QString, QString)), this, SLOT(on_registerSucc(QString, QString)));
            forget->exec();
        }
    }
    //交给上级
    return QWidget::eventFilter(watched, event);
}

void LoginW::on_BtnSureLogin_clicked()  //登录按钮
{
    //获取对话框的值
    QString editUser = ui->editUser->text();
    QString editPass = ui->editPass->text();

    //查询
    QSqlQuery query(DB);
    query.prepare("select No, Account, Password from userInfo where Account = :ACC");
    query.bindValue(":ACC", editUser);
    query.exec();
    query.first();
    if(!query.isValid()){
        QMessageBox::critical(this, "错误", "账号错误");
        return;
    }

    //判断密码是否正确
    QSqlRecord record = query.record();
    QString DataEncryPass = record.value("Password").toString();  //获取数据库密码(加密过的)

    QString NowEncryPass = PSWD;
    if(PSWD.isEmpty()) {  //如果没有记住加密过后的密码
        NowEncryPass  = RegisterW::encrypt(editPass);  //加密新输入的密码
    }

    if(DataEncryPass == NowEncryPass){  //密码正确
        //记住密码处理
        QFile file(QApplication::applicationDirPath() + "temp");
        if(ui->rememberPass->isChecked()){
            file.open(QIODevice::Truncate|QIODevice::WriteOnly|QIODevice::Text);
            QTextStream stream(&file);
            stream << editUser + " " + NowEncryPass;  //存入账号与加密后的密码
            file.close();
        }else if (file.exists()){
            file.remove();
        }
        QMessageBox::information(this, "恭喜", "登录成功");
        //通知父窗口谁登录了
        emit whoLogin(record.value("No").toInt());
        accept();
    }else{
        PSWD.clear();  //错了一次后，不取记住的密码了
        inputCount++;
        if(inputCount >= 3){
            QMessageBox::critical(this, "错误", "您已连续输错三次密码，程序即将退出！");
            QFile file(QApplication::applicationDirPath() + "temp");
            file.remove();  //删除记住的密码文件
            reject();
        }
        QMessageBox::warning(this, "警告", "密码输入错误，请重试！");
    }
}

void LoginW::on_registerSucc(QString acc, QString pass)  //注册成功发来的信号，填充账号与密码
{
    ui->editUser->setText(acc);
    ui->editPass->setText(pass);
    ui->rememberPass->setChecked(false);  //重置状态
    PSWD.clear(); //新注册账号了，不取记住的密码
}

void LoginW::on_labRegister_linkActivated(const QString &link)
{

}
