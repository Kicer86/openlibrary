/*
    Universal debug routines
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
#include <vector>

#ifdef __GNUC__
	#define FUNCTION_NAME __PRETTY_FUNCTION__
#else
	#define FUNCTION_NAME __FUNCTION__
#endif

#define ol_debug(l) Debug(FUNCTION_NAME, l)

//control macros:
//DEBUG_QSTRING_SUPPORT - support for QString
//DEBUG_VERBOSE         - more verbose output
//DEBUG_DISABLE_OUTPUT  - disable output

#ifdef DEBUG_QSTRING_SUPPORT
#include <QString>
#endif


namespace ol
{

    enum class Level
    {
        Debug,
        Info,
        Warning,
        Error
    };


    class Debug
    {
            std::stringstream data;
            Level level;

            bool enableOutput() const
            {
                //debugging off?
    #ifdef DEBUG_DISABLE_OUTPUT
                return false;
    #else
        #ifdef NDEBUG
                if (level == Level::Debug)      //no output if NDEBUG was defined and output level==DEBUG
                    return false;
        #endif
                return true;
    #endif
            }

        public:
            Debug(const char *f_name, DebugLevel::Level l = Level::Info): data(), level(l)
            {
    #ifdef DEBUG_VERBOSE
                if (enableOutput())
                    data << f_name << ": ";
    #else
                if (enableOutput())
                    (void) f_name;
    #endif
            }

            ~Debug()
            {
                if (enableOutput())
                    std::clog << data.str() << std::endl;
            }

            template <typename T> Debug& operator<<(const T &arg)
            {
                if (enableOutput())
                    data << arg;

                return *this;
            }

            template <typename T> Debug& operator<<(const std::vector<T *> &v)
            {
                if (enableOutput())
                    for(T* item: v)
                        data << *item << "; ";

                return *this;
            }

            template <typename T> Debug& operator<<(const std::vector<T> &v)
            {
                if (enableOutput())
                    for(T& item: v)
                        data << item << "; ";

                return *this;
            }

    #ifdef DEBUG_QSTRING_SUPPORT
            Debug& operator<<(const QString &arg)
            {
                return (*this) << arg.toLocal8Bit().data();
            }
    #endif
    };

}

#endif // DEBUG_HPP
