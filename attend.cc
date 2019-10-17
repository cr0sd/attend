#include<fox-1.6/fx.h>
#include<sqlite3.h>

#define PROGRAMTITLE "Attend"
#define COMPILETIME __DATE__ ", " __TIME__

#include"foxy.cc"
#include"sqly.cc"

#include<time.h>
#include<stdio.h>

void printTime()
{
	char t[512];
	struct tm *tt;
	time_t ttt=time(NULL);
	tt=localtime(&ttt);
	strftime(t,512,"Current time: %F %T",tt);
	puts(t);
}

// entry point
int main(int argc,char**argv)
{
	printTime();
	// Everything is done in mywin
	FXApp a(PROGRAMTITLE,PROGRAMTITLE);
	a.init(argc,argv);
	new mywin(&a);
	a.create();
	a.run();
}
