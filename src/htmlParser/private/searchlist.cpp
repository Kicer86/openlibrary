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

namespace
{
    std::string *stripQuotas(std::string *s)
    {
        std::string &str=*s;         //str and s are now the same

        if (str.empty()==false &&
            str[0]==str[str.length()-1] &&
            (str[0]=='"' || str[0]=='\'')
            )
        {
            str.erase(0, 1);               //remove 1st " or '
            str.erase(str.length()-1, 1);  //remove last one
        }

        return &str;
    }

    std::string stripBlanks(const std::string &s)
    {
        std::string str=s;

        //from the begining
        int len=str.length();
        int l=0;

        while (l<len && str[l]<=32)
            l++;

        if (l)
            str.erase(0, l);

        //from the end
        len=str.length();
        int p=len-1;
        while (p>=0 && str[p]<=32)
            p--;

        p++;
        if (p<len)
            str.erase(p);  //remove them all from the end

        return str;
    }
}


SearchList::SearchList(): m_elements()
{}

void SearchList::setResults(const SearchList& list)
{
    m_elements.clear(); //remove current elements

    for (auto &item: list.getList())
        m_elements.push_back(item);                       //copy all values
}


void SearchList::init(const HtmlTagList* tagList, const std::string& id)
{
    //get search results
    SearchList ret;
    const HtmlTagList::HtmlTags &listOfTags = tagList->getHtmlTags();

    for (SearchListElement it = listOfTags.begin(); it < listOfTags.end(); it++ )
        if (it->getId() == id && it->isOpening())
            ret.addElement(it);

    setResults(ret);
}


void SearchList::findDescendant(const std::string& id)
{
    SearchList newResuls;                //new list of valid tags after filtration below

    for (SearchListElement element: m_elements)    //element is now copy of iterator on HtmlTagList (for accessing descentants etc)
    {
        int level = element->getLevel();
        element++;                                 //go to next tag
        while (true)
        {
            if (element->getLevel() == level + 1)  //this element is a child of main one ?
            {
                if (element->getId() == id)        //matches pattern?
                {
                    newResuls.addElement(element); //save it as result
                    break;                         //go to next
                }
            }
            else if (element->getLevel() <= level) //are we outside main element ?
            {
                //main element has no proper descentant - do not add it to new list
                break;
            }
        }
    }

    //override current valid tags list with found tags
    setResults(newResuls);
}


void SearchList::withAttr(const std::string& name)
{
    SearchList newResuls;                //new list of valid tags after filtration below

    for (SearchListElement element: m_elements)
    {
        if (element->hasAttr(name))      //does this tag have proper attribute?
            newResuls.addElement(element); //save it as result
    }

    //override current valid tags list with found tags
    setResults(newResuls);
}


void SearchList::withAttr(const std::string& name, const std::string& val)
{
    std::string value(val);   //make a writable copy
    Strings::stripQuotas(&value);

    SearchList newResuls;                //new list of valid tags after filtration below

    for (SearchListElement element: m_elements)
    {
        if (element->hasAttr(name))      //does this tag have proper attribute?
        {
            HtmlTag::Attr attr = element->getAttr(name);
            if (attr.value == value)
                newResuls.addElement(element); //save it as result
        }
    }

    //override current valid tags list with found tags
    setResults(newResuls);
}


void SearchList::addElement(const SearchListElement &element)
{
    m_elements.push_back(element);
}



const SearchList::SearchElements& SearchList::getList() const
{
    return m_elements;
}
