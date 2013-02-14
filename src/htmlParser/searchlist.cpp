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

#include "../debug.hpp"
#include "../str_op.hpp"
#include "htmltaglist.hpp"
#include "searchlist.hpp"

SearchList::SearchList(): m_elements()
{}

void SearchList::setResults(const SearchList& list)
{
    m_elements.clear(); //remove current elements

    debug(DebugLevel::Debug) << "after filtration:";

    for (auto &item: list.getList())
    {
        m_elements.push_back(item);                       //copy all values

        debug(DebugLevel::Debug) << *( item );
    }
}


void SearchList::init(const HtmlTagList* tagList, const std::string& id)
{
    //get search results
    debug(DebugLevel::Debug) << "creating SearchList with \"" << id << "\" as primary filter";

    SearchList ret;
    const HtmlTagList::HtmlTags &listOfTags = tagList->getHtmlTags();

    for (SearchListElement it = listOfTags.begin(); it < listOfTags.end(); it++ )
        if (it->getId() == id && it->isOpening())
            ret.addElement(it);

    setResults(ret);
}


void SearchList::findDescendant(const std::string& id)
{
    debug(DebugLevel::Debug) << "adding filter 'descentant': \"" << id << '"';
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
    debug(DebugLevel::Debug) << "adding filter 'attribute': \"" << name << '"';

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

    debug(DebugLevel::Debug) << "adding filter 'attribute': \"" << name << "\" equal to \"" << value << '"';
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
