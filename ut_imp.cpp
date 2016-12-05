#include <iostream>
#include "uniform_table.h"
using std::cout;
using std::endl;
int count;
int size_ut;
struct ut *header;
void create_ut(){
	count=0;
	size_ut=100;
	header=new struct ut[size_ut];
}
void expand_ut()
{
	int limit=size_ut;
	size_ut=2*size_ut;
	int ct=count;
	count=0;
	struct ut *q=new struct ut[size_ut];
	for(int i=0;i<=ct;i++)
	{
		q[i].symb=header[i].symb;
		q[i].token_name=header[i].token_name;
		insert(q[i].symb,q[i].token_name);
	}
	header=q;
}
void insert(string s1,string s2)
{
	header[count].symb=s1;
	header[count].token_name=s2;
	count++;
	if(count==((75*size_ut)/100))
		expand_ut();
}
void print_ut()
{
	int i=0;
	for(;i<=count;i++)
	{
		cout<<"Symbol: "<<header[i].symb<<" Token Name: "<<header[i].token_name<<endl;
	}
	
}
