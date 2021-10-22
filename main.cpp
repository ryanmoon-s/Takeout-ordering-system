#include "mainw.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //打开数据库，打开它的窗口释放了，也就关闭了。
    QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE"); //加载驱动
    QDir dir = QApplication::applicationDirPath();
    QString path = dir.path() + "/uglyTakeoutFood.db";
    DB.setDatabaseName(path);
    if(!DB.open()){
        qDebug() << "打开数据库失败";
        exit(1);
    }

    MainW *w = new MainW();
    //加载登录界面
    LoginW *login = new LoginW();
    QWidget::connect(login, SIGNAL(whoLogin(int)), w, SLOT(whoLogin(int)));
    int ret = login->exec();
    if(ret == QDialog::Rejected)
        exit(1);

    //加载主界面
    w->show();
    return a.exec();
}
