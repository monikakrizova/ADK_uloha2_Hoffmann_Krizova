#include "draw.h"

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    qp.begin(this);

    QPolygon pol;

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
