%{
  #define YYSTYPE std::string    //kontener na dane leksera
  
  #include <string>
  #include <vector>
  
  #include "htmlparser.hpp" 
  #include "css.tab.hh"         //include bison's definitions
%}


%option never-interactive      
%option reentrant
%option bison-bridge
%option noyywrap
%option extra-type="HtmlParser::CssSPData*"
%option prefix="css_"


%%
      
\".*[^\\]\"   { //string
                *yylval=yytext;
                return STRING;
              }
              
\'.*[^\\]\'   { //string
                *yylval=yytext;
                return STRING;
              }

[[:alnum:]_-]+  { //text
                  *yylval=yytext;
                  return TEXT;
                }

[[:blank:]$]+ {} //remove blanks

.             { //some character            
                return yytext[0];
              }

%%
