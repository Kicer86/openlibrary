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

#include <iostream>
#include <sstream>

#define debug(l) Debug(__PRETTY_FUNCTION__, l)

#ifdef DEBUG_QSTRING_SUPPORT
#include <QString>
#endif

namespace DebugLevel
{
    enum Level
    {
        Debug,
        Info,
        Warning,
        Error
    };
}

class Debug
{
        std::stringstream data;
        DebugLevel::Level level;

        bool enableOutput() const
        {
            //debugging off?
#ifdef NDEBUG
            if (level == DebugLevel::Debug)      //no output if NDEBUG was defined and output level==DEBUG
                return false;
#endif
            return true;
        }

    public:
        Debug(const char *f_name, DebugLevel::Level l = DebugLevel::Info): data(), level(l)
        {
            if (enableOutput())
                data << f_name << ": ";
        }

        ~Debug()
        {
            if (enableOutput())
                std::clog << data.str() << std::endl;
        }

        template <typename T> Debug &operator<<(const T &arg)
        {
            if (enableOutput())
                data << arg;

            return *this;
        }

#ifdef DEBUG_QSTRING_SUPPORT
        Debug &operator<<(const QString &arg)
        {
            return (*this) << arg.toLocal8Bit().data();
        }
#endif
};



#endif // DEBUG_HPP
