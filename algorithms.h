#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <vector>

class Algorithms
{
public:
    Algorithms();
    double get2LinesAngle(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4);
    int getPointLinePosition(QPoint &a,QPoint &p1,QPoint &p2);
    double getPointLineDistance(QPoint &a, QPoint &p1, QPoint &p2);
    QPolygon cHull (std::vector <QPoint> &points);
    QPolygon qHull (std::vector <QPoint> &points);
    void qHullRecursive(int r, int s, std::vector<QPoint> &points, QPolygon &ch);
    std::vector <QPoint> rotate(std::vector <QPoint> &points, double sigma);
    std::tuple<std::vector<QPoint>, double> minMaxBox(std::vector <QPoint> &points);
    QPolygon minAreaEnclosingRectangle(std::vector <QPoint> &points);
    QPolygon wallAverage(std::vector <QPoint> &points);
    double LH(std::vector <QPoint> &points);
    std::vector <QPoint> resizeRectangle(std::vector <QPoint> &points, std::vector <QPoint> &er);
    std::vector<QPolygon> wallAverage(std::vector <QPolygon> &buildings);

};

#endif // ALGORITHMS_H
