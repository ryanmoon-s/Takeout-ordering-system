#include "addressconw.h"
#include "ui_addressconw.h"

AddressConW::AddressConW(QWidget *parent, int userNo) :
    QDialog(parent),
    ui(new Ui::AddressConW)
{
    ui->setupUi(this);
    this->userNo = userNo;
    loadAddress();  //加载地址

    //QSS引入
    QDir qssdir = QApplication::applicationDirPath();
    QString qsspath = qssdir.path() + "/allPages.qss";
    QFile qssFile(qsspath);
    qssFile.open(QFile::ReadOnly);
    QTextStream stream(&qssFile);
    QString stylesheet = stream.readAll();
    this->setStyleSheet(stylesheet);
    qssFile.close();

    //隐藏设置地址界面
    ui->tabWidget->setTabEnabled(1, false);

    ui->labAddAddr->installEventFilter(this);
}

AddressConW::~AddressConW()
{
    delete ui;
}

bool AddressConW::eventFilter(QObject *watched, QEvent *event)  //事件过滤
{
    //新增地址
    if(watched == ui->labAddAddr){
        if(event->type() == QEvent::MouseButtonPress){
            ui->plainAddr->clear();
            ui->tabWidget->setTabEnabled(1, true);
            ui->tabWidget->setCurrentIndex(1);
            whatFlag = 0;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void AddressConW::paintEvent(QPaintEvent *event)  //绘制
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

void AddressConW::loadAddress() //加载地址
{
    ui->listWidget->clear();

    //查询
    QSqlQuery query;
    QSqlRecord record;
    query.prepare("select No, Address from address where userNo = ?");
    query.bindValue(0, userNo);
    query.exec();
    query.first();
    if(!query.isValid())
        return;
    record = query.record();

    //添加
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(record.value("Address").toString());
    item->setData(Qt::WhatsThisRole, record.value("No").toInt());
    ui->listWidget->addItem(item);
    while(query.next()){
        record = query.record();
        item = new QListWidgetItem();
        item->setData(Qt::WhatsThisRole, record.value("No").toInt());
        item->setText(record.value("Address").toString());
        ui->listWidget->addItem(item);
    }
}

void AddressConW::on_listWidget_itemDoubleClicked(QListWidgetItem *item)  //item双击，编辑地址
{
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setCurrentIndex(1);
    ui->plainAddr->clear();
    ui->plainAddr->appendPlainText(item->text());
    whatFlag = item->data(Qt::WhatsThisRole).toInt();  //具体编号
}

void AddressConW::on_BtnSure_clicked()  //确认修改
{
    if(ui->plainAddr->document()->isEmpty()){
        QMessageBox::warning(this, "警告", "请输入有效地址!");
        return;
    }
    QString address = ui->plainAddr->toPlainText();

    QSqlQuery query;
    QSqlRecord record;

    if(whatFlag == 0){
        //新增地址
        query.prepare("insert into address values(NULL, ? , ?)");
        query.bindValue(0, address);
        query.bindValue(1, userNo);
        query.exec();
    }else{
        //修改地址
        query.prepare("update address set Address = ? where No =  ?");
        query.bindValue(0, address);
        query.bindValue(1, whatFlag);
        query.exec();
    }

    //重新更新List
    loadAddress();
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setCurrentIndex(0);
}

void AddressConW::on_listWidget_customContextMenuRequested(const QPoint &pos)  //右键菜单
{
    Q_UNUSED(pos);
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->acEdit);
    menu->addAction(ui->acDel);
    menu->exec(QCursor::pos());  //在鼠标位置起一个菜单
}

void AddressConW::on_acDel_triggered()  //actDel删除地址
{
    QSqlQuery query;
    QSqlRecord record;

    query.prepare("delete from address where No = ?");
    query.bindValue(0, ui->listWidget->currentItem()->data(Qt::WhatsThisRole).toInt());
    query.exec();


    //重新更新List
    loadAddress();
}

void AddressConW::on_acEdit_triggered()  //acEdit 编辑地址
{
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setCurrentIndex(1);
    ui->plainAddr->clear();
    ui->plainAddr->appendPlainText(ui->listWidget->currentItem()->text());
    whatFlag = ui->listWidget->currentItem()->data(Qt::WhatsThisRole).toInt();  //具体编号
}

void AddressConW::on_tabWidget_currentChanged(int index)  //切换tab时，将编辑失能
{
    if(ui->tabWidget->currentIndex() == 0 && index == 0){
        ui->tabWidget->setTabEnabled(1, false);
    }
}
