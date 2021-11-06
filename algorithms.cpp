#include "algorithms.h"
#include <cmath>
#include "sortbyy.h"
#include "sortbyx.h"
#include <iostream>

#include <QtGui>

Algorithms::Algorithms()
{

}

double Algorithms::get2LinesAngle(QPointF &p1, QPointF &p2, QPointF &p3, QPointF &p4)
{
    //Compute angle formed by two lines

    //Coordinate differences
    double ux=p2.x()-p1.x();
    double uy=p2.y()-p1.y();

    double vx=p4.x()-p3.x();
    double vy=p4.y()-p3.y();

    //Dot product
    double dp=ux*vx+uy*vy;

    //Norms
    double nu = sqrt(ux*ux + uy*uy);
    double nv = sqrt(vx*vx + vy*vy);

    //Angle
    return fabs(acos(dp/(nu*nv)));
}

int Algorithms::getPointLinePosition(QPointF &a,QPointF &p1,QPointF &p2)
{
    //Analyze point and line position
    double eps = 1.0e-10;

    //Coordinate differences
    double ux=p2.x()-p1.x();
    double uy=p2.y()-p1.y();

    double vx=a.x()-p1.x();
    double vy=a.y()-p1.y();

    //Half plane test(cross product)
    double t = ux*vy-vx*uy;

    //Point in the left halfplane
    if (t > eps)
        return 1;

    //Point in the right halfplane
    if (t < -eps)
        return 0;

    //Point on the line
    return -1;
}

double Algorithms::getPointLineDistance(QPointF &a, QPointF &p1, QPointF &p2)
{
    //Compute distance between line and point
    double up = fabs(a.x()*(p1.y() - p2.y()) + p1.x()*(p2.y() - a.y()) + p2.x()*(a.y() - p1.y()));

    double dx = p2.x()-p1.x();
    double dy = p2.y()-p1.y();

    double d = sqrt(dx*dx + dy*dy);

    return up/d;
}



QPolygonF Algorithms::cHull (std::vector <QPointF> &points)
{
     //Create convex hull, Jarvis scan
     QPolygonF ch;

     //Find pivot
     QPointF q=*std::min_element(points.begin(), points.end(), sortByY());

     //Add pivot to convex hull
     ch.append(q);

     //Create pj, pjj
     QPointF pj = q;
     QPointF pjj(0,q.y());

     do
     {
         //Find next convex hull point
         int i_max = -1;
         double om_max = 0;
         for (int i = 0; i<points.size(); i++)
         {
             double om = get2LinesAngle(pj, pjj, pj, points[i]);

             //Update maximum
             if (om > om_max)
             {
                 om_max = om;
                 i_max = i;
             }
         }

         //Add point to the convex hull
         ch.append(points[i_max]);

         //Update last two points of the convex hull
         pjj = pj;
         pj = points[i_max];

     } while (pj != q);

     return ch;
}

QPolygonF Algorithms::qHull (std::vector <QPointF> &points)
{
    //Create quick hull
    QPolygonF qh;
    std::vector<QPointF> su;
    std::vector<QPointF> sl;

    //Sort points
    std::sort(points.begin(), points.end(), sortByY());

    //Create pivots
    QPointF q1 = points[0];
    QPointF q3 = points.back();

    //Add pivots to SU and SL
    su.push_back(q1);
    su.push_back(q3);
    sl.push_back(q1);
    sl.push_back(q3);

    for (unsigned int i = 0; i < points.size(); i++)
    {
        int result = getPointLinePosition(points[i],q1,q3);
        if (result == 1)
            //Point in upper part
            su.push_back(points[i]);
        else if (result == 0)
            sl.push_back(points[i]);
    }

    qh.append(q3);
    qHullRecursive(1,0,su,qh);
    qh.append(q1);
    qHullRecursive(0,1,sl,qh);

    return qh;
}

void Algorithms::qHullRecursive(int r, int s, std::vector<QPointF> &points, QPolygonF &ch)
{
    double d_max = 0;
    int i_max = -99;

    for (int unsigned i = 2; i < points.size(); i++)
    {
        int result = getPointLinePosition(points[i],points[r],points[s]);

        if (result == 0)
        {
            double dist = getPointLineDistance(points[i], points[r], points[s]);

            if (dist > d_max)
            {
                d_max = dist;
                i_max = i;
            }
        }

    }
    if (i_max != -99)
    {
        qHullRecursive(r, i_max, points, ch);
        ch.push_back(points[i_max]);
        qHullRecursive(i_max, s, points, ch);
    }
}

std::vector <QPointF> Algorithms::rotate(std::vector <QPointF> &points, double sigma)
{
    //Rotate dataset by angle
    std::vector <QPointF> r_points;

    for (int unsigned i = 0; i < points.size(); i++)
    {
        //Rotate point
        double x_r = points[i].x()*cos(sigma) - points[i].y()*sin(sigma);
        double y_r = points[i].x()*sin(sigma) + points[i].y()*cos(sigma);

        // Create point
        QPointF rp(x_r, y_r);

        //Add point to the list
        r_points.push_back(rp);

    }

    return r_points;
}

std::tuple<std::vector<QPointF>, double> Algorithms::minMaxBox(std::vector <QPointF> &points)
{
    //Return vertices of min-max box and its area
    double area = 0;

    //Return vertices with extreme coordinates
    QPointF pxmin = *std::min_element(points.begin(), points.end(), sortByX());
    QPointF pxmax = *std::max_element(points.begin(), points.end(), sortByX());
    QPointF pymin = *std::min_element(points.begin(), points.end(), sortByY());
    QPointF pymax = *std::max_element(points.begin(), points.end(), sortByY());

    //Create min-max box vertices
    QPointF v1(pxmin.x(),pymin.y());
    QPointF v2(pxmax.x(),pymin.y());
    QPointF v3(pxmax.x(),pymax.y());
    QPointF v4(pxmin.x(),pymax.y());

    //Create min-max box polygon
    std::vector<QPointF> mmb{v1,v2,v3,v4};

    //Calculate min-max box area
    area = (pxmax.x() - pxmin.x())*(pymax.y() - pymin.y());

    return {mmb, area};
}

QPolygonF Algorithms::minAreaEnclosingRectangle(std::vector <QPointF> &points)
{
    //Create minimum area enclosing rectangle
    QPolygonF ch = cHull(points);
    std::vector<QPointF> er;
    er.clear();

    //Searching for min max box with min area
    int n = ch.size();
    double sigma_min=0;
    std::vector<QPointF> mmb_min;

    //Initializing area_min
    auto [mmb, area_min] = minMaxBox(points);

    for (int j = 0; j < n; j++)
    {
         //Coordinate differences
         double dx = ch[(j+1)%n].x() - ch[j].x();
         double dy = ch[(j+1)%n].y() - ch[j].y();

         double sigma = atan2(dy, dx);

         //Rotate by -sigma
         std::vector<QPointF> r_points = rotate(points, -sigma);

         //Create min-max box
         auto [mmb, area] = minMaxBox(r_points);

         //Update minimum
         if (area < area_min)
         {
             area_min = area;
             sigma_min = sigma;
             mmb_min = mmb;
         }
     }

    //Create enclosing rectangle
    er = rotate(mmb_min, sigma_min);

    std::cout << er.size() << std::endl;

    //Resize rectangle, preserve area of the building
    std::vector<QPointF> err = resizeRectangle(points,er);

    //Create QPolygonF
    QPolygonF er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);

    return er_pol;
}

QPolygonF Algorithms::wallAverage(std::vector <QPointF> &points)
{
    //Create enclosing rectangle using wall average method
    double sigma = 0, si_sum = 0;
    QPolygonF pol;

    //Compute initial direction
    double dx = points[1].x() - points[0].x();
    double dy = points[1].y() - points[0].y();
    double sigma_ = atan2(dy, dx);

    //Compute directions for segments
    int n = points.size();
    for (int i = 0; i < n; i++)
    {
        //Compute direction and length
        double dxi = points[(i+1)%n].x() - points[i].x();
        double dyi = points[(i+1)%n].y() - points[i].y();
        double sigmai = atan2(dyi, dxi);
        double lengthi = sqrt(dxi*dxi + dyi*dyi);

        //Compute direction differences
        double dsigmai = sigmai - sigma_;
        if (dsigmai < 0)
            dsigmai += 2*M_PI;

        //Compute fraction
        double ki=round(dsigmai/(M_PI/2));

        //Compute reminder
        double ri=dsigmai-ki*(M_PI/2);

        //Weighted average sums
        sigma += ri*lengthi;
        si_sum += lengthi;
    }

    //Weighted average
    sigma = sigma_ + sigma/si_sum;

    //Rotate by -sigma
    std::vector<QPointF> r_points = rotate(points, -sigma);

    //Create min-max box
    auto [mmb, area] = minMaxBox(r_points);

    //Create enclosing rectangle
    std::vector<QPointF> er = rotate(mmb, sigma);

    //Resize rectangle, preserve area of the building
    std::vector<QPointF> err = resizeRectangle(points,er);

    //Create QPolygonF
    QPolygonF er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);

    return er_pol;
}

QPolygonF Algorithms::longestEdge(std::vector <QPointF> &points)
{
    //Create enclosing rectangle using longest edge method
    double d_max = 0, sigma, dx, dy;
    QPolygonF pol;

    int n = points.size();
    for (int i = 0; i < n; i++)
    {
        //Compute coords differences and length
        double dxi = points[(i+1)%n].x() - points[i].x();
        double dyi = points[(i+1)%n].y() - points[i].y();
        double lengthi = sqrt(dxi*dxi + dyi*dyi);

        //Check if length [i] is bigger than max legth
        if (lengthi > d_max)
        {
            d_max = lengthi;
            dx = dxi;
            dy = dyi;
        }
    }

    //Compute direction
    sigma = atan2(dy,dx);

    //Rotate by -sigma
    std::vector<QPointF> r_points = rotate(points, -sigma);

    //Create min-max box
    auto [mmb, area] = minMaxBox(r_points);

    //Create enclosing rectangle
    std::vector<QPointF> er = rotate(mmb, sigma);

    //Resize rectangle, preserve area of the building
    std::vector<QPointF> err = resizeRectangle(points,er);

    //Create QPolygonF
    QPolygonF er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);

    return er_pol;
}

QPolygonF Algorithms::weightedBisector(std::vector <QPointF> &points)
{
    //Create enclosing rectangle using weighted bisector method
    double u1_max = 0, u2_max = 0, dx1 = 0, dx2, dy1 = 0, dy2, sigma1, sigma2, sigma;
    QPolygonF pol;

    int n = points.size(); //Number of vertices in polygon

    //Compute two longest diagonals in polygon
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n-3; j++)
        {
            double dxi = points[(i+j+2)%n].x() - points[i].x();
            double dyi = points[(i+j+2)%n].y() - points[i].y();
            double lengthi = sqrt(dxi*dxi + dyi*dyi);

            //Check if length [i] is bigger than max legth of diagonal
            if (lengthi >= u1_max)
            {
                //Update two longest diagonals
                u2_max = u1_max;
                u1_max = lengthi;
                dx2 = dx1;
                dy2 = dy1;
                dx1 = dxi;
                dy1 = dyi;
            }
        }
    }

    //Compute direction by weighted avarage
    sigma1 = atan2(dy1,dx1);
    sigma2 = atan2(dy2,dx2);
    sigma = (sigma1*u1_max + sigma2*u2_max)/(u1_max+u2_max);

    //Rotate by -sigma
    std::vector<QPointF> r_points = rotate(points, -sigma);

    //Create min-max box
    auto [mmb, area] = minMaxBox(r_points);

    //Create enclosing rectangle
    std::vector<QPointF> er = rotate(mmb, sigma);

    //Resize rectangle, preserve area of the building
    std::vector<QPointF> err = resizeRectangle(points,er);

    //Create QPolygonF
    QPolygonF er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);

    return er_pol;
}

double Algorithms::LH(std::vector <QPointF> &points)
{
    //Get area of building by L' Huillier formula
    int n = points.size();
    double area = 0;

    //Proces all vertices of the building
    for (int i = 0; i < n; i++)
    {
        area += points[i].x() * (points[(i+1)%n].y() - points[(i-1+n)%n].y());
    }

    //Return unsigned area
    return 0.5 * fabs(area);
}

std::vector <QPointF> Algorithms::resizeRectangle(std::vector <QPointF> &points, std::vector <QPointF> &er)
{
    //Resize rectangle to given area

    //Building area
    double AB = LH(points);

    //Rectangle area
    double AR = LH(er);

    //Fraction of areas
    double k = AB/AR;

    //Center of mass
    double xc = (er[0].x() + er[1].x() + er[2].x() + er[3].x())/4;
    double yc = (er[0].y() + er[1].y() + er[2].y() + er[3].y())/4;

    //Compute vector components
    double u1x = er[0].x() - xc;
    double u1y = er[0].y() - yc;
    double u2x = er[1].x() - xc;
    double u2y = er[1].y() - yc;
    double u3x = er[2].x() - xc;
    double u3y = er[2].y() - yc;
    double u4x = er[3].x() - xc;
    double u4y = er[3].y() - yc;

    //Create new rectangle vertices
    QPointF v1_(xc + sqrt(k) * u1x, yc + sqrt(k) * u1y);
    QPointF v2_(xc + sqrt(k) * u2x, yc + sqrt(k) * u2y);
    QPointF v3_(xc + sqrt(k) * u3x, yc + sqrt(k) * u3y);
    QPointF v4_(xc + sqrt(k) * u4x, yc + sqrt(k) * u4y);

    //Add vertices to vector
    std::vector<QPointF> er_res = {v1_, v2_, v3_, v4_};

    return er_res;
}
