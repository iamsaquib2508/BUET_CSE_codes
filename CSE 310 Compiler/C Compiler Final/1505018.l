%option noyywrap
%{
#include<bits/stdc++.h>
#include "1505018.h"
#include "y.tab.h"
#define ffr(i,a,b) for(i=a;i<b;i++)
#define mm(a,b) memset((a),(b),sizeof(a))
#define pf printf

using namespace std;

void yyerror(char*);
extern YYSTYPE yylval;

int linenoc=1;

int ascval(char tok2)
{
	if(tok2=='n') return (int)'\n';
	else if(tok2=='b') return (int)'\b';
	else if(tok2=='f') return (int)'\f';
	else if(tok2=='r') return (int)'\r';
	else if(tok2=='t') return (int)'\t';
	else if(tok2=='v') return (int)'\v';
	else if(tok2=='0') return (int)'\0';
	else if(tok2=='\n') return -1;
	else return (int)tok2;
}
#define bug pf("got in lexer")
%}
alphanumeric [a-zA-Z0-9_]
letter       [a-zA-Z_]
iden         {letter}{alphanumeric}*
digit        [0-9]
num          {digit}+
nl 	 \n
scharwbslash (\\[\'\"\\bfnrtv0])
schar	 ([^\'\\\b\f\n\r\t\v\0]|(\\[^\n\b\f\r\t\v\0]))
strchar 	 [^\n\b\f\r\t\v\\\"\0])
comchar 	 [^\n\\]
sline  	 (([^\n\b\f\r\t\v\\\0]|(\\\"))*((\\*[^\n\b\f\r\t\v\\\"\0])+|(\\\\)*(\\[\\\"]))+)*
scom 	 ({comchar})*(\\*({comchar})+)*
integer   ({num}(E+?{num})?)
floating  (({num}?\.{num}(E[-+]?{num})?)|({num}E\-{num}))
%%
{nl}			{linenoc++;}
if 			{return IF;}
else 			{return ELSE;}
for 			{return FOR;}
while 			{return WHILE;}
do			{return DO;}
break 			{return BREAK;}
int 			{return INT;}
char 			{return CHAR;}
float 			{return FLOAT;}
double 			{return DOUBLE;}
void 			{return VOID;}
return 			{return RETURN;}
switch 			{return SWITCH;}
case 			{return CASE;}
default 			{return DEFAULT;}
continue 			{return CONTINUE;}
println           {return PRINTLN;}
[,] 			{return COMMA;}
[;]			{return SEMICOLON;}
[=]			{return ASSIGNOP;}
[!]			{return NOT;




}
\(|\) 			{//Insert LPAREN

if(strcmp(yytext,"(")==0){return LPAREN;}
else{return RPAREN;}

}
\{|\} 			{//Insert LCURL
if(strcmp(yytext,"{")==0){return LCURL;}
else{return RCURL;}

}
\[|\]			{//Insert LTHIRD
if(strcmp(yytext,"[")==0){return LTHIRD;}
else{return RTHIRD;}





}
(\+\+)|(\-\-)		{
	string type="INCOP";
	if(yytext[0]=='+') return INCOP;
	else return DECOP;
}
[-+]			{
	string type="ADDOP";
	SymbolInfo * s = new SymbolInfo(yytext, type);
	yylval.sym = s;
	return ADDOP;
}
(\*)|(\/)|(\%)			{
	string type="MULOP";
	SymbolInfo * s = new SymbolInfo(yytext, type);
	yylval.sym = s;
	return MULOP;
}
(&&)|(\|\|)			{//Insert LOGICOP
	string type="LOGICOP";
	SymbolInfo * s = new SymbolInfo(yytext, type);
	yylval.sym = s;
	return LOGICOP;
}
(&)|(\|)|(\^)|(<<)|(>>)			{//Insert BITOP
	string type="BITOP";
	SymbolInfo * s = new SymbolInfo(yytext, type);
	yylval.sym = s;
	return BITOP;
}
(<=|<|>|>=|==|!=)	{//RELOP
	string type="RELOP";
	SymbolInfo * s = new SymbolInfo(yytext, type);
	yylval.sym = s;
	return RELOP;
}
{integer}		{//Insert const int
	string type="CONST_INT";
	SymbolInfo * s = new SymbolInfo(yytext, type);

	yylval.sym = s;
	return CONST_INT;
}
{floating}		{//Insert const double, //e-?
	string type="CONST_FLOAT";
	SymbolInfo * s = new SymbolInfo(yytext, type);
	yylval.sym = s;
	//pf("%s in lexer\n", yytext);
	return CONST_FLOAT;
}
{integer}{iden} {//
//errorprint(linenoc,"Invalid prefix on ID", yytext);
}
{floating}{iden} {//
//errorprint(linenoc,"Invalid prefix on ID", yytext);
}
{integer}\.{floating} {
//errorprint(linenoc,"Too many decimal point", yytext);
}
{num}E[-+]?{floating} {//ill form
//errorprint(linenoc,"Ill formed number", yytext);




}
{iden}  {
	string type="ID";
	SymbolInfo * s = new SymbolInfo(yytext, type);
	s->show=s->getName();
	yylval.sym = s;

	return ID;
}
%%
