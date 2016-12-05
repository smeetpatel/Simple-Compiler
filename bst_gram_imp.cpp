#include <iostream>
#include <string>
#include <cstring>
#include <stack>
#include "bst_gram.h"
#include "utility.h"

using std::cout;
using std::endl;
using std::stack;
using std::string;
struct grammar *head;
map<pair<string,string>,string> tt;

struct grammar *newNode(string pro)
{
	char *pass=new char[pro.size()+1];
	copy(pro.begin(),pro.end(),pass);
	pass[pro.size()]='\0';
	struct grammar *newnode=new struct grammar;
	newnode->nt=std::strtok(pass,"\t");
	newnode->production=pro;
	newnode->first_flag=false;
	newnode->follow_flag=false;
	newnode->left=newnode->right=NULL;
	return newnode;
}

struct grammar *addRule(struct grammar* node,string pro)
{
	if(node==NULL)	return newNode(pro);
	if(strcmp(node->production.c_str(),pro.c_str())==0)
		cout<<"Ambigous Grammar Error"<<endl;
	else if(strcmp(node->production.c_str(),pro.c_str())>0)
		node->left=addRule(node->left,pro);
	else
		node->right=addRule(node->right,pro);
	
	return node;
}

struct grammar *search(struct grammar *root,string nt)
{
	if(root==NULL|| (strcmp(root->nt.c_str(),nt.c_str())==0))
		return root;
	if(strcmp(root->nt.c_str(),nt.c_str())<0)
		return search(root->right,nt);

	return search(root->left,nt);
}

void print_first(struct grammar *root)
{
	if(root!=NULL)
	{
		cout<<"NT: "<<root->nt<<"\tFirst Size: "<<root->first.size()<<endl;
		for(int i=0;i<root->first.size();i++)
			cout<<root->first[i]<<"\t";
		cout<<endl;
		print_first(root->left);
		print_first(root->right);
	}
}
void print_tree(struct grammar *root)
{
	if(root!=NULL)
	{
		cout<<"NT: "<<root->nt<<"\t"<<"PRODUCTION: "<<root->production<<endl;
		print_tree(root->left);
		print_tree(root->right);
	}
}   

void first_calc(struct grammar *root)
{
	if(root!=NULL)
	{
		if(root->first_flag==false)
			compute_first(root->production);
		first_calc(root->left);
		first_calc(root->right);
	}
}


vector<struct grammar *>	search_nt_containing_pros(struct grammar *root,string nt1)
{
	vector<struct grammar *> possible;
	stack<struct grammar *>	stac;
	struct grammar *current=root;
	bool done=false;
	
	while(!done)
	{	
		if(current!=NULL)
		{
			stac.push(current);
			current=current->left;
		}
		else
		{
			if(!stac.empty())
			{
				current=stac.top();
				stac.pop();
				if(current->production.find(nt1)!=std::string::npos)
					possible.push_back(current);
				current=current->right;
			}
			else
				done=true;
		}
	}
	return possible;
}

void follow_calc(struct grammar *root)
{
	if(root!=NULL)
	{
		if(root->follow_flag==false)
			compute_follow(root,"");
		follow_calc(root->left);
		follow_calc(root->right);
	}
}

void print_first_follow(struct grammar *root)
{
	int i;
	if(root!=NULL)
	{
		cout<<"NT: "<<root->nt<<endl;
		cout<<"First Size: "<<root->first.size()<<endl;
		for(i=0;i<root->first.size();i++)
			cout<<root->first[i]<<"\t";
		cout<<"\nFollow Size: "<<root->follow.size()<<endl;
		for(i=0;i<root->follow.size();i++)
			cout<<root->follow[i]<<"\t";
		cout<<endl;
		print_first_follow(root->left);
		print_first_follow(root->right);
	}
}

void parsertable(struct grammar *root)
{
	if(root!=NULL)
	{
		char *pass=new char[root->production.size()+1];
		copy(root->production.begin(),root->production.end(),pass);
		pass[root->production.size()]='\0';
		char *tok;
		vector<string>	mid;
		pair<string,string> combination;
		bool epFlag=false;
		tok=strtok(pass,"\t");
		while(tok!=NULL)
		{
			tok=strtok(NULL,"|");
			if(tok!=NULL)
				mid.push_back(tok);
		}
		for(int i=0;i<mid.size();i++)
		{
			string extract1=isNT(mid[i]);
			if(extract1.empty())
			{
				extract1=getTerminal(mid[i]);
				if(extract1.empty())
				{
					extract1=getChar(mid[i]);
					if(extract1.empty())
					{
						extract1=getNumber(mid[i]);
						if(extract1.empty())
						{
							cout<<"GRAMMAR ERROR"<<endl;
						}
					}

				}
				combination=make_pair(root->nt,extract1);
				tt[combination]=mid[i];
			}
			else
			{
				struct grammar *node=search(head,extract1);
				if(node==NULL)
					continue;
				for(int j=0;j<node->first.size();j++)
				{
					if(node->first[j]=="epsilon")
						epFlag=true;
					combination=make_pair(root->nt,node->first[j]);
					tt[combination]=mid[i];
				}
				if(epFlag==true)
				{
					for(int j=0;j<node->follow.size();j++)
					{
						combination=make_pair(root->nt,node->follow[j]);
						tt[combination]=mid[i];
					}
				}
			}
			epFlag=false;
		}
		mid.erase(mid.begin(),mid.end());
		delete pass;
		parsertable(root->left);
		parsertable(root->right);
	}
}

void print_map()
{
	for(map<pair<string,string>,string>::iterator it=tt.begin();it!=tt.end();it++)
	{
		cout<<"Key: "<<it->first.first<<"\t"<<it->first.second<<"\tValue: "<<it->second<<endl;
	}
}
