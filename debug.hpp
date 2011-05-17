/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Michał Walenciak <Kicer86@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef DEBUG_HPP
#define DEBUG_HPP


#ifdef IN_KDEVELOP_PARSER
//let kdevelop see class Debug :)
#define DEBUG
#endif


//only if debugging
#ifdef DEBUG

#include <iostream>
#include <string>
#define debug() Debug(__PRETTY_FUNCTION__)

class Debug
{
    std::string data;
  public:
    Debug(const char* f_name): data(f_name)
    {
      data+=" ";
    }

    ~Debug()
    {
      std::clog << data << std::endl;;
    }
    
    template <class T> Debug &operator<<(const T &arg)
    {
      data+=arg;
      return *this;
    }
};

#else
//no debugging:
#define debug() Debug()
class Debug
{
  public:
    template<class T> Debug &operator<<(const T &arg) 
    {
      return *this;
    };
};
#endif

#endif // DEBUG_HPP
