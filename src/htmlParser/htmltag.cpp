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

HtmlTag::HtmlTag ( bool type ) : level ( 0 ), closing ( type ), id(), attributes(), plainText()
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
  ret += ( closing? "</": "<" ) + id;

  if ( attributes.size() >0 )
    ret += ' ';

  for ( unsigned int i=0; i<attributes.size(); i++ )
  {
    ret += attributes[i].name + "=\"" + attributes[i].value + '"';
    if ( i+1<attributes.size() )
      ret += ' ';                 //put space between attributes
  }

  ret += '>';

  ret += plainText;
  return ret;
}


bool HtmlTag::hasAttr ( const std::string& attr ) const
{
  bool ret=false;
  for (unsigned int i=0; i<attributes.size(); i++)
    if ( attributes[i].name==attr )
    {
      ret=true;
      break;
    }
  return ret;
}


HtmlTag::Attr HtmlTag::getAttr ( const std::string& attr ) const
{
  Attr ret;
  for (unsigned int i=0; i<attributes.size(); i++)
    if ( attributes[i].name==attr )
    {
      ret=attributes[i];
      break;
    }
  return ret;
}


std::vector<HtmlTag::Attr> HtmlTag::getAttrs ( const std::string& attr ) const
{
  std::vector<Attr> ret;
  for (unsigned int i=0; i<attributes.size(); i++)
    if ( attributes[i].name==attr )
      ret.push_back ( attributes[i] );

  return ret;
}


bool HtmlTag::Attr::operator== ( const std::string& cmp ) const
{
  return name==cmp;
}


void HtmlTag::setLevel ( int l )
{
  level=l;
}


int HtmlTag::getLevel() const
{
  return level;
}


void HtmlTag::setType ( bool c )
{
  closing=c;
}


bool HtmlTag::isClosing() const
{
  return closing==true;
}


bool HtmlTag::isOpening() const
{
  return closing==false;
}


void HtmlTag::addAttr(const HtmlTag::Attr& attr)
{
  //drop quotas
  HtmlTag::Attr attribute=attr;        //make a writable copy
  Strings::stripQuotas(&attribute.value);
  attributes.push_back(attribute);
}


void HtmlTag::setId ( const std::string& i )
{
  id=i;
}


std::string HtmlTag::getId() const
{
  return id;
}


void HtmlTag::setText ( const std::string& s )
{
  plainText=s;
}


std::string HtmlTag::getText() const
{
  return plainText;
}
