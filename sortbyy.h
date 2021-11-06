#ifndef SORTBYY_H
#define SORTBYY_H
#include <QtGui>
#include <QPointF>

class sortByY
{
public:
    sortByY();
    bool operator () (QPointF &p1, QPointF &p2)
    {
        return (p1.y() < p2.y()) || ((p1.y() == p2.y()) && (p1.x() < p2.x()));
    }
};

#endif // SORTBYY_H
