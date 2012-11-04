
%pure_parser
%error-verbose
%parse-param { HtmlParser::CssSPData* cssSPData } 
%parse-param {void *scanner}
%lex-param {yyscan_t *scanner}
%name-prefix="css_"

%{         
  #define YYSTYPE std::string 
  
  #include <iostream>
  #include <string>
  #include <vector>
  
  #include "htmlparser.hpp"  
  #include "htmltaglist.hpp"  
  #include "css.yy.hh"  
  
  int css_error(HtmlParser::CssSPData* cssSPData, yyscan_t scanner, char const* s)
  {
    std::cout << "error: " << s << std::endl;
    return 1;
  }
%}

%token TEXT
%token STRING

%%

query: exp         {};
query: query exp   {};

exp:  tag         { //just tag name
                    $$=$1; 
                    std::cout << '(' << $$ << ')' << std::endl;
                    
                    //filtrate all tags matching this ($1) id
                    cssSPData->results.init(cssSPData->htmlCode, $1);
                  }; 
exp:  exp tag     { //descendant of some tag
                    $$='(' + $1 + ' ' + $2 + ')'; 
                    std::cout << $$ << std::endl; 
                    
                    //filtrate all tags that have such descendant
                    cssSPData->results.findDescendant($2);
                  }; 
exp:  exp '>' tag { 
                    $$='(' + $1 + '>' + $3 + ')'; 
                    std::cout << $$ << std::endl; 
                    
                    //
                  }; //child

exp:  exp '[' condition ']'  { 
                                $$ = $1 + '[' + $3 + ']'; 
                                
                                //remove all tags which doesn't have proper attribute
                             };
//some more rulez to be here

tag:  TEXT                    {}; 

condition: TEXT                 { 
                                  $$ = $1; 
                                  //remove all tags which doesn't have proper attribute
                                  cssSPData->results.withAttr($1);
                                };
condition: TEXT '=' value       { 
                                  $$ = $1 + '=' + $3; 
                                  //remove all tags which doesn't have proper attribute
                                  cssSPData->results.withAttr($1, $3);
                                };
condition: TEXT '~' '=' value   { $$ = $1 + "~=" + $4; };
condition: TEXT '|' '=' value   { $$ = $1 + "|=" + $4; };

//for conveniance TEXT or STRING
value: STRING;
value: TEXT;
%%
