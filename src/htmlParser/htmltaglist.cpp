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


#include "htmltaglist.hpp"
#include "searchlist.hpp"

HtmlTagList::HtmlTagList(): m_htmlTags()
{

}


void HtmlTagList::addElement(const HtmlTag &x)
{
    HtmlTag newTag = x; //make a copy
    if (m_htmlTags.empty() > 0)     //there are already some elements?
    {
        const HtmlTag &last = m_htmlTags.back(); //last element
        if (last.isClosing())   //last one is closing one? (</tag>)
        {
            if (newTag.isClosing()) //new one is also closing one? - decrease it's depth-level
                newTag.setLevel(last.getLevel() - 1);
            else                    //else - use the same one
                newTag.setLevel(last.getLevel());
        }
        else //opening
        {
            if (newTag.isOpening()) //new one is also opening? - increase its depth-level
                newTag.setLevel(last.getLevel() + 1);
            else                    //else - use the same one
                newTag.setLevel(last.getLevel());
        }
    }
    m_htmlTags.push_back(newTag);
}


const HtmlTagList::HtmlTags& HtmlTagList::getHtmlTags() const
{
    return m_htmlTags;
}


HtmlTagList::HtmlTags& HtmlTagList::getHtmlTags()
{
    return m_htmlTags;
}


SearchList HtmlTagList::findAll(const std::string &id) const
{
    SearchList ret;

    for (SearchListElement it = m_htmlTags.begin(); it < m_htmlTags.end(); it++ )
        if (it->getId() == id && it->isOpening())
            ret.addElement(it);

    return ret;
}

