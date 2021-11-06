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
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    //Create enclosing rectangle
    Algorithms a;
    std::vector<QPolygonF> ers, chs;
    std::vector<QPointF> points;

    if (ui->comboBox->currentIndex()==1) //Wall average
    {
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            QPolygonF pol = polygons[i];
            points.clear();
            for (int j = 0; j < pol.size(); j++)
            {
                points.push_back(pol[j]);
            }

            QPolygonF er = a.wallAverage(points);
            ers.push_back(er);

        }
        ui->Canvas->setEr(ers);
    }

    else if (ui->comboBox->currentIndex()==0) //Min area enclosing rectangle
    {
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            QPolygonF pol = polygons[i];
            points.clear();
            for (int j = 0; j < pol.size(); j++)
            {
                points.push_back(pol[j]);
            }

            QPolygonF er = a.minAreaEnclosingRectangle(points);
            ers.push_back(er);

        }
        ui->Canvas->setEr(ers);
    }
    else if (ui->comboBox->currentIndex()==2) //Longest edge
    {
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            QPolygonF pol = polygons[i];
            points.clear();
            for (int j = 0; j < pol.size(); j++)
            {
                points.push_back(pol[j]);
            }

            QPolygonF er = a.longestEdge(points);
            ers.push_back(er);

        }
        ui->Canvas->setEr(ers);
    }
    else if (ui->comboBox->currentIndex()==3) //Weighted Bisector
    {
        for (unsigned int i = 0; i < polygons.size(); i++)
        {
            QPolygonF pol = polygons[i];
            points.clear();
            for (int j = 0; j < pol.size(); j++)
            {
                points.push_back(pol[j]);
            }

            QPolygonF er = a.weightedBisector(points);
            ers.push_back(er);

        }
        ui->Canvas->setEr(ers);
    }

    //Repaint
    points.clear();
    repaint();
}

void Widget::on_pushButtonLoadData_clicked()
{
    //Open text file with dialog
    //QString file_name = "D:/Github/ADK/ADKI_uloha1/polygon_JTSK.txt";
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open Text file"), "", tr("Text Files (*.txt)"));
    ui->Canvas->loadData(file_name);
    repaint();
}


void Widget::on_pushButtonConvexHull_clicked()
{
    //Get pols
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    //Create enclosing rectangle
    Algorithms a;
    QPolygonF ch;
    std::vector<QPolygonF> chs;
    std::vector<QPointF> points;

    for (unsigned int i = 0; i < polygons.size(); i++)
    {
        QPolygonF pol = polygons[i];
        points.clear();
        ch.clear();
        for (int j = 0; j < pol.size(); j++)
        {
            points.push_back(pol[j]);
        }
        if (ui->comboBoxConvexHull->currentIndex()==0) //Convex hull
            ch = a.cHull(points);

        else if (ui->comboBoxConvexHull->currentIndex()==1) //Quick hull
            ch = a.qHull(points);

        chs.push_back(ch);
    }
    ui->Canvas->setCh(chs);
    repaint();
}

