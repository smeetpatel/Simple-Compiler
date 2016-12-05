#ifndef	_bst_gram_h_included_
#define	_bst_gram_h_included_
#include <string>
#include <vector>
#include <map>
using std::map;
using std::string;
using std::vector;
using std::pair;
struct grammar{
	string nt;
	string	production;
	vector<string>	first;
	vector<string> follow;
	bool first_flag;
	bool follow_flag;
	struct grammar *left;
	struct grammar *right;
};

extern struct grammar *head;
extern map<pair<string,string>,string>	tt;
struct grammar *newNode(string);
struct grammar *addRule(struct grammar*,string);
struct grammar *search(struct grammar *,string);
vector<struct grammar *>	search_nt_containing_pros(struct grammar *,string);
void print_tree(struct grammar*);
void first_calc(struct grammar *);
void follow_calc(struct grammar *);
void print_first_follow(struct grammar *);
void parsertable(struct grammar *);
void print_map();
void print_first(struct grammar *);
#endif
