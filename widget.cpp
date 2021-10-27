#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clear();
}

void Widget::on_pushButton_clicked()
{
    //Get points
    std::vector<QPoint> points = ui->Canvas->getPoints();

    //Create enclosing rectangle
    Algorithms a;
    QPolygon er;
    if (ui->comboBox->currentIndex()==0)
    {
        //Minimum area enclosing rectangle
        QPolygon ch = a.cHull(points);
        er = a.minAreaEnclosingRectangle(points);
        ui->Canvas->setCh(ch);
    }

    else
    {
        //Wall average
        er = a.wallAverage(points);
    }

    //Update enclosing rectangle
    ui->Canvas->setEr(er);

    //Repaint
    repaint();
}
