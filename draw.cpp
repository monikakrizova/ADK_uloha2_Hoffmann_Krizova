#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    p.setX(-100);
    p.setY(-100);
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    qp.begin(this);

    //Draw points
    //int r=4;
    QPolygon pol;
    //repaint();

    /*for (int i=0; i<points.size(); i++)
    {
        qp.drawEllipse(points[i].x()-r,points[i].y()-r,2*r,2*r);
        pol.append(points[i]);
    }*/

    for (unsigned int i = 0; i < buildings.size(); i++)
    {
        qp.drawPolygon(buildings[i]);
    }

    //Draw polygon
    //qp.setBrush(Qt::yellow);
    //qp.drawPolygon(pol);

    //Draw convex hull
    qp.setBrush(Qt::NoBrush);
    qp.setPen(Qt::red);
    //qp.drawPolygon(chs);
    for (int i = 0; i < chs.size(); i++)
    {
        qp.drawPolygon(chs[i]);
    }

    //Draw enclosing rectangle
    qp.setBrush(Qt::NoBrush);
    qp.setPen(Qt::green);
    for (int i = 0; i < ers.size(); i++)
    {
        qp.drawPolygon(ers[i]);
    }

    qp.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates
    int x = event->pos().x();
    int y = event->pos().y();

    //Create point
    p.setX(x);
    p.setY(y);

    //Add point to the vector
    points.push_back(p);

    //Update screen
    repaint();
}

void Draw::clear()
{
    //points.clear();
    buildings.clear();
    chs.clear();
    ers.clear();

    repaint();
}

void Draw::loadData(QString &file_name)
{
    //Load data from the *.txt file
    QPolygon polygon;
    QPoint vertice;

    QFile inputFile(file_name);
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                int id = line.split(" ")[0].toInt();
                int x = line.split(" ")[1].toDouble();
                int y = line.split(" ")[2].toDouble();

                if (id == 1)
                {
                    if (polygon.empty() == false)
                        buildings.push_back(polygon);
                    polygon.clear();
                    //Add vertice to the end of the QPoint vector
                    vertice.setX(x);
                    vertice.setY(y);
                    polygon.append(vertice);
                }
                else
                {
                    //Add vertice to the end of the QPoint vector
                    vertice.setX(x);
                    vertice.setY(y);
                    polygon.append(vertice);
                }
            }

            //Save polygon to the vector of QPolygons
            buildings.push_back(polygon);
        }

        inputFile.close();
    }
