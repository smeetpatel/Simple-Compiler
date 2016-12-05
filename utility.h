#ifndef	_utility_h_included_
#define _utility_h_included_
/*Header File that contains utility functions for computing first and follow sets..and probably also for LL(1) parser*/
#include "bst_gram.h"
#include <string>
#include <stack>
#include <deque>
using std::deque;
using std::stack;
using std::string;

extern deque<string>	inputTable;
extern stack<string>	derivationTable;
string isNT(string);
string getTerminal(string);
string getChar(string);
string getNumber(string);
void compute_first(string);
void compute_follow(struct grammar *,string);
bool checkSyntax();
string checkFormat(string);
#endif






