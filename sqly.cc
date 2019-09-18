#pragma once
#include<sqlite3.h>
#include<assert.h>
#include"dir.cc"

class Db
{
	sqlite3*db;
	//this db wants SCHEMA: id,name,date,time,type

	Dir *dir;
public:
	// Database management
	Db(Dir*);

	void open(const char*dbfname);	// Open from dbfname (used at startup)
	void save(const char*dbfname);	// Save to dbfname (used in a "save old, create new" scenario)
	void createTable();				// Use schema: id,name,date,time,type

	// SQL queries, inserts, updates, deletions
	void outputCsv(const char*sql,const char*fname);	// Base function for other queries
	void readExecSqlFile(const char*fname);				// Use custom SQL script
	void insertSignIn(const char*name);
	void insertSignOut(const char*name);
	void selectAllWhereName(const char*name);
	void selectAllTardies(const char*sqlTime);
	void selectAllLeaveEarlies(const char*sqlTime);
	void selectAll();
};

Db::Db(Dir *dir=NULL)
{
	db=NULL;
	this->dir=dir;
}


// Global SQLITE3 callback
int sqlite3_print_row(void* data_file,int argc,char**argv,char**)
{
	//for(int i=0;cols[i];++i)
		//fprintf((FILE*)data_file,"%s,\t",cols[i]);
	for(int i=0;i<argc;++i)
		fprintf((FILE*)data_file,"%s,\t",argv[i]);
	fputs("\n",(FILE*)data_file);
	return 0;
}

void Db::open(const char*dbfname)
{
	assert(this->db==NULL);
	int r=sqlite3_open(dbfname,&db);
	if(r)
	{
		printf("error:can't open database '%s'\n",dbfname);
		return;
	}

	char sql[512];
	char*errmsg=NULL;
	strcpy(sql,
		"create table if not exists attendance("
		"id integer primary key autoincrement,"
		"name text,"
		"date date,"
		"time time,"
		"type text"
		");"
		);
	sqlite3_exec(db,sql,sqlite3_print_row,NULL,&errmsg);
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

void Db::outputCsv(const char*sql,const char*fname)
{
	assert(this->db!=NULL);
	printf("SQL statement:\n%s\n",sql);
	char*errmsg=NULL;
	FILE*f=fopen(dir->getDataDir(fname),"w+");
	if(!f){puts("error:couldn't open file");return;}
	sqlite3_exec(db,sql,sqlite3_print_row,f,&errmsg);
	fclose(f);
	if(errmsg) puts(errmsg);


	// Create, execute system call to open file in OS
	char t[1024];
	#ifdef AT_WINDOWS
		strcpy(t,"start ");
		strcat(t,dir->getDataDir(fname));
		strcat(t," &");
		printf("systemcall: '%s'\n",t);
		system(t);
	#else
		strcpy(t,"gio open ");
		strcat(t,dir->getDataDir(fname));
		strcat(t," &");
		printf("systemcall: '%s'\n",t);
		system(t);
	#endif
}

void Db::selectAll()
{
	char sql[512];
	sprintf(sql,"SELECT * FROM attendance;");
	outputCsv(sql,"file.csv");
}

void Db::selectAllWhereName(const char*name)
{
	char sql[512];
	sprintf(sql,"SELECT * FROM attendance WHERE name='%s';\n",name);
	outputCsv(sql,"file.csv");
}

void Db::selectAllTardies(const char*sqlTime)
{
	puts("selectAllTardies");
	char sql[512];
	//sprintf(sql,"SELECT name,count(*) FROM attendance WHERE time>time('%s') AND type='SIGNIN' GROUP BY name ORDER BY name;",sqlTime);
	sprintf(sql,"SELECT * FROM attendance WHERE time>time('%s') AND type='SIGNIN' ORDER BY name;",sqlTime);
	outputCsv(sql,"file.csv");
}

void Db::selectAllLeaveEarlies(const char*sqlTime)
{
	puts("selectAllLeaveEarlies: Needs to be implemented");
	char sql[512];
	//sprintf(sql,"SELECT name,count(*),AVG('%s'-time) FROM attendance "
				//"WHERE time<time('%s') AND type='SIGNOUT' "
				//"GROUP BY name ORDER BY name;",
				//sqlTime,sqlTime);
	sprintf(sql,"SELECT * FROM attendance "
				"WHERE time<time('%s') AND type='SIGNOUT' "
				"ORDER BY name;",
				sqlTime);
	outputCsv(sql,"file.csv");
}
