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

#include <iostream>
#include <string>
#include <vector>

#include "htmlparser.hpp"
#include "htmltaglist.hpp"
#include "private/cssspdata.hpp"

#define YY_NO_UNISTD_H

#include "html.tab.hh"
#include "html.yy.hh"
//undef macro which will be redefined below
#undef YY_EXTRA_TYPE

#include "css.tab.hh"
#include "css.yy.hh"
#undef YY_EXTRA_TYPE

int html_parse(HtmlTagList*, yyscan_t);
int css_parse(CssSPData*, yyscan_t);

#ifndef NDEBUG
extern int html_debug;
extern int css_debug;
#endif

HtmlParser::HtmlParser(const std::string &html): htmlCode(0)
{
  htmlCode = new HtmlTagList;
  parse(html);
}


HtmlParser::~HtmlParser()
{
    delete htmlCode;
}


void HtmlParser::parse(const std::string& html)
{
  //read the html code - each tag will be one field in std::vector
  YY_BUFFER_STATE bp;

#ifndef NDEBUG
  html_debug=0;
#endif

  yyscan_t scanner;
  html_lex_init_extra(htmlCode, &scanner);

  bp=html__scan_string(html.data(), scanner);
  html__switch_to_buffer(bp, scanner);
  html_parse(htmlCode, scanner);

  html__delete_buffer(bp, scanner);
  html_lex_destroy(scanner);

  //display data
  for ( const HtmlTag &i: htmlCode->getHtmlTags())
    std::cout << i << std::endl;
}


std::vector< HtmlTag* > HtmlParser::findAll(const std::string& query)
{
  std::cout << query << std::endl;

  //read the html code - each tag will be one field in std::vector
  YY_BUFFER_STATE bp;
  CssSPData cssSPData;
  cssSPData.htmlCode=htmlCode;

#ifndef NDEBUG
  css_debug=0;
#endif

  yyscan_t scanner;
  css_lex_init_extra(&cssSPData, &scanner);

  bp=css__scan_string(query.data(), scanner);
  css__switch_to_buffer(bp, scanner);
  css_parse(&cssSPData, scanner);

  css__delete_buffer(bp, scanner);
  css_lex_destroy(scanner);

  std::vector<HtmlTag *> ret;   //convert list of iterators to pointers
  for (const SearchListElement &i: cssSPData.results.getList())
    ret.push_back(const_cast<HtmlTag*>(&(*i)) ); //take of constness

  return ret;
}
