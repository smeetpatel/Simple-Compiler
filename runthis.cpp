#include <iostream>
#include <string>
#include <fstream>
#include "header.h"
#include "utility.h"
#include "bst_gram.h"
using namespace std;
extern int yylex();
extern int lineno;
extern char* yytext;

string arr[]={"Angular","RelationalOperator","Punctuations","AirthmeticOperator","Assignment","Brackets","Unidentified Character","Keyword","Space","Whitespace","Newline","id","int","num","Fraction","Invalid String"};

int main()
{
	int ntoken,j=0;
	unsigned long rvalue;
	int access;

	string line;
	ifstream filemanip;
	filemanip.open("grammar.txt",ios::in);
	while(getline(filemanip,line))
		head=addRule(head,line);
	head->follow.push_back("$");
	derivationTable.push("$");
	derivationTable.push(head->nt);	//stack for LL(1) parser
	first_calc(head);	
	follow_calc(head);
	parsertable(head);
	
	
	ntoken=yylex();	//returns apt number from "header.h"
	while(ntoken)
	{
		if(ntoken==7 || ntoken==16)
		{
			cout<<"Error at line: "<<lineno<<endl;
			cout<<"\t"<<"\""<<yytext<<"\""<<" is "<<arr[ntoken-1]<<endl;
		}
		
		if(ntoken==12 || ntoken==13 || ntoken==14 || ntoken==11)
			inputTable.push_back(arr[ntoken-1]);
		else if(ntoken!=9 && ntoken!=10)
				inputTable.push_back(yytext);
		ntoken=yylex();	//read next token
	}
	/*above code stores series of tokens into inputTable*/
	inputTable.push_back("$");	//input string
	//now add every grammar rule to bst and do needful for parser
	
	if(checkSyntax())
		cout<<"Succesful Parse"<<endl;
	else
		cout<<"Unable to compile the program. Errors detacted as indicated."<<endl;
		
	return 0;
}
