/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Michał Walenciak <Kicer86@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <list>

class Router
{
  public:
    struct Point
    {
      int x;
      int y;
      Point(): x(0),y(0) {}
      Point(int _x, int _y): x(_x), y(_y) {}
      
      bool operator==(const Point &p)
      {
        return x==p.x && y==p.y;
      }
      
      Point &operator=(const Point &p)
      {
        x=p.x;
        y=p.y;
        
        return *this;
      }
    };
    
    struct Area
    {
      Point p1;
      Point p2;
      
      Area(const Point &_p1, const Point &_p2): p1(_p1), p2(_p2) 
      {
        //keep p2>p1
        
        if (p1.x > p2.x)
          std::swap(p1.x, p2.x);

        if (p1.y > p2.y)
          std::swap(p1.y, p2.y); 
      }
    };
    
    Router(Area);
    virtual ~Router();
    
    void addObstacle(const Area &a);
    bool route(const Router::Point& begin, const Router::Point& end);
    char **getField() const
    {
      return field;
    }
      
  private:
    struct Worker: public Point
    {
      char value;    //between 0 and 7
      
      bool operator==(const Point &p)
      {
        return Point::operator==(p);
      }
      
      bool operator!=(const Point &p)
      {
        return ! Point::operator==(p);
      }
      
      Worker &operator=(const Point &p)
      {
        Point::operator=(p);
        return *this;
      }
      
      Worker(): Point(), value(0) {}
      explicit Worker(const Point &p):Point(p), value(0) {}
    };
    
    Router(const Router&);
    Router& operator=(const Router&);
    
    void addWorker(const Worker &w);
    int sqr(int);
    double distance(const Point &, const Point &);
    
    std::list<Worker> workers;
    char **field;        //two dimensional table with values -1-15 (0-7 from worker, 15 - ocuppied area, -1 - empty)
    Point basePoint;
    int width, height;   
};

#endif // ROUTER_HPP
