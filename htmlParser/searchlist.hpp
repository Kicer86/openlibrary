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


#ifndef SEARCHLIST_HPP
#define SEARCHLIST_HPP

#include <vector>

#include "htmltag.hpp"

class HtmlTagList;

typedef std::vector<HtmlTag>::const_iterator SearchListElement;

class SearchList: public std::vector<SearchListElement>
{
    void setResults(const SearchList& list);

  public:
    SearchList();
    void init(const HtmlTagList* tagList, const std::string& id); //init list with tagList. keep only tags that have given id   (css selector: E)
    void findDescendant(const std::string& id);                   //find descendants of tags being keep, which have proper name (css selector: E F - E is current tag list, F - descentant name)
    void withAttr(const std::string& name);                       //leave only tags with given attr name (css selector E[foo])
    void withAttr(const std::string& name, const std::string& val); //leave only tag with given attr and it's value (css selector E[foo="warning"]
};

#endif // SEARCHLIST_HPP
