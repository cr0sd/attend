#pragma once
#include<fox-1.6/fx.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ST FXString
#define ST_TOCS text

#define ENC_KEY 0xFE

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

	void RegisterAcct(St*s,int n,const char*name,
		const char*uname,const char*pw);
	St *FindSt(St*s,int n,const char*name);
};

// Encrypt c_str and return heap encrypted version
char*encrypt(const char*s,char key)
{
	int strl=strlen(s)+1;
	char*r=NULL;

	// Edge case: empty string, set to empty string and return
	if(strl==1)
	{
		r=new char[1]();
		return r;
	}
	// Normal case: encrypt and return
	else
	{
		r=new char[strl];
		for(int i=0;i<strl;++i)
			r[i]=s[i]^key;
		return r;
	}
}

// Decrypt encrypted c_str and return heap decrypted version
char*decrypt(const char*s,char key)
{
	int strl=strlen(s)+1;
	char*r=NULL;

	// Edge case: empty string, set to empty string and return
	if(strl==1)
	{
		r=new char[1]();
		return r;
	}
	// Normal case: encrypt and return
	else
	{
		r=new char[strl];
		for(int i=0;i<strl;++i)
			r[i]=s[i]^key;
		return r;
	}
}

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
// Save St array to file fn
void SaveStArray(St*s,int n,const char*fn)
{
	FILE*f=fopen(fn,"wb+");
	if(!f){printf("error:can't open '%s'\n",fn);return;}
	for(int i=0;i<n;++i)
	{
		fputs(s[i].name(),f);fputc('\n',f);

		//encrypt pw before saving
		char*encryptedPw=encrypt(s[i].uname(),ENC_KEY);
		int siz=strlen(encryptedPw);
		fwrite(&siz,4,1,f);
		fputs(encryptedPw,f);fputc('\n',f);
		delete[] encryptedPw;

		fputs(s[i].pw(),f);fputc('\n',f);
	}
	fclose(f);
}


// Load St array from file fn
void LoadStArray(St*s,int n,const char*fn)
{
	FILE*f=fopen(fn,"rb+");
	if(!f){printf("error:can't open '%s'\n",fn);return;}

	char t[512]={0};
	for(int i=0;i<n&&!feof(f);++i)
	{
		fgets(t,512,f);
		CSSub(t,'\n','\0',false);
		s[i].name(t);

		fgets(t,512,f);
		CSSub(t,'\n','\0',false);
		s[i].uname(t);

		//encrypt pw before saving
		int siz;
		fread(&siz,4,1,f);
		fread(t,1,siz,f);
		char*decryptedPw=decrypt(t,ENC_KEY);
		s[i].pw(decryptedPw);
		delete[] decryptedPw;

		fgets(t,512,f);
		CSSub(t,'\n','\0',false);
		s[i].pw(t);
	}
	fclose(f);
}

// Return St* where name matches name param
St *St::FindSt(St*s,int n,const char*name)
{
	for(int i=0;i<n;++i)
		if(strcmp(s[i].name(),name)==0)
			return s+i;
	return NULL;
}

// Append uname and pw data to student with name
void St::RegisterAcct(St*s,int n,const char*name,
	const char*uname,const char*pw)
{
	St*user=FindSt(s,n,name);

	// Validate input
	if(!user)
	{
		printf("error:didn't find user with name '%s'\n",name);
		return;
	}

	user->uname(uname);
	user->pw(pw);
}
