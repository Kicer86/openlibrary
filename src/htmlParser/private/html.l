%{
  #define YYSTYPE std::string    //kontener na dane leksera
  #ifdef YYDEBUG
    #include <iostream>
  #endif
  #include <string>
  #include <vector>

  #include "private/htmltaglist.hpp"
  #include "html.tab.hh"         //include bison's definitions
%}

%option nounistd
%option never-interactive
%option reentrant
%option bison-bridge
%option noyywrap
%option extra-type="HtmlTagList*"
%option prefix="html_"

%x PARAM
%x PARAM2
%x COMMENT
%x TAG

%%

"<!--"        { //comment
                #ifdef YYDEBUG
                  std::clog << "HL: entering COMMENT" << std::endl;
                #endif
                BEGIN(COMMENT);
              }

"<!"          { //parameter
                #ifdef YYDEBUG
                  std::clog << "HL: entering <!" << std::endl;
                #endif
                BEGIN(PARAM);
              }

"<%"          { //parameter
                #ifdef YYDEBUG
                  std::clog << "HL: entering <%" << std::endl;
                #endif
                BEGIN(PARAM);
              }

"<"           { //tag
                #ifdef YYDEBUG
                  std::clog << "HL: entering TAG" << std::endl;
                #endif
                BEGIN(TAG);
                return yytext[0];
              }

[^<"']*       { //text between tags
                #ifdef YYDEBUG
                  std::clog << "HL: got text: " << yytext << std::endl;
                #endif
                *yylval=yytext;
                return TEXT;
              }

<*>\n         {}  //eat endl (useless in html)
<INITIAL,TAG>\"(\\.|[^"])*\"  { //string
                                #ifdef YYDEBUG
                                  std::clog << "HL: got \"string: " << yytext << " while in INITIAL or TAG" << std::endl;
                                #endif
                                *yylval=yytext;
                                return STRING;
                              }

<INITIAL,TAG>\'(\\.|[^'])*\'  { //string
                                #ifdef YYDEBUG
                                  std::clog << "HL: got 'string: " << yytext << " while in INITIAL or TAG" << std::endl;
                                #endif
                                *yylval=yytext;
                                return STRING;
                              }

<COMMENT>{
"-->"   {
          #ifdef YYDEBUG
            std::clog << "HL: leaving COMMENT" << std::endl;
          #endif
          BEGIN(INITIAL);
        }
.       {} //drop all characters
}

<PARAM>{
">"       {
            #ifdef YYDEBUG
              std::clog << "HL: leaving <!" << std::endl;
            #endif
            BEGIN(INITIAL);
          }
.         {} //drop all characters
}

<PARAM2>{
"%>"          {
                #ifdef YYDEBUG
                  std::clog << "HL: leaving <%" << std::endl;
                #endif
                BEGIN(INITIAL);
              }
.             {} //drop all characters
}

<TAG>{
[[:alnum:]_-]+  { //text
                  #ifdef YYDEBUG
                    std::clog << "HL: got " << yytext << " while in TAG" << std::endl;
                  #endif
                  *yylval=yytext;
                  return TEXT;
                }

[[:blank:]$]+ {} //remove blanks

">"           { //tag end - return to main state
                #ifdef YYDEBUG
                  std::clog << "HL: leaving TAG" << std::endl;
                #endif
                BEGIN(INITIAL);
                return yytext[0];
              }

.             { //some character
                #ifdef YYDEBUG
                  std::clog << "HL: got character: " << yytext[0] << " while in TAG" << std::endl;
                #endif
                return yytext[0];
              }
}
%%
