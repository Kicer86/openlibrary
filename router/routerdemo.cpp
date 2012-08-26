/*
    Router visualisation
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

#include <QPaintEvent>
#include <QPainter>

#include "routerdemo.hpp"
#include "router.hpp"

#define WIDTH 30
#define HEIGHT 10
#define SIZE 20

void RoutingThread::run()
{
  router=new Router(Router::Area(Router::Point(0,0), Router::Point(WIDTH,HEIGHT)));
  router->addObstacle(Router::Area(Router::Point(10,0), Router::Point(12,8)));
  router->addObstacle(Router::Area(Router::Point(20,9), Router::Point(22,2)));
  router->route(Router::Point(0,0), Router::Point(25,9));
  
  exit();
}


RoutingThread::~RoutingThread()
{
  delete router;
}


char** RoutingThread::getField()
{
  return router->getField();
}


RouterDemo::RouterDemo(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
  thread.start();
  
  connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
  timer.start(200);    //repaint frequency
}


void RouterDemo::paintEvent(QPaintEvent* e)
{
  char **field=thread.getField();
  
  QPainter painter(this);
  
  for (int y=0; y< HEIGHT; y++)
    for (int x=0; x< WIDTH; x++)
    {
      switch (field[y][x])
      {
        case 0:
          painter.setBrush(QColor(Qt::red));
          break;
        
        case 1:
          painter.setBrush(QColor(Qt::darkRed));
          break;
          
        case 2:
          painter.setBrush(QColor(Qt::blue));
          break; 
          
        case 3:
          painter.setBrush(QColor(Qt::darkBlue));
          break; 
        
        case 4:
          painter.setBrush(QColor(Qt::green));
          break;
        
        case 5:
          painter.setBrush(QColor(Qt::darkGreen));
          break;
          
        case 6:
          painter.setBrush(QColor(Qt::cyan));
          break; 
          
        case 7:
          painter.setBrush(QColor(Qt::darkCyan));
          break;   
          
        case 15:
          painter.setBrush(QColor(Qt::black));
          break; 
          
        default:
          painter.setBrush(QColor(Qt::white));
          break; 
      }
      
      QRect rect(x*SIZE, y*SIZE, SIZE, SIZE);
      painter.drawRect(rect);
    }
    
  e->accept();
}
