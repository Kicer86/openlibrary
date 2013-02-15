
%pure_parser
%error-verbose
%parse-param { HtmlTagList* htmlStruct }
%parse-param {void *scanner}
%lex-param {yyscan_t *scanner}
%name-prefix="html_"

%{
/*   #define YYPARSE_PARAM scanner  */
/*   #define YYLEX_PARAM   scanner  */

  #define YYSTYPE std::string
  #define TAG htmlTag
  #define TAGS htmlStruct

  //define this macro to satisfy VisualStudio
  #define YY_NO_UNISTD_H

  #include <iostream>
  #include <string>
  #include <vector>

  #include "private/htmltaglist.hpp"
  #include "html.yy.hh"

  int html_error(HtmlTagList* htmlStruct, yyscan_t scanner, char const* s)
  {
    std::cout << "error: " << s << std::endl;
    return 1;
  }

  HtmlTag htmlTag;
%}

%token TEXT
%token STRING

%%

//eat lines
input:    /* empty */;
input:    input html_element;

html_element:  text     { std::cerr << "Warning: orphaned text element: " << $1 << std::endl; }; //element text
html_element:  tag      {  };                  // element is <tag>
html_element:  tag text { TAG.setText($2); };  // <tag> some text


tag:      '<'           { TAG.clear(); TAG.setType(false); } //add new tag to list
          tag_body '>'  {
                          TAGS->addElement(htmlTag);
                          if (TAG.getId()=="br" || TAG.getId()=="embeded" || TAG.getId()=="hr" ||
                              TAG.getId()=="img" || TAG.getId()=="input" || TAG.getId()=="link" ||
                              TAG.getId()=="li" || TAG.getId()=="meta" || TAG.getId()=="option" ||
                              TAG.getId()=="p" /* ? */)
                              {  //for this tags automatically add closing tag
                                HtmlTag newClosingTag(true);
                                newClosingTag.setId(TAG.getId());
                                TAGS->addElement(newClosingTag);
                              }
                        };

tag:      '<' '/'       { TAG.clear(); TAG.setType(true); } //add new tag to list
          tag_body '>'  { TAGS->addElement(htmlTag); };  //html /tag

tag_body: TEXT          { TAG.setId($1); };   //html tag is <tag>
tag_body: TEXT attr     { TAG.setId($1); };   //or <tag attr>
tag_body: tag_body '/'  {
                          // add closing tag
                          HtmlTag newClosingTag(true);
                          newClosingTag.setId(TAG.getId());
                          TAGS->addElement(newClosingTag);
                        }

//html's tag attributes

attr:     attr_opt;       //<tag attr>
attr:     attr attr_opt;  //or <tag attr attr>

//attribute variants
attr_opt:     TEXT                { HtmlTag::Attr newAttr; newAttr.name=$1; TAG.addAttr(newAttr); };  //attr is just some attr <tag attr>
attr_opt:     TEXT '=' TEXT       { HtmlTag::Attr newAttr; newAttr.name=$1; newAttr.value=$3;     TAG.addAttr(newAttr); };  //or <tag attr=some_letters/numbers>
attr_opt:     TEXT '=' '#' TEXT   { HtmlTag::Attr newAttr; newAttr.name=$1; newAttr.value='#'+$4; TAG.addAttr(newAttr); };  //or <tag attr=#some_letters/numbers>
attr_opt:     TEXT '=' TEXT '%'   { HtmlTag::Attr newAttr; newAttr.name=$1; newAttr.value=$3+'%'; TAG.addAttr(newAttr); };  //or <tag attr=some_letters/numbers%>
attr_opt:     TEXT '=' STRING     { HtmlTag::Attr newAttr; newAttr.name=$1; newAttr.value=$3;     TAG.addAttr(newAttr); };  //or <tag attr="some_letters">


//text between tags
text:     text_opt;
text:     text text_opt       { $$ = $1 + $2; };

// text variants
text_opt: STRING;
text_opt: TEXT;

%%
