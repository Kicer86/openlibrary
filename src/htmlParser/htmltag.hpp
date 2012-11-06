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


#ifndef HTMLTAG_HPP
#define HTMLTAG_HPP

#include <string>
#include <vector>
#include <ostream>

#include <OpenLibraryExports.hpp>

class OL_EXPORTS HtmlTag
{
  public:
    HtmlTag(bool type=false);
    virtual ~HtmlTag();

    operator std::string() const;
    friend std::ostream& operator << (std::ostream &out, const HtmlTag &htamlTag)
    {
        out << htamlTag.operator std::string();

        return out;
    }

    struct Attr
    {
      Attr(): name(), value()
      {}

      std::string name;
      std::string value;
      bool operator==(const std::string &cmp) const;
    };

    void setLevel(int l);
    int getLevel() const;
    void setType(bool c);
    bool isClosing() const;
    bool isOpening() const;
    void addAttr(const HtmlTag::Attr& attr);
    void setId(const std::string &i);
    std::string getId() const;
    void setText(const std::string &s);
    std::string getText() const;
    bool hasAttr(const std::string &attr) const;
    Attr getAttr(const std::string &attr) const;
    std::vector<Attr> getAttrs(const std::string &attr) const;
    std::string toString() const;

  private:
	struct Data;
	std::unique_ptr<Data> m_data;
};

#endif // HTMLTAG_HPP
