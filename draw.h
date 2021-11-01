#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>
#include "sortbyx.h"
#include "sortbyy.h"

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint> points;
    std::vector <QPolygon> ers, chs;
    //QPolygon ch;
    std::vector <QPolygon> buildings;
    QPoint p;
    //long long y_min = 999999999999, x_min = -999999999999;
    //long long y_min = 999999999999, x_max = -999999999999;


public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    std::vector<QPolygon> getPolygons(){return buildings;}
    void setCh(std::vector <QPolygon> &ch_){chs = ch_;}
    void setEr(std::vector <QPolygon> &er_){ers = er_;}
    void loadData(QString &file_name);

signals:

public slots:
};

#endif // DRAW_H
