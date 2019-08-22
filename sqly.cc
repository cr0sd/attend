#pragma once
#include<sqlite3.h>

class Db
{
	sqlite3*db;
	//this db wants SCHEMA: id,name,date,time,type
public:
	// Database management
	void open(const char*dbfname);	// Open from dbfname (used at startup)
	void save(const char*dbfname);	// Save to dbfname (used in a "save old, create new" scenario)
	void createTable();				// Use schema: id,name,date,time,type

	// SQL queries, inserts, updates, deletions
	void readExecSqlFile(const char*fname); // Use custom SQL script
	void insertSignIn(const char*name,time_t date,time_t time);
	void insertSignout(const char*name,time_t date,time_t time);
	char*selectWhereName(const char*name);
	char*selectAll();
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
