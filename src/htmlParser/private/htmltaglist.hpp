/*
    List of tags which handles theirs level
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


#ifndef HTMLTAGLIST_HPP
#define HTMLTAGLIST_HPP

#include <vector>

#include "htmltag.hpp"
#include "searchlist.hpp"

class HtmlTagList
{
    public:
        typedef std::vector<HtmlTag> HtmlTags;
        HtmlTagList();
        void addElement(const HtmlTag& x);
        const HtmlTags& getHtmlTags() const;

    private:
        HtmlTags m_htmlTags;
};

#endif // HTMLTAGLIST_HPP
