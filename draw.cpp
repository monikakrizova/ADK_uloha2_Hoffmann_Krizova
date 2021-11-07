#include "draw.h"
#include <iostream>

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    qp.begin(this);

    QPolygonF pol;

    //Draw polygons
    for (unsigned int i = 0; i < buildings.size(); i++)
    {
        qp.drawPolygon(buildings[i]);
    }

    //Draw convex hull
    qp.setBrush(Qt::NoBrush);
    qp.setPen(Qt::red);
    for (unsigned int i = 0; i < chs.size(); i++)
    {
        qp.drawPolygon(chs[i]);
    }

    //Draw enclosing rectangle
    qp.setBrush(Qt::NoBrush);
    qp.setPen(Qt::green);
    for (unsigned int i = 0; i < ers.size(); i++)
    {
        qp.drawPolygon(ers[i]);
    }
    qp.end();
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get cursor coordinates
    int x = event->pos().x();
    int y = event->pos().y();
}

void Draw::clear()
{
    //Clear canvas
    buildings.clear();
    chs.clear();
    ers.clear();

    repaint();
}

void Draw::loadData(QString &file_name)
{
    //Load data from the *.txt file
    QPolygonF polygon;
    std::vector<QPolygonF> buildings_;

    QPointF vertice;

    QFile inputFile(file_name);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            int id = line.split(" ")[0].toInt();
            double y = line.split(" ")[1].toDouble();
            double x = line.split(" ")[2].toDouble();

            if (id == 1)
            {
                if (polygon.empty() == false)
                    buildings_.push_back(polygon);
                polygon.clear();

                //Add vertice to the end of the QPointF vector
                vertice.setX(x);
                vertice.setY(y);
                polygon.append(vertice);
            }
            else
            {
                //Add vertice to the end of the QPointF vector
                vertice.setX(x);
                vertice.setY(y);
                polygon.append(vertice);
            }

            //Find max Y and min X to determine origin of local system
            QPointF pymax = *std::max_element(polygon.begin(), polygon.end(), sortByY());
            double yy = pymax.y();
            QPointF pxmin = *std::min_element(polygon.begin(), polygon.end(), sortByX());
            double xx = pxmin.x();

            if (yy > y_max)
                y_max = yy;
            if (xx < x_min)
                x_min = xx;

            //Additionaly find min Y and max X to determine the scale in both directions
            QPointF pymin = *std::min_element(polygon.begin(), polygon.end(), sortByY());
            double yy_ = pymin.y();
            QPointF pxmax = *std::max_element(polygon.begin(), polygon.end(), sortByX());
            double xx_ = pxmax.x();

            if (yy_ < y_min)
                y_min = yy_;
            if (xx_ > x_max)
                x_max = xx_;
        }

        //Save polygon to the vector of QPolygonFs
        buildings_.push_back(polygon);
    }

    //Compute scales to zoom in in canvas
    double canvas_weight = 952.0;
    double canvas_height = 748.0;

    double dy = fabs(y_max-y_min);
    double dx = fabs(x_max-x_min);

    double k;
    if (dy > dx)
        k = canvas_weight/dy;
    else
        k = canvas_height/dx;

    //Transform coordinates from JTSK to canvas
    for (int unsigned i = 0; i < buildings_.size(); i++)
    {
        QPolygonF pol = buildings_[i];
        for (int j = 0; j < pol.size(); j++)
        {
            double temp = pol[j].x();
            pol[j].setX(-k*(pol[j].y()-y_max));
            pol[j].setY(k*(temp-x_min));
        }

        buildings.push_back(pol);
    }
    inputFile.close();
}
