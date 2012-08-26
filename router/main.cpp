
#include <QApplication>

#include "routerdemo.hpp"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  
  RouterDemo *routerDemo=new RouterDemo;
  
  routerDemo->show();
  
  return app.exec();
}
