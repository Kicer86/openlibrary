/*
    Class - container for html tags list
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

#include "../str_op.hpp"
#include "htmltag.hpp"

struct HtmlTag::Data
{
	int level;         //depth level
    bool closing;      //false: <tag>, true: </tag>
    std::string id;
    std::vector<HtmlTag::Attr> attributes;
    std::string plainText;

	Data(bool type = false): level ( 0 ), closing ( type ), id(), attributes(), plainText() {}
};

HtmlTag::HtmlTag ( bool type ) : m_data(new HtmlTag::Data(type))
{}


HtmlTag::~HtmlTag()
{}


HtmlTag::operator std::string() const
{
  return toString();
}


std::string HtmlTag::toString() const
{
  std::string ret;
  for ( int i=0; i<getLevel(); i++ )
    ret += ' ';
  ret += ( m_data->closing? "</": "<" ) + m_data->id;

  if ( m_data->attributes.size() >0 )
    ret += ' ';

  for ( unsigned int i=0; i < m_data->attributes.size(); i++ )
  {
    ret += m_data->attributes[i].name + "=\"" + m_data->attributes[i].value + '"';
    if ( i+1 < m_data->attributes.size() )
      ret += ' ';                 //put space between attributes
  }

  ret += '>';

  ret += m_data->plainText;
  return ret;
}


bool HtmlTag::hasAttr ( const std::string& attr ) const
{
  bool ret = false;
  for (unsigned int i=0; i < m_data->attributes.size(); i++)
    if ( m_data->attributes[i].name==attr )
    {
      ret = true;
      break;
    }
  return ret;
}


HtmlTag::Attr HtmlTag::getAttr ( const std::string& attr ) const
{
  Attr ret;
  for (size_t i=0; i < m_data->attributes.size(); i++)
    if ( m_data->attributes[i].name==attr )
    {
      ret = m_data->attributes[i];
      break;
    }
  return ret;
}


std::vector<HtmlTag::Attr> HtmlTag::getAttrs ( const std::string& attr ) const
{
  std::vector<Attr> ret;
  for (unsigned int i=0; i < m_data->attributes.size(); i++)
    if ( m_data->attributes[i].name==attr )
      ret.push_back ( m_data->attributes[i] );

  return ret;
}


bool HtmlTag::Attr::operator== ( const std::string& cmp ) const
{
  return name == cmp;
}


void HtmlTag::setLevel ( int l )
{
  m_data->level = l;
}


int HtmlTag::getLevel() const
{
  return m_data->level;
}


void HtmlTag::setType ( bool c )
{
  m_data->closing = c;
}


bool HtmlTag::isClosing() const
{
  return m_data->closing == true;
}


bool HtmlTag::isOpening() const
{
  return m_data->closing == false;
}


void HtmlTag::addAttr(const HtmlTag::Attr& attr)
{
  //drop quotas
  HtmlTag::Attr attribute = attr;        //make a writable copy
  Strings::stripQuotas(&attribute.value);
  m_data->attributes.push_back(attribute);
}


void HtmlTag::setId ( const std::string& i )
{
  m_data->id=i;
}


std::string HtmlTag::getId() const
{
  return m_data->id;
}


void HtmlTag::setText ( const std::string& s )
{
  m_data->plainText = s;
}


std::string HtmlTag::getText() const
{
  return m_data->plainText;
}
