#pragma once
#include<sqlite3.h>
#include<stdio.h>
#include<assert.h>

class Db
{
	sqlite3*db;
	//this db wants SCHEMA: id,name,date,time,type
public:
	// Database management
	Db();

	void open(const char*dbfname);	// Open from dbfname (used at startup)
	void save(const char*dbfname);	// Save to dbfname (used in a "save old, create new" scenario)
	void createTable();				// Use schema: id,name,date,time,type

	// SQL queries, inserts, updates, deletions
	void readExecSqlFile(const char*fname); // Use custom SQL script
	void insertSignIn(const char*name);
	void insertSignOut(const char*name);
	void selectWhereName(const char*name);
	void selectAll();
};

Db::Db(){db=NULL;}

void Db::open(const char*dbfname)
{
	assert(this->db==NULL);
	int r=sqlite3_open(dbfname,&db);
	if(r)
	{
		printf("error:can't open database '%s'\n",dbfname);
		return;
	}
}


// Global SQLITE3 callback
int sqlite3_print_row(void* data_file,int argc,char**argv,char** /*columns*/)
{
	//FILE*f=fopen("file.csv","w+");
	if(!data_file)
	{
		puts("error:couldn't open CSV file!");
		return 0;
	}
	for(int i=0;i<argc;++i)
		fprintf((FILE*)data_file,"%s,\t",argv[i]);
	fputs("\n",(FILE*)data_file);
	//fclose(f);
	return 0;
}

void Db::insertSignIn(const char*name)
{
	assert(this->db!=NULL);
	puts("Db::insertSignIn");
	char sql[512];
	char*errmsg=NULL;
	sprintf(sql,"insert into attendance"
				"(name,date,time,type)"
				"values('%s',date('now','localtime'),"
				"time('now','localtime'),'SIGNIN');",name);
	printf("executing SQL:\n%s\n",sql);
	sqlite3_exec(db,sql,sqlite3_print_row,NULL,&errmsg);
	if(errmsg)
		puts(errmsg);
	else
		puts("No error message from Sqlite3");
}


void Db::insertSignOut(const char*name)
{
	assert(this->db!=NULL);
	puts("Db::insertSignOut");
	char sql[512];
	char*errmsg=NULL;
	sprintf(sql,"insert into attendance"
				"(name,date,time,type)"
				"values('%s',date('now','localtime'),"
				"time('now','localtime'),'SIGNOUT');",name);
	printf("executing SQL:\n%s\n",sql);
	sqlite3_exec(db,sql,sqlite3_print_row,NULL,&errmsg);
	if(errmsg)
		puts(errmsg);
	else
		puts("No error message from Sqlite3");
}

void Db::readExecSqlFile(const char*fname)
{
	assert(this->db!=NULL);
	FILE*f=fopen(fname,"r");
	if(!f){printf("error:can't open file '%s' (does not exist or corrupted)\n",fname);return;}
	
	fclose(f);
}

void Db::selectAll()
{
	assert(this->db!=NULL);
	char*errmsg=NULL;
	FILE*f=fopen("file.csv","w+");
	sqlite3_exec(db,"SELECT * FROM attendance;",sqlite3_print_row,f,&errmsg);
	fclose(f);
	if(errmsg)
		puts(errmsg);
}
