%{
	#include "header.h"
	int lineno=1;
%}
/*regular definitions*/
airthmetic_operator	("+"|"-"|"*"|"/"|"%")
relational_operator	(==|!=|<|>|<=|>=)
punctuations	(;|,|"."|"\""|"/*"|"*/"|"//")
brackets	("("|")"|"{"|"}"|"["|"]")
assignment_operator	(=)
illegal_characters	("~"|"`"|"!"|"@"|"#"|"$"|"^"|"&"|"_"|"|"|"\\"|":"|"'"|"?")
keywords	("and"|"not"|"or"|"if"|"then"|"else"|"for"|"class"|"int"|"float"|"get"|"put"|"return"|"program")
nonzero	[1-9]
digit	[0-9]
letter	[a-z]|[A-Z]
fraction	"."({digit}*){nonzero}|".0"
integer	{nonzero}({digit}*)|"0"
floatno	{digit}+"."{digit}*
num	{integer}|{floatno}
id	{letter}({letter}|{digit}|"_")*
%%
"<>"	return ANGULAR;
{relational_operator}	return RELATIONALOP;
{punctuations}	return PUNCTUATIONS;
{airthmetic_operator}	return AIRTHMETICOP;
"="	return ASSIGNMENT;
{brackets}	return BRACKETS;
{illegal_characters}+({letter}|{num}|{illegal_characters}|{punctuations})	return UNIDENTIFIED_CHARACTER;
{keywords}	return KEYWORD;
[ ]*	return SPACE;
\t	return WHITESPACE;
({integer}|{floatno})+({letter}|{illegal_characters})+({letter}|{digit}|{illegal_characters})*	return INVALID_STRING;
("_")+({letter}|{digit}|{illegal_characters})+({letter}|{digit}|{illegal_characters}|"_")*	return INVALID_STRING;
{letter}+({num}|"_")("~"|"`"|"!"|"@"|"#"|"$"|"^"|"&"|"|"|"\\"|":"|"'"|"?")+({letter}|{num}|{illegal_characters}|{punctuations})	return INVALID_STRING;
{id}	return ID;
{integer}	return INTEGER;
{floatno}	return FLOATNO;
{fraction}	return FLOATNO;
\n	{lineno++; return NEWLINE;}
%%

int yywrap(void)
{
	return 1;
}
