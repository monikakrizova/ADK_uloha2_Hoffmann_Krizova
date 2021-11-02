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


void Widget::on_pushButtonClear_clicked()
{
    ui->Canvas->clear();
}

void Widget::on_pushButtonSimplify_clicked()

{
    //Get pols
    std::vector<QPolygon> polygons = ui->Canvas->getPolygons();

    //Create enclosing rectangle
    Algorithms a;
    std::vector<QPolygon> ers, chs;
    std::vector<QPoint> points;

    if (ui->comboBox->currentIndex()==1) //Wall average
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
        //QPolygon ch = a.cHull(points); //ch pak smazat
        QPolygon er = a.wallAverage(points);

        ers.push_back(er);
        //chs.push_back(ch);

        }
        ui->Canvas->setEr(ers);
        //ui->Canvas->setCh(chs);

    }

    else if (ui->comboBox->currentIndex()==0) //Min area enclosing rectangle
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
        QPolygon ch = a.cHull(points);
        QPolygon er = a.minAreaEnclosingRectangle(points);

        ers.push_back(er);
        chs.push_back(ch);

        }
        ui->Canvas->setEr(ers);
        ui->Canvas->setCh(chs);

    }
    else if (ui->comboBox->currentIndex()==2) //Longest edge
    {
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            QPolygon pol = polygons[i];
            points.clear();
            for (int j = 0; j < pol.size(); j++)
            {
                points.push_back(pol[j]);
            }

        //Longest edge
        //QPolygon ch = a.cHull(points);
        QPolygon er = a.longestEdge(points);

        ers.push_back(er);
        //chs.push_back(ch);

        }
        ui->Canvas->setEr(ers);
        //ui->Canvas->setCh(chs);
    }

    //Repaint
    points.clear();
    repaint();
}

void Widget::on_pushButtonLoadData_clicked()
{
    QString file_name = "C:/Users/monik/OneDrive/Documents/GitHub/ADK_uloha2/buildings.txt";
    //QString file_name = "C:/Users/monik/Documents/SKOLA/Ing_3.semestr/ADK/exported_pols/buildings_SJTSK.txt";
    //QString file_name = QFileDialog::getOpenFileName(this, tr("Open Text file"), "", tr("Text Files (*.txt)"));
    ui->Canvas->loadData(file_name);
    repaint();
}


void Widget::on_pushButtonConvexHull_clicked()
{
    //Get pols
    std::vector<QPolygon> polygons = ui->Canvas->getPolygons();

    //Create enclosing rectangle
    Algorithms a;
    QPolygon ch;
    std::vector<QPolygon> chs;
    std::vector<QPoint> points;

    for (unsigned int i = 0; i < polygons.size(); i++)
    {
        QPolygon pol = polygons[i];
        points.clear();
        ch.clear();
        for (int j = 0; j < pol.size(); j++)
        {
            points.push_back(pol[j]);
        }
        if (ui->comboBoxConvexHull->currentIndex()==0)
            ch = a.cHull(points);

        else if (ui->comboBoxConvexHull->currentIndex()==1)
            ch = a.qHull(points);

    chs.push_back(ch);
    }
    ui->Canvas->setCh(chs);
    repaint();
}

