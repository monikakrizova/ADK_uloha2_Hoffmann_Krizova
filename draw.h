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
    QPolygon ch, er;

public:
    explicit Draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void clear();
    std::vector<QPoint> getPoints(){return points;}
    void setCh(QPolygon &ch_){ch = ch_;}
    void setEr(QPolygon &er_){er = er_;}


signals:

public slots:
};

#endif // DRAW_H
