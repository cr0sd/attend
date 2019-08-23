#include<fox-1.6/fx.h>
#include<sqlite3.h>

#include"foxy.cc"
#include"sqly.cc"

// entry point
int main(int argc,char**argv)
{
	// Everything is done in mywin
	FXApp a("Attend","Attend");
	a.init(argc,argv);
	new mywin(&a);
	a.create();
	a.run();
}
