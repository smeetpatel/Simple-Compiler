#ifndef	_uniform_table_h_included_
#define _uniform_table_h_included_
#include <string>
using std::string;
struct ut{
	string symb;
	string token_name;
};
extern int count;
extern int size_ut;
extern struct ut *header;
void create_ut();
void expand_ut();
void insert(string,string);
void print_ut();

#endif
