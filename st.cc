#pragma once
#include<fox-1.6/fx.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

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
	if(strl==1 || s[0]==0)
	{
		r=new char[1]();
		return r;
	}
	// Normal case: encrypt and return
	else
	{
		r=new char[strl]();
		//printf("THISISPASSWORD!(%s)\n",r);
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
	if(strl==1 || s[0]==0)
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
	// Create file, save each member of s
	FILE*f=fopen(fn,"wb+");
	if(!f){printf("error:can't open '%s'\n",fn);return;}

	for(int i=0;i<n;++i)
	{
		size_t sl;

		// Save name
		sl=strlen(s[i].name());
		fwrite(&sl,sizeof(size_t),1,f);
		fwrite(s[i].name(),1,sl,f);

		// Save uname
		sl=strlen(s[i].uname());
		fwrite(&sl,sizeof(size_t),1,f);
		fwrite(s[i].uname(),1,sl,f);

		// Encrypt, save pw
		char*encryptedPw=encrypt(s[i].pw(),ENC_KEY);
		sl=strlen(s[i].pw());
		printf("PW: %s\nENC_PW: %s\n",s[i].pw(),encryptedPw);
		fwrite(&sl,sizeof(size_t),1,f);
		fwrite(encryptedPw,1,sl,f);
		delete[] encryptedPw;
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

		size_t sl;

		// Read Name
		memset(t,0,512);
		fread(&sl,sizeof(size_t),1,f);
		fread(t,1,sl,f);
		//printf("NAME got '%s'\n",t);
		//CSSub(t,'\n','\0',false); // Convert endlines to null-terms
		s[i].name(t);

		// Read uname
		memset(t,0,512);
		fread(&sl,sizeof(size_t),1,f);
		fread(t,1,sl,f);
		//printf("UNAME got '%s'\n",t);
		//CSSub(t,'\n','\0',false); // Convert endlines to null-terms
		s[i].uname(t);

		// Read, decrypt pw
		memset(t,0,512);
		fread(&sl,sizeof(size_t),1,f);
		fread(t,1,sl,f);
		//printf("PW got '%s'\n",t);
		char*decryptedPw=decrypt(t,ENC_KEY);
		s[i].pw(decryptedPw);
		delete[] decryptedPw;
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
