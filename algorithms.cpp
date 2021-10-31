#include "algorithms.h"
#include <cmath>
#include "sortbyy.h"
#include "sortbyx.h"
#include <iostream>

#include <QtGui>

Algorithms::Algorithms()
{

}

double Algorithms::get2LinesAngle(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4)
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


QPolygon Algorithms::cHull (std::vector <QPoint> &points)
{
    //Create convex hull, Jarvis scan
    QPolygon ch;

    //Find pivot
    QPoint q=*std::min_element(points.begin(), points.end(), sortByY());

    //Add pivot to convex hull
    ch.append(q);

    //Create pj, pjj
    QPoint pj = q;
    QPoint pjj(0,q.y());

    do
    {
        //Find next convex hull point
        int i_max = -1;
        double om_max = 0;
        for (unsigned int i = 0; i<points.size(); i++)
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


std::vector <QPoint> Algorithms::rotate(std::vector <QPoint> &points, double sigma)
{
    //Rotate dataset by angle
    std::vector <QPoint> r_points;
    for (unsigned int i = 0; i < points.size(); i++)
    {
        //Rotate point
        double x_r = points[i].x()*cos(sigma) - points[i].y()*sin(sigma);
        double y_r = points[i].x()*sin(sigma) + points[i].y()*cos(sigma);

        // Create point
        QPoint rp(x_r, y_r);

        //Add point to the list
        r_points.push_back(rp);
    }
    return r_points;
}


std::tuple<std::vector<QPoint>, double> Algorithms::minMaxBox(std::vector <QPoint> &points)
{
    //Return vertices of min-max box and its area
    double area = 0;

    //Return vertices with extreme coordinates
    QPoint pxmin = *std::min_element(points.begin(), points.end(), sortByX());
    QPoint pxmax = *std::max_element(points.begin(), points.end(), sortByX());
    QPoint pymin = *std::min_element(points.begin(), points.end(), sortByY());
    QPoint pymax = *std::max_element(points.begin(), points.end(), sortByY());

    //Create min-max box vertices
    QPoint v1(pxmin.x(),pymin.y());
    QPoint v2(pxmax.x(),pymin.y());
    QPoint v3(pxmax.x(),pymax.y());
    QPoint v4(pxmin.x(),pymax.y());

    //Create min-max box polygon
    std::vector<QPoint> mmb{v1,v2,v3,v4};

    //Calculate min-max box area
    area = (pxmax.x() - pxmin.x())*(pymax.y() - pymin.y());

    return {mmb, area};
}


QPolygon Algorithms::minAreaEnclosingRectangle(std::vector <QPoint> &points)
{
    //Create minimum area enclosing rectangle
    QPolygon ch = cHull(points);

    //Searching for min max box with min area
    int n = ch.size();
    double sigma_min=0;
    std::vector<QPoint> mmb_min;

    //Initializing area_min
    auto [mmb, area_min] = minMaxBox(points);
            std::cout<< "minmaxbox inicializovan " << std::endl;

    for (int i=0; i<n; i++)
    {
         //Coordinate differences
         double dx = ch[(i+1)%n].x() - ch[i].x();
         double dy = ch[(i+1)%n].y() - ch[i].y();

         double sigma = atan2(dy, dx);

         //Rotate by -sigma
         std::vector<QPoint> r_points = rotate(points, (-sigma));

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
    std::cout<< "mmb min " << area_min << ", sigma min: " << sigma_min << std::endl;


    //Create enclosing rectangle
    std::vector<QPoint> eR = rotate(mmb_min, sigma_min);

    std::cout<< "narotovano " << std::endl;


    //Resize rectangle, preserve area of the building
    std::vector<QPoint> err = resizeRectangle(points,eR);

    std::cout<< "resiznuto " << std::endl;


    //Create QPolygon
    QPolygon er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);
    std::cout<< "vse ok " << std::endl;

    return er_pol;
}


QPolygon Algorithms::wallAverage(std::vector <QPoint> &points)
{
    //Create enclosing rectangle using wall average method
    double sigma = 0, si_sum = 0;
    QPolygon pol;

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
    std::vector<QPoint> r_points = rotate(points, -sigma);

    //Create min-max box
    auto [mmb, area] = minMaxBox(r_points);

    //Create enclosing rectangle
    std::vector<QPoint> er = rotate(mmb, sigma);

    //Resize rectangle, preserve area of the building
    std::vector<QPoint> err = resizeRectangle(points,er);

    //Create QPolygon
    QPolygon er_pol;
    er_pol.append(err[0]);
    er_pol.append(err[1]);
    er_pol.append(err[2]);
    er_pol.append(err[3]);

    return er_pol;
}


double Algorithms::LH(std::vector <QPoint> &points)
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


std::vector <QPoint> Algorithms::resizeRectangle(std::vector <QPoint> &points, std::vector <QPoint> &er)
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
        QPoint v1_(xc + sqrt(k) * u1x, yc + sqrt(k) * u1y);
        QPoint v2_(xc + sqrt(k) * u2x, yc + sqrt(k) * u2y);
        QPoint v3_(xc + sqrt(k) * u3x, yc + sqrt(k) * u3y);
        QPoint v4_(xc + sqrt(k) * u4x, yc + sqrt(k) * u4y);

        //Add vertices to vector
        std::vector<QPoint> er_res = {v1_, v2_, v3_, v4_};

        return er_res;
}
