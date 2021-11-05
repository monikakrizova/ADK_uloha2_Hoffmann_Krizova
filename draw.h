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
    double y_max = 0, x_min = 999999999; //pro transformaci
    double y_min = 999999999, x_max = 0; //pro meritko


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
