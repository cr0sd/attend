#pragma once
#include<fox-1.6/fx.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ST FXString
#define ST_TOCS text

class St
{
public:
	ST n;
	ST u;
	ST p;
public:
	const char*name(){return n.ST_TOCS();}
	const char*uname(){return u.ST_TOCS();}
	const char*pw(){return p.ST_TOCS();}

	void name(const char*n){this->n=(FXchar*)n;}
	void uname(const char*u){this->u=u;}
	void pw(const char*p){this->p=p;}
};

// Substitute chars inside string s
void CSSub(char*s,char a,char b,bool all=true)
{
	for(unsigned i=0;i<strlen(s);++i)
		if(s[i]==a)
		{
			s[i]=b;
			if(!all) break;
		}
}

// Global functions to manage array of St(udents)
void SaveStArray(St*s,int n,const char*fn)
{
	FILE*f=fopen(fn,"w+");
	if(!f){printf("error:can't open %s\n",fn);return;}
	for(int i=0;i<n;++i)
	{
		fputs(s[i].name(),f);fputc('\n',f);
		fputs(s[i].uname(),f);fputc('\n',f);
		fputs(s[i].pw(),f);fputc('\n',f);
	}
	fclose(f);
}

void LoadStArray(St*s,int n,const char*fn)
{
	FILE*f=fopen(fn,"r");
	if(!f){printf("error:can't open %s\n",fn);return;}

	char t[512]={0};
	for(int i=0;i<n&&!feof(f);++i)
	{
		fgets(t,512,f);
		if(*t!='\n') //Empty
		{
			CSSub(t,'\n','\0',false);
			s[i].name(t);
		}

		fgets(t,512,f);
		if(*t!='\n')
		{
			CSSub(t,'\n','\0',false);
			s[i].uname(t);
		}

		fgets(t,512,f);
		if(*t!='\n')
		{
			CSSub(t,'\n','\0',false);
			s[i].pw(t);
		}
	}
	fclose(f);
}
