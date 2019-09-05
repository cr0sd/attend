#pragma once
#include<FOX-1.6/fx.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<string> // std::string

#define ST FXString
#define ST_TOCS text

class St
{
	ST n;
	ST u;
	ST p;
public:
	const char*name(){return n.ST_TOCS();}
	const char*uname(){return u.ST_TOCS();}
	const char*pw(){return p.ST_TOCS();}

	void name(const char*n){this->n=n;}
	void uname(const char*u){this->u=u;}
	void pw(const char*p){this->p=p;}
};

void SaveStArray(St*s,int n,const char*fn)
{
	FILE*f=fopen(fn,"wb");
	if(!f){printf("error:can't open %s\n",fn);return;}
	for(int i=0;i<n;++i)
	{
		fputs(s[i].name(),f);fputc('\n',f);
		fputs(s[i].uname(),f);fputc('\n',f);
		fputs(s[i].pw(),f);fputc('\n',f);
	}
	fclose(f);
}

int main()
{
	// Read and write an array of St(udents).
	St s[10];
	s[0].name("Johnny");
	s[0].uname("jboy12");
	s[0].pw("234dhjk8");
	s[1].name("Sarah");
	SaveStArray(s,10,"file.txt");
}
