#ifndef SORTBYX_H
#define SORTBYX_H
#include <QtGui>

class sortByX
{
public:
    sortByX();
    bool operator () (QPoint &p1, QPoint &p2)
    {
        return p1.x() < p2.x() || (p1.x() == p2.x()) && (p1.y() < p2.y());
    }
};

#endif // SORTBYX_H
