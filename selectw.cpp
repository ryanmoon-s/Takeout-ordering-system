#include "selectw.h"
#include "ui_selectw.h"

SelectW::SelectW(QWidget *parent, QString price, int foodNo, QString foodName) :
    QWidget(parent),
    ui(new Ui::SelectW)
{
    ui->setupUi(this);
    ui->labSelPrice->setText(price);  //价格
    this->foodNo = foodNo;
    ui->lab_Sel->setText(ui->btnSel1->text() + "、" + ui->btnSel2->text());  //已选规格初始值
    ui->labName->setText(foodName);

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

SelectW::~SelectW()
{
    delete ui;
}

void SelectW::paintEvent(QPaintEvent *event)  //绘制
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

    //painter.drawRect(rect);
    painter.drawRoundRect(rect,5 , 5);

    /***********************已选规格**********************/
    int gridS_H = ui->gridSele->geometry().height();
    int gridT_H = ui->gridTotal->geometry().height();
    int can_H = ui->btnCancel->height();
    rect.setRect(0, height() - gridS_H - gridT_H - can_H - 28, width(), gridS_H);

    brushCol.setRgba(qRgba(171, 187, 198, 230));
    brush.setColor(brushCol);
    painter.setBrush(brush);

    painter.drawRect(rect);
}


void SelectW::on_btnCancel_clicked()  //取消
{
    close();
}

void SelectW::on_btnSel2_clicked()
{
    ui->btnSel3->setChecked(false);
    ui->btnSel4->setChecked(false);
    ui->btnSel5->setChecked(false);
    ui->btnSel6->setChecked(false);
    hot = 1;

    ui->lab_Sel->setText(ui->btnSel1->text() + "、" + ui->btnSel2->text());
}

void SelectW::on_btnSel3_clicked()
{
    ui->btnSel2->setChecked(false);
    ui->btnSel4->setChecked(false);
    ui->btnSel5->setChecked(false);
    ui->btnSel6->setChecked(false);
    hot = 2;

    ui->lab_Sel->setText(ui->btnSel1->text() + "、" + ui->btnSel3->text());
}

void SelectW::on_btnSel4_clicked()
{
    ui->btnSel3->setChecked(false);
    ui->btnSel2->setChecked(false);
    ui->btnSel5->setChecked(false);
    ui->btnSel6->setChecked(false);
    hot = 3;

    ui->lab_Sel->setText(ui->btnSel1->text() + "、" + ui->btnSel4->text());
}

void SelectW::on_btnSel5_clicked()
{
    ui->btnSel3->setChecked(false);
    ui->btnSel4->setChecked(false);
    ui->btnSel2->setChecked(false);
    ui->btnSel6->setChecked(false);
    hot = 4;

    ui->lab_Sel->setText(ui->btnSel1->text() + "、" + ui->btnSel5->text());
}

void SelectW::on_btnSel6_clicked()
{
    ui->btnSel3->setChecked(false);
    ui->btnSel4->setChecked(false);
    ui->btnSel5->setChecked(false);
    ui->btnSel2->setChecked(false);
    hot = 5;

    ui->lab_Sel->setText(ui->btnSel1->text() + "、" + ui->btnSel6->text());
}

void SelectW::on_btnSel1_clicked()
{
    ui->btnSel1->setChecked(true);
}

void SelectW::on_btnAddCar_clicked()  //加入购物车
{
    emit updateShopCar(foodNo, hot);
}
