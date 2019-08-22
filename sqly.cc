#pragma once
#include<sqlite3.h>

class Db
{
	sqlite3*db;
	//this db wants SCHEMA: id,name,date,time,type
public:
	void open(const char*dbfname);
	void readExecSqlFile(const char*fname);
	void signIn(const char*name);
	char* getAll();
};

void Db::open(const char*dbfname)
{
	int r=sqlite3_open(dbfname,&db);
	if(r)
	{
		printf("error:can't open database '%s'\n",dbfname);
		return;
	}
}

void Db::readExecSqlFile(const char*fname)
{
	FILE*f=fopen(fname,"r");
	if(!f){printf("error:can't open file '%s' (does not exist or corrupted)\n",fname);return;}
	
	fclose(f);
}
