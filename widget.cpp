#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <iostream>
#include <QFileDialog>

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


void Widget::on_pushButton_clear_clicked()
{
    ui->Canvas->clear();
}

void Widget::on_pushButton_simplify_clicked()

{
    //Get pols
    std::vector<QPolygon> polygons = ui->Canvas->getPolygons();

    //Create enclosing rectangle
    Algorithms a;
    std::vector<QPolygon> ers, chs;
    std::vector<QPoint> points;

    if (ui->comboBox->currentIndex()==1)
    {
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            QPolygon pol = polygons[i];
            points.clear();
            for (int j = 0; j < pol.size(); j++)
            {
                points.push_back(pol[j]);
            }

        //Minimum area enclosing rectangle + Convex hull
        QPolygon ch = a.cHull(points); //ch pak smazat
        QPolygon er = a.wallAverage(points);

        ers.push_back(er);
        chs.push_back(ch);

        }
        ui->Canvas->setEr(ers);
        ui->Canvas->setCh(chs);

    }

    else
    {
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            QPolygon pol = polygons[i];
            points.clear();
            for (int j = 0; j < pol.size(); j++)
            {
                points.push_back(pol[j]);
            }

        //Minimum area enclosing rectangle + Convex hull
        QPolygon ch = a.cHull(points); //ch pak smazat
        QPolygon er = a.minAreaEnclosingRectangle(points);

        ers.push_back(er);
        chs.push_back(ch);

        }
        ui->Canvas->setEr(ers);
        ui->Canvas->setCh(chs);

    };

    //Update enclosing rectangle
    //ui->Canvas->setEr(er);

    //Repaint
    repaint();
}

void Widget::on_pushButton_loadData_clicked()
{
    QString file_name = "C:/Users/monik/OneDrive/Documents/GitHub/ADK_uloha2/ADK_uloha2/buildings.txt";
    //QString file_name = QFileDialog::getOpenFileName(this, tr("Open Text file"), "", tr("Text Files (*.txt)"));
    //QFileInfo fileinfo(file_name);
    ui->Canvas->loadData(file_name);
}

