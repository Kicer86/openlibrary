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

#include <algorithm>
#include <vector>

#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "router.hpp"

Router::Router(Router::Area area)
{ 
  width = area.p2.x - area.p1.x;
  height = area.p2.y - area.p1.y;
  
  basePoint = area.p1;
  
  field=new char * [height];
  for (int i = 0; i < width; i++)
    field[i] = new char[width];
  
  for (int y = 0; y<height; y++)
    for (int x = 0; x<width; x++)
      field[y][x] = -1;
}


Router::~Router()
{
  for (int i = 0; i < width; i++)
    delete [] field[i];
  
  delete [] field;
}


void Router::addWorker(const Router::Worker& w)
{
  if (w.x>=basePoint.x && w.x < basePoint.x + width &&
      w.y>=basePoint.y && w.y < basePoint.y + height)   //in area?
  {
    //check if this filed is free
    if (field[w.y][w.x]==-1)  //empty
    {
      field[w.y][w.x]=w.value;   //fill this field of field ;)
      
      Worker c=w;
      c.value++;
      c.value&=7;
      
      workers.push_front(c);      //save worker
    }
  }
}


int Router::sqr(int a)
{
  return a*a;
}


double Router::distance(const Router::Point &p1, const Router::Point &p2)
{
  return sqrt(sqr(p1.x-p2.x) + sqr(p1.y-p2.y));
}


void Router::addObstacle(const Router::Area& obst)
{
   for (int y = obst.p1.y; y <= obst.p2.y; y++)
    for (int x = obst.p1.x; x <= obst.p2.x; x++)
      field[y][x] = 15;
}


bool Router::route(const Router::Point &begin, const Router::Point &end)
{
  Worker first(begin);
  addWorker(first);
  
  while (workers.size()>0)
  {
    //get first worker;
    Worker worker=workers.front();  //get first
    workers.pop_front();            //and remove it
    
    //add new workers  
    Worker top=worker, bottom=worker, left=worker, right=worker;
    
    top.y++;
    bottom.y--;
    right.x++;
    left.x--;
    
    if (top==end || bottom==end || left==end || right==end)
      return true;
    
    //calculate distance each of new points to end
    double top_d=distance(top, end);
    double bottom_d=distance(bottom, end);
    double right_d=distance(right, end);
    double left_d=distance(left, end);
    
    typedef std::pair<double, Worker*> Distance;
    std::vector<Distance> distances;
    distances.push_back(Distance(top_d, &top));
    distances.push_back(Distance(bottom_d, &bottom));
    distances.push_back(Distance(left_d, &left));
    distances.push_back(Distance(right_d, &right));
    
    std::sort(distances.begin(), distances.end());
    
    addWorker(*distances[3].second);
    addWorker(*distances[2].second);
    addWorker(*distances[1].second);
    addWorker(*distances[0].second);       //closest one will be first
    
    usleep(300e3);   //0.1 sec
  }
  
  return false;
}
