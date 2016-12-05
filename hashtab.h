#ifndef	_hashtab_h_included_
#define	_hashtab_h_included_
#include <string>
using std::string;
struct symbol_table{
	string symbol;
	string type;
	int length;
	struct symbol_table *next;
};

extern int ct;
extern int size;
extern struct symbol_table	*table;
void create_table();
unsigned long hash_function(string);
void expand_table();
void insert(struct symbol_table *,int,string);
struct symbol_table *search(string);
void print_table();
#endif


