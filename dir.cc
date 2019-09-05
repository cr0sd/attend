#pragma once

#include<stdlib.h>
#include<stdio.h>
#define PROGNAME "Attend"

//#define AT_WINDOWS

class Dir
{
public:
	char *dir_wd;		// Working Directory
	char *dir_data;		// Data Directory
	Dir();
	~Dir();
	char *getWd(const char *append);
	char *getDataDir(const char *append);
};

char *Dir::getDataDir(const char *append=NULL)
{
	static char t[1024];
	strcpy(t,dir_data);
	if(append)
	{
		#ifdef AT_WINDOWS
			strcat(t,"\\");
		#else
			strcat(t,"/");
		#endif
			strcat(t,append);
	}
	return t;
}

char *Dir::getWd(const char *append=NULL)
{
	static char t[1024];
	strcpy(t,dir_wd);
	if(append)
	{
		#ifdef AT_WINDOWS
			strcat(t,"\\");
		#else
			strcat(t,"/");
		#endif
			strcat(t,append);
	}
	return t;
}

Dir::Dir()
{
	// Create dir_wd
	char *t=(char*)malloc(1024);
#ifdef AT_WINDOWS
	int i=sprintf(t,"%s",getenv("PROGRAMFILES"));
	sprintf(t+i,"\\%s",PROGNAME);
	dir_wd=t;
#else
	strcpy(t,".");
	dir_wd=t;
#endif

	// Create dir_data
	t=(char*)malloc(1024);
#ifdef AT_WINDOWS
	i=sprintf(t,"%s",getenv("APPDATA"));
	sprintf(t+i,"\\%s",PROGNAME);
	dir_data=t;
#else
	strcpy(t,".");
	dir_data=t;
#endif
}

Dir::~Dir()
{
	if(dir_wd)free(dir_wd);
	if(dir_data)free(dir_data);
}
