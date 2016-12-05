#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include "bst_gram.h"
#include "utility.h"
using std::cout;
using std::endl;
using std::set;
extern int lineno;
deque<string>	inputTable;
stack<string>	derivationTable;
string isNT(string str)	//extracts first NT if it exists
{
	int i=0;
	string extracted_nt;
	if(str[i]=='<')	//to see if not"<>"
	{
		extracted_nt+=str[i++];
		while(str[i]!='>' && str[i]!='\0')
			extracted_nt+=str[i++];
		if((i==1 && str[i]=='>') || str[i]=='\0'||str[i]==' ')
			return "";
		extracted_nt+=str[i];
		return extracted_nt;
	}
	else
		return extracted_nt;
}

string getTerminal(string str)	//extracts the first terminal, if it exists
{
	int i=0;
	string extracted_ter;
	while(str[i]>=97 && str[i]<=122)
	{
		extracted_ter += str[i++];
	}
	return extracted_ter;
}

string getNumber(string str)	//extracts the first number, if it exists
{
	int i=0;
	string number;
	while(str[i]>=48 && str[i]<=57)
	{
		number += str[i++];
	}
	return number;
}

string getChar(string str)	//extracts the character, if it exists
{
	string extracted_op;
	if(str[0]=='+' || str[0]=='-'|| str[0]=='*'|| str[0]=='/' || str[0]=='{' || str[0]=='(' || str[0]=='[' || str[0]==',' ||str[0]==';' ||str[0]==')' ||str[0]==']' ||str[0]=='}' || str[0]=='.' || str[0]=='$')
		return extracted_op+=str[0];
	else{
		string s=str.substr(0,2);
		if(s=="=="||s=="<="||s==">="||s=="<>")
			return s;
	}
	if(str[0]=='=' || str[0]=='<' || str[0]=='>')
		return extracted_op+=str[0];
	return extracted_op;
}

void compute_first(string pro)
{
	/*for passing to "STRTOK":*/
	char *pass=new char[pro.size()+1];
	copy(pro.begin(),pro.end(),pass);
	pass[pro.size()]='\0';
	char *tok;
	vector<string> sub_strings;
	tok=strtok(pass,"\t");
	sub_strings.push_back(tok);
	while(tok!=NULL)
	{
		tok=strtok(NULL,"|");
		if(tok!=NULL)
			sub_strings.push_back(tok);
	}
	//Have Divided the string into apt strings
	struct grammar *node = search(head,sub_strings[0]);
	if(sub_strings.size()==1 || (sub_strings.size()==2 && sub_strings[1]=="epsilon"))	
	{
		node->first.push_back(sub_strings[0]);
		node->first_flag=true;
		return;
	}
	//above if condition checks first condition to find first
	//traverse through all substrings:
	string extract1;
	bool bit=0;//as flag for epsilon or not
	for(int i=1;i<sub_strings.size();i++)
	{
		up:
		extract1=isNT(sub_strings[i]);
		if(extract1.empty())
		{
			extract1=getTerminal(sub_strings[i]);
			if(!extract1.empty())
				node->first.push_back(extract1);
			else
			{
				extract1=getChar(sub_strings[i]);
				if(!extract1.empty())
					node->first.push_back(extract1);
				else {
					extract1=getNumber(sub_strings[i]);
					if(!extract1.empty())
						node->first.push_back(extract1);
					else
						cout<<"Syntax error:Wrong terminal"<<endl;
				}
				
			}
		}
		else
		{
			struct grammar *temp_node=search(head,extract1);
			if(temp_node==NULL)
				continue;
			if(temp_node->first_flag==false)
				compute_first(temp_node->production);
			bit=0;	//as flag for epsilon or not
			for(int j=0;j<temp_node->first.size();j++)
			{
				if(temp_node->first[j]!="epsilon")
					node->first.push_back(temp_node->first[j]);
				else
					bit=1;
			}
			if(bit==0)	
				continue;
			else
			{
				string sub_sub_string=sub_strings[i].substr(extract1.length());
				if(!sub_sub_string.empty())
				{	
					sub_strings[i]=sub_sub_string;
					goto up;
				}
				else{
					node->first.push_back("epsilon");
				}
			}	
		}
	}
	node->first_flag=true;
	set<string> s(node->first.begin(),node->first.end());
	node->first.assign(s.begin(),s.end());
	s.erase(s.begin(),s.end());
	sub_strings.erase(sub_strings.begin(),sub_strings.end());
}

void compute_follow(struct grammar *pro,string parent)
{
	string nt1=pro->nt;
	vector<struct grammar *> possible=search_nt_containing_pros(head,nt1);
	for(int i=0;i<possible.size();i++)
	{
		string nt2=possible[i]->nt;
		if(nt1!=nt2)
		{
			vector<string>	sub_strings;
			char *pass=new char[possible[i]->production.size()+1];
			copy(possible[i]->production.begin(),possible[i]->production.end(),pass);
			pass[possible[i]->production.size()]='\0';
			char *tok;
			tok=strtok(pass,"\t");
			while(tok!=NULL)
			{
				tok=strtok(NULL,"|");
				if(tok!=NULL)
				{
					sub_strings.push_back(tok);
				}
			}
			struct grammar *node;
			for(int j=0;j<sub_strings.size();j++)
			{
				secondCheck:
				size_t found=sub_strings[j].find(nt1);
	
				if(found!=std::string::npos)
				{
	
					sub_strings[j]=sub_strings[j].substr(found+nt1.length());
					up:
					if(sub_strings[j].empty())
					{
						if(parent==nt2)
						{
							struct grammar *pnode=search(head,parent);
							if(!pnode->follow.empty())
							{
								for(int k=0;k<pnode->follow.size();k++)
								{
									pro->follow.push_back(pnode->follow[k]);	
								}
							}	
						}	
						else
						{
							node=search(head,nt2);
							if(node->follow_flag==false)
								compute_follow(node,nt1);
							for(int k=0;k<node->follow.size();k++)
								pro->follow.push_back(node->follow[k]);	
						
						}
					}
					else
					{
						string extract1=isNT(sub_strings[j]);
						if(extract1.empty())
						{
							extract1=getTerminal(sub_strings[j]);
							if(extract1.empty())
							{
								extract1=getChar(sub_strings[j]);
								if(extract1.empty())
								{
									extract1=getNumber(sub_strings[j]);
									if(extract1.empty())				
									{		
										cout<<"SYNTAX ERROR"<<endl;
										continue;
									}
								}	
							}
							pro->follow.push_back(extract1);
							continue;
						
						}
						bool epsilon_flag=false;
						node=search(head,extract1);
						for(int k=0;k<node->first.size();k++)
						{
							if(node->first[k]=="epsilon")
								epsilon_flag=true;
							else
								pro->follow.push_back(node->first[k]);
						}
						if(epsilon_flag)
						{
							sub_strings[j]=sub_strings[j].substr(extract1.length());	
							if(sub_strings[j].empty())
							{
								node=search(head,nt2);
								if(node->follow_flag==false)
									compute_follow(node,nt1);
								for(int k=0;k<node->follow.size();k++)
									pro->follow.push_back(node->follow[k]);	
							}
							else
								goto up;
						}
					
					}
				}
				else
					continue;
					
				if(sub_strings[j].find(nt1)!=std::string::npos)
					goto secondCheck;
			}
			sub_strings.erase(sub_strings.begin(),sub_strings.end());
			delete pass;
		
		}
	}
	set<string> s(pro->follow.begin(),pro->follow.end());
	pro->follow.assign(s.begin(),s.end());
	s.erase(s.begin(),s.end());
	pro->follow_flag=true;
}

bool checkSyntax()
{
	bool flag=true;
	pair<string,string>	combination;	//For Searching the map
	string toBeMatched=inputTable.front();
	inputTable.pop_front();
	string nextToken=inputTable.front();
	string nonTer=isNT(derivationTable.top());
	string temp,mid;
	while(nonTer != "$" || toBeMatched!="$")
	{
		//match
		if(nonTer==toBeMatched)
		{
			mid=derivationTable.top();
			derivationTable.pop();
			size_t found =mid.find(nonTer);
			mid=mid.substr(found+nonTer.length());
			if(mid[0]==' ')
				mid=mid.substr(1);
			if(!mid.empty())
				derivationTable.push(mid);
		
			toBeMatched=inputTable.front();
			inputTable.pop_front();
			nextToken=inputTable.front();
		}
		//predict
		else
		{
			getextract:
			nonTer=isNT(derivationTable.top());

			if(!nonTer.empty())
			{
				mid=derivationTable.top();
				derivationTable.pop();
				size_t found =mid.find(nonTer);
				mid=mid.substr(found+nonTer.length());
				if(mid[0]==' ')
					mid=mid.substr(1);
				if(!mid.empty())
					derivationTable.push(mid);
				combination=make_pair(nonTer,toBeMatched);
				if(!tt[combination].empty())	//path exists
				{
					mid=tt[combination];
					temp=checkFormat(mid);
					if((temp[0]=='<' || temp[0]=='>') && temp.length()==1)
					{
						mid=derivationTable.top();
						temp=checkFormat(mid);
					}
					else if(temp[0]!='<')	//not a NT
					{
						size_t found=mid.find(temp);
						mid=mid.substr(found+temp.length());
						if(mid[0]==' ')
							mid=mid.substr(1);
						if(!mid.empty())	
							temp=checkFormat(mid);
						else
						{
							mid=derivationTable.top();
							temp=checkFormat(mid);
						}
					}
					else
					{
						struct grammar *tp=search(head,temp);
						mid=tp->production;
						if(toBeMatched!="<")
							temp=toBeMatched;
						else
							temp=" ";
						size_t found=mid.find(temp);
						if(found==std::string::npos)
						{
							found=mid.find("|");
							if(found!=std::string::npos)
							{
								mid=mid.substr(found);
							}
							else
							{
							found=mid.find(checkFormat(tt[combination]));
							temp=checkFormat(tt[combination]);
							mid=mid.substr(found+temp.length());
							}
						}
						else
							mid=mid.substr(found+temp.length());
						if(mid.empty())
						{
							mid=tt[combination];
							temp=checkFormat(mid);
						}
						else if(mid[0]=='\t')
						{
							temp="";
							for(int i=0;i<mid.length();i++)
							{
								if(mid[i]!='\t')
									temp+=mid[i];
							}	
							mid=temp;
						}
						else if(mid[0]=='>')
							mid=mid.substr(2);
						temp=checkFormat(mid);
			
						if(mid[0]=='|')
						{
							mid=tt[combination];
							temp=checkFormat(mid);
							found=mid.find(temp);
							mid=mid.substr(found+temp.length());
							if(mid[0]==' ')
								mid=mid.substr(1);
							temp=checkFormat(mid);
						}
						else if(temp[0]=='<')
						{
						 	found=mid.find(temp);
						 	mid=mid.substr(found+temp.length());
							if(mid[0]==' ')
								mid=mid.substr(1);
							temp=checkFormat(mid);
						}
						else
						{
							size_t found=mid.find(temp);
							mid=mid.substr(found+temp.length());
							if(mid[0]==' ')
								mid=mid.substr(1);
							if(!mid.empty())	
								temp=checkFormat(mid);
							else
							{
								mid=derivationTable.top();
								temp=checkFormat(mid);
							}
						}
					}
					if(!tt[make_pair(temp,nextToken)].empty() || !tt[make_pair(temp,"epsilon")].empty() || temp==nextToken || (tt[make_pair(checkFormat(tt[combination]),toBeMatched)].empty() && !tt[make_pair(checkFormat(tt[combination]),"epsilon")].empty()))
					{
						derivationTable.push(tt[combination]);
						goto getextract;
					}
					else
						continue;
				}
				else if(!tt[make_pair(nonTer,"epsilon")].empty())	
				{
					goto getextract;	//check for epsilon entry
				}
				else
				{
					flag=false;
					cout<<"Syntax Error"<<endl;
					cout<<"\""<<toBeMatched<<"\""<<" unexpected"<<endl;
				}
			}
			else	//terminal, char or number
			{
				nonTer=getTerminal(derivationTable.top());
				if(nonTer.empty())
					nonTer=getChar(derivationTable.top());
				if(nonTer.empty())
					nonTer=getNumber(derivationTable.top());
				if(nonTer!=toBeMatched)
				{
					flag=false;
					cout<<"Syntactic Error "<<endl;
					cout<<"\""<<toBeMatched<<"\""<<" unexpected"<<endl;
					derivationTable.pop();
					if(toBeMatched=="$")
						return flag;
					toBeMatched=inputTable.front();
					inputTable.pop_front();
					
				}
			}
		}
	}
	if(inputTable.size()!=0)
	{
		cout<<"Syntactic Error from line: "<<lineno<<endl;
		flag=false;
	}
	return flag;
}

string checkFormat(string s)
{
	string temp;
	temp=isNT(s);
	if(temp.empty())
	{
		temp=getTerminal(s);
		if(temp.empty())
		{
			temp=getChar(s);
			if(temp.empty())
				temp=getNumber(s);
		}
	}
	return temp;
}
