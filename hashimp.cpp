#include <iostream>
#include <string>
#include "hashtab.h"
using std::cout;
using std::endl;
using std::string;
int ct;
int size;
struct symbol_table	*table;
void create_table()
{
	ct=0;
	size=100;
	table=new struct symbol_table[size];
}

unsigned long hash_function(string str)
{
	unsigned long hash=5381;
	int c,i=0;
	while(c=str[i])
	{
		hash=((hash<<5)+hash)+c;	//hash*33+c
		i++;
	}
	return ((hash%size));
}
void expand_table()
{
	int key;
	int limit=size;
	size=2*size;
	struct symbol_table *q=new struct symbol_table[size];
	for(int i=0;i<limit;i++)
	{
		if(table[i].symbol!="")
		{
			key=hash_function(table[i].symbol);
			insert(q,key,table[i].symbol);
			struct symbol_table *ptr=new struct symbol_table;
			ptr=table[i].next;
			while(ptr->next!=NULL)
			{
				key=hash_function(ptr->symbol);
				insert(q,key,ptr->symbol);
				ptr=ptr->next;
			}
			key=hash_function(ptr->symbol);
			insert(q,key,ptr->symbol);
		}
	}
	table=q;
}

void insert(struct symbol_table *tab,int key,string sym)
{
	struct symbol_table *ptr=search(sym);
	if(ptr==NULL)
	{
		if(tab[key].symbol=="")
		{
			tab[key].symbol=sym;
			ct++;
			if(ct==((75*size)/100))
			{
				expand_table();
			}
		}
		else
		{
			struct symbol_table *newnode=new struct symbol_table;
			newnode->symbol=sym;
			newnode->next=NULL;
			tab[key].next=newnode;
		}
	}
}

struct symbol_table *search(string value)
{
	unsigned long rvalue =hash_function(value);
	int key=rvalue%size;
	if(table[key].symbol!="")
	{
		if(table[key].symbol==value)
			return ((table)+key);
		else{
			struct symbol_table *ptr=new struct symbol_table;
			ptr=table[key].next;
			while(ptr->symbol!=value)
			{
				ptr=ptr->next;
			}
			if(ptr->symbol!=value)
				return NULL;
			else
				return ptr;
		}
	}
	else
		return NULL;
}

void print_table()
{
	int i=0;
	for(;i<size;i++)
	{
		if(table[i].symbol!="")
		{
			cout<<table[i].symbol;
			if(table[i].next!=NULL)
			{
				struct symbol_table *ptr=new struct symbol_table;
				ptr=table[i].next;
				while(ptr->next!=NULL)
				{
					cout<<" "<<ptr->symbol<<" ";
					ptr=ptr->next;
				}
				cout<<" "<<ptr->symbol<<endl;
			}
			else
				cout<<endl;
		}
		
	}
}
