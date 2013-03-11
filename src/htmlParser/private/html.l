%{
  #define YYSTYPE std::string    //kontener na dane leksera
  
  #include <string>
  #include <vector>
  
  #include <debug.hpp>

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
                debug(DebugLevel::Debug) << "HL: entering COMMENT";

                BEGIN(COMMENT);
              }

"<!"          { //parameter
                debug(DebugLevel::Debug) << "HL: entering <!";
                
                BEGIN(PARAM);
              }

"<%"          { //parameter
                debug(DebugLevel::Debug) << "HL: entering <%";

                BEGIN(PARAM);
              }

"<"           { //tag
                debug(DebugLevel::Debug) << "HL: entering TAG";

                BEGIN(TAG);
                return yytext[0];
              }

[^<"']*       { //text between tags
                debug(DebugLevel::Debug) << "HL: got text: " << yytext;

                *yylval=yytext;
                return TEXT;
              }

<*>\n         {}  //eat endl (useless in html)
<INITIAL,TAG>\"(\\.|[^"])*\"  { //string
                                debug(DebugLevel::Debug) << "HL: got \"string: " << yytext << " while in INITIAL or TAG";

                                *yylval=yytext;
                                return STRING;
                              }

<INITIAL,TAG>\'(\\.|[^'])*\'  { //string
                                debug(DebugLevel::Debug) << "HL: got 'string: " << yytext << " while in INITIAL or TAG";

                                *yylval=yytext;
                                return STRING;
                              }

<COMMENT>{
"-->"   {
          debug(DebugLevel::Debug) << "HL: leaving COMMENT";

          BEGIN(INITIAL);
        }
.       {} //drop all characters
}

<PARAM>{
">"       {
            debug(DebugLevel::Debug) << "HL: leaving <!";

            BEGIN(INITIAL);
          }
.         {} //drop all characters
}

<PARAM2>{
"%>"          {
                debug(DebugLevel::Debug) << "HL: leaving <%";

                BEGIN(INITIAL);
              }
.             {} //drop all characters
}

<TAG>{
[[:alnum:]_-]+  { //text
                  debug(DebugLevel::Debug) << "HL: got " << yytext << " while in TAG";

                  *yylval=yytext;
                  return TEXT;
                }

[[:blank:]$]+ {} //remove blanks

">"           { //tag end - return to main state
                debug(DebugLevel::Debug) << "HL: leaving TAG";

                BEGIN(INITIAL);
                return yytext[0];
              }

.             { //some character
                debug(DebugLevel::Debug) << "HL: got character: " << yytext[0] << " while in TAG";

                return yytext[0];
              }
}
%%
