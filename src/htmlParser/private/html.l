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
                ol_debug(DebugLevel::Debug) << "HL: entering COMMENT";

                BEGIN(COMMENT);
              }

"<!"          { //parameter
                ol_debug(DebugLevel::Debug) << "HL: entering <!";

                BEGIN(PARAM);
              }

"<%"          { //parameter
                ol_debug(DebugLevel::Debug) << "HL: entering <%";

                BEGIN(PARAM);
              }

"<"           { //tag
                ol_debug(DebugLevel::Debug) << "HL: entering TAG";

                BEGIN(TAG);
                return yytext[0];
              }

[^<"']*       { //text between tags
                ol_debug(DebugLevel::Debug) << "HL: got text: " << yytext;

                *yylval=yytext;
                return TEXT;
              }

<*>\n         {}  //eat endl (useless in html)
<INITIAL,TAG>\"(\\.|[^"])*\"  { //string
                                ol_debug(DebugLevel::Debug) << "HL: got \"string: " << yytext << " while in INITIAL or TAG";

                                *yylval=yytext;
                                return STRING;
                              }

<INITIAL,TAG>\'(\\.|[^'])*\'  { //string
                                ol_debug(DebugLevel::Debug) << "HL: got 'string: " << yytext << " while in INITIAL or TAG";

                                *yylval=yytext;
                                return STRING;
                              }

<COMMENT>{
"-->"   {
          ol_debug(DebugLevel::Debug) << "HL: leaving COMMENT";

          BEGIN(INITIAL);
        }
.       {} //drop all characters
}

<PARAM>{
">"       {
            ol_debug(DebugLevel::Debug) << "HL: leaving <!";

            BEGIN(INITIAL);
          }
.         {} //drop all characters
}

<PARAM2>{
"%>"          {
                ol_debug(DebugLevel::Debug) << "HL: leaving <%";

                BEGIN(INITIAL);
              }
.             {} //drop all characters
}

<TAG>{
[[:alnum:]_-]+  { //text
                  ol_debug(DebugLevel::Debug) << "HL: got " << yytext << " while in TAG";

                  *yylval=yytext;
                  return TEXT;
                }

[[:blank:]$]+ {} //remove blanks

">"           { //tag end - return to main state
                ol_debug(DebugLevel::Debug) << "HL: leaving TAG";

                BEGIN(INITIAL);
                return yytext[0];
              }

.             { //some character
                ol_debug(DebugLevel::Debug) << "HL: got character: " << yytext[0] << " while in TAG";

                return yytext[0];
              }
}
%%
