#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <vector>
#include <QPointF>
#include <QPolygonF>

class Algorithms
{
public:
    Algorithms();
    double get2LinesAngle(QPointF &p1, QPointF &p2, QPointF &p3, QPointF &p4);
    int getPointLinePosition(QPointF &a,QPointF &p1,QPointF &p2);
    double getPointLineDistance(QPointF &a, QPointF &p1, QPointF &p2);
    QPolygonF cHull (std::vector <QPointF> &points);
    QPolygonF qHull (std::vector <QPointF> &points);
    void qHullRecursive(int r, int s, std::vector<QPointF> &points, QPolygonF &ch);
    std::vector <QPointF> rotate(std::vector <QPointF> &points, double sigma);
    std::tuple<std::vector<QPointF>, double> minMaxBox(std::vector <QPointF> &points);
    QPolygonF minAreaEnclosingRectangle(std::vector <QPointF> &points);
    QPolygonF wallAverage(std::vector <QPointF> &points);
    QPolygonF longestEdge(std::vector <QPointF> &points);
    QPolygonF weightedBisector(std::vector <QPointF> &points);
    double LH(std::vector <QPointF> &points);
    std::vector <QPointF> resizeRectangle(std::vector <QPointF> &points, std::vector <QPointF> &er);

};

#endif // ALGORITHMS_H
