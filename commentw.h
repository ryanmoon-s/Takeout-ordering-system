#ifndef COMMENTW_H
#define COMMENTW_H

#include <QDialog>
#include <QtWidgets>
#include <QtSql>

namespace Ui {
class CommentW;
}

class CommentW : public QDialog
{
    Q_OBJECT

public:
    explicit CommentW(QWidget *parent = nullptr, int orderNo = 0);
    ~CommentW();

protected:
    void paintEvent(QPaintEvent *event) override;


private slots:
    void on_BtnSure_clicked();

private:
    Ui::CommentW *ui;

    int orderNo;
};

#endif // COMMENTW_H
