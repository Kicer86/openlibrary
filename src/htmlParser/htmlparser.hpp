/*
    Html parser
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


#ifndef HTMLPARSER_HPP
#define HTMLPARSER_HPP

#include <vector>
#include <string>

#include "htmlParser_export.h"

class HtmlTagList;
class HtmlTag;

/**
 * @brief Main HtmlParser class
 *
 **/
class HTMLPARSER_EXPORT HtmlParser
{
        HtmlTagList *htmlCode;

    public:
        HtmlParser(const std::string &html);
        HtmlParser(const HtmlParser &) = delete;
        virtual ~HtmlParser();

        void parse(const std::string &html);
        std::vector<HtmlTag*> findAll(const std::string &query);  //use http://www.w3.org/TR/CSS2/selector.html

        HtmlParser& operator=(const HtmlParser &) = delete;
};

#endif // HTMLPARSER_HPP
