%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "CoolLexer.h"

#undef YY_DECL
#define YY_DECL int CoolLexer::yylex()

%}

white_space       [ \f\r\t\v\b]*
digit             [0-9]
alpha             [A-Za-z_]
ualpha            [A-Z]
lalpha            [a-z]
alpha_num         ({alpha}|{digit})
identifier_type   {ualpha}{alpha_num}*
identifier_object {lalpha}{alpha_num}*
identifier_other	{alpha}{alpha_num}*
integer           {digit}+
line_comment      --.*

%x STRING
%x COMMENT

%option warn nodefault batch noyywrap c++
%option yylineno
%option yyclass="CoolLexer"

%%

"(*"                 { BEGIN(COMMENT); comment_level = 1; }
<COMMENT>"(*"        { ++comment_level; }
<COMMENT><<EOF>>     Error("a comment may not contain EOF");
<COMMENT>\n          { ++lineno; }
<COMMENT>.           { }
<COMMENT>"*)"        { --comment_level; if (comment_level == 0) { BEGIN(INITIAL); } }

{line_comment}       { }

"\""                 { BEGIN(STRING); yymore(); }
<STRING><EOF>        Error("a string may not contain EOF");
<STRING>\n           Error("a string may not contain newline");
<STRING>\0           Error("a string may not contain the null");
<STRING>[^\\\"\n]*   yymore();
<STRING>\\\n         { ++lineno; yymore(); }
<STRING>\\					 yymore();
<STRING>"\""         { BEGIN(INITIAL); EscapeStringLiteral(); return TOKEN_STRING; }

(?i:class)           return TOKEN_CLASS;
(?i:inherits)        return TOKEN_INHERITS;
(?i:if)              return TOKEN_IF;
(?i:then)            return TOKEN_THEN;
(?i:else)            return TOKEN_ELSE;
(?i:fi)              return TOKEN_FI;
(?i:while)           return TOKEN_WHILE;
(?i:loop)            return TOKEN_LOOP;
(?i:pool)            return TOKEN_POOL;
(?i:case)            return TOKEN_CASE;
(?i:of)              return TOKEN_OF;
(?i:let)             return TOKEN_LET;
(?i:in)              return TOKEN_IN;
(?i:isvoid)          return TOKEN_ISVOID;
(?i:new)             return TOKEN_NEW;
(?i:esac)            return TOKEN_ESAC;
(?i:not)             return TOKEN_NOT;
t(?i:rue)            return TOKEN_TRUE;
f(?i:alse)           return TOKEN_FALSE;

"<-"                 return TOKEN_ASSIGN;
";"                  return TOKEN_SEMICOLON;
":"                  return TOKEN_COLON;
"."                  return TOKEN_DOT;
","                  return TOKEN_COMMA;
"@"                  return TOKEN_AT;
"=>"                 return TOKEN_ARROW_RIGHT;
"("                  return TOKEN_REGULAR_OPEN;
")"                  return TOKEN_REGULAR_CLOSE;
"{"                  return TOKEN_CURLY_OPEN;
"}"                  return TOKEN_CURLY_CLOSE;
"["                  return TOKEN_SQUARE_OPEN;
"]"                  return TOKEN_SQUARE_CLOSE;
"+"                  return TOKEN_PLUS;
"-"                  return TOKEN_MINUS;
"*"                  return TOKEN_MUL;
"/"                  return TOKEN_DIV;
"~"                  return TOKEN_TILDE;
"<="                 return TOKEN_LESS_EQUAL;
"<"                  return TOKEN_LESS;
"="                  return TOKEN_EQUAL;

{integer}            return TOKEN_INTEGER;

{identifier_type}    return TOKEN_IDENTIFIER_TYPE;
{identifier_object}  return TOKEN_IDENTIFIER_OBJECT;
{identifier_other}   return TOKEN_IDENTIFIER_OTHER;

{white_space}        { }
\n                   ++lineno;
.                    Error("unrecognized character");

%%

void CoolLexer::Error(const char* msg) const {
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}

void CoolLexer::EscapeStringLiteral() const {
  const char *it = yytext + 1;
  char *str = yytext;
  
  while (*it != '\"') {
  	if (*it != '\\') {
  		*str++ = *it++;
  		continue;
  	}

    switch (*++it) {
      case 'b':
        *str = '\b';
        break;
      case 't':
        *str = '\t';
        break;
      case 'n':
        *str = '\n';
        break;
      case 'f':
        *str = '\f';
        break;
      default:
        *str = *it;
        break;
    }

    ++str;
    ++it;
  }

  *str = '\0';
}

