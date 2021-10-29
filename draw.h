#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint> points;
    std::vector <QPolygon> ers, chs;
    //QPolygon ch;
    std::vector <QPolygon> buildings;
    QPoint p;

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
