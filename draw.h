#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>
#include "sortbyx.h"
#include "sortbyy.h"
#include <QPointF>
#include <QPolygonF>

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPointF> points;
    std::vector <QPolygonF> ers, chs;
    //QPolygonF ch;
    std::vector <QPolygonF> buildings;
    QPointF p;
    double y_max = 0, x_min = 999999999; //pro transformaci
    double y_min = 999999999, x_max = 0; //pro meritko


public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    std::vector<QPolygonF> getPolygons(){return buildings;}
    void setCh(std::vector <QPolygonF> &ch_){chs = ch_;}
    void setEr(std::vector <QPolygonF> &er_){ers = er_;}
    void loadData(QString &file_name);

signals:

public slots:
};

#endif // DRAW_H
