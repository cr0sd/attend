#pragma once
#include<fx.h>
#include<sqlite3.h>

#include"sqly.cc"

FXint listSort(FXListItem*a,FXListItem*b);

// define mywin class
class mywin : public FXMainWindow
{
	FXDECLARE(mywin)
	FXButton		*buttonQuit;
	FXButton		*buttonSignin;
	FXButton		*buttonSignout;
	FXMenuBar		*menubar;
	FXMenuPane		*mpFile;
	FXFileDialog	*dialogBox;
	FXChoiceBox		*choice;
	FXList			*list;

	Db db; // sqly

	mywin(){}

public:
	enum
	{
		ID_QUIT=FXMainWindow::ID_LAST,
		ID_SIGNIN,
		ID_SIGNOUT,
		ID_OPEN,
		ID_QUERY
	};

	// FOX-1.6 callbacks
	// (Which may or may not interface
	// with SQL ;) )
	long quit(FXObject*,FXSelector,void*);
	long signin(FXObject*,FXSelector,void*);
	long signout(FXObject*,FXSelector,void*);
	long dialog(FXObject*,FXSelector,void*);
	long query(FXObject*,FXSelector,void*);

	void create();
	mywin(FXApp*a);
	~mywin();
};


// implement mywin class
FXDEFMAP(mywin) mywinMap[]=
{
	FXMAPFUNC(SEL_CLOSE,0,mywin::quit),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_QUIT,mywin::quit),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_SIGNIN,mywin::signin),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_SIGNOUT,mywin::signout),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_OPEN,mywin::dialog),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_QUERY,mywin::query)
};

FXIMPLEMENT(mywin,FXMainWindow,mywinMap,ARRAYNUMBER(mywinMap))

// mywin  CLASS CONSTRUCTOR!!
mywin::mywin(FXApp*a) : FXMainWindow(a,"Attend")
{
	// Set up GUI stuff -----

	// Menu bar
	dialogBox=new FXFileDialog(this,"Open");
	dialogBox->resize(320,190);
	choice=new FXChoiceBox(this,"Prompt","Are you sure?",NULL,"Yes\nNo");
	choice->resize(320,190);
	
	menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
	mpFile=new FXMenuPane(this);
	new FXMenuTitle(menubar,"&File",NULL,mpFile);
	new FXMenuCommand(mpFile,"&Quit",NULL,this,mywin::ID_QUIT);
	new FXMenuCommand(mpFile,"&Open",NULL,this,mywin::ID_OPEN);
	new FXMenuCommand(mpFile,"&Query",NULL,this,mywin::ID_QUERY);

	// Buttons
	buttonQuit=new FXButton (this,"Quit",NULL,this,mywin::ID_QUIT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignin=new FXButton (this,"Sign in",NULL,this,mywin::ID_SIGNIN,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignout=new FXButton (this,"Sign out",NULL,this,mywin::ID_SIGNOUT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);

	// Table
	list=new FXList(this,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y|LIST_SINGLESELECT|FRAME_THICK);
	list->appendItem("John Smith");
	list->appendItem("Sam Adams");
	list->appendItem("Joe Schmoe");
	list->appendItem("Harold Borg");
	list->appendItem("Norm Andersson");
	list->appendItem("Sarah Peters");
	list->appendItem("Joe Schmoe");
	list->appendItem("Jane Jingleheimerschmidt");
	list->appendItem("Andrew Roberts");
	list->appendItem("Bob Roberts");
	list->appendItem("Norm Andersson");
	list->appendItem("Frank Franks");
	list->appendItem("Sam Adams");
	list->appendItem("Sarah Peters");
	list->appendItem("Joe Schmoe");
	list->appendItem("Sam Adams");
	list->appendItem("Paul Garrison");
	list->appendItem("Harold Borg");
	list->appendItem("Phil Juares");
	list->appendItem("Pat Patterson");
	list->appendItem("Joe Jibbers");

	list->setSortFunc((FXListSortFunc)listSort);
	list->sortItems();
	list->selectItem(0); // So that the user knows who is SIGNed IN or OUT

	resize(640,480);

	Db db;
	db.open("data.db");
}

mywin::~mywin()
{
	delete buttonQuit;
	delete buttonSignin;
	delete buttonSignout;
	delete menubar;
	delete mpFile;
	delete dialogBox;
}

// Create window
void mywin::create()
{
	FXMainWindow::create();
	show(PLACEMENT_SCREEN);
}

// Quit GUI
long mywin::quit(FXObject*sender,FXSelector s,void*)
{
	puts("Can we finally exit the program now?");
	sender->handle(this,s,NULL);
	getApp()->exit(1);
	return 1;
}

// Sign in button callback
long mywin::signin(FXObject*,FXSelector,void*)
{
	puts("This gem (I mean SIGNIN function) is a placeholder.");
	static FXString msg;
	msg="";
	msg+="Are you sure you want to sign in as:\n";
	msg+=list->getItemText(list->getCurrentItem()).text();
	msg+="?";
	choice->ask(this,0,"SIGN IN",msg,NULL,"Yes\nNo");
	return 1;
}

// Sign out button callback
long mywin::signout(FXObject*,FXSelector,void*)
{
	puts("This gem (I mean SIGNOUT function) is a placeholder.");
	static FXString msg;
	msg="";
	msg+="Are you sure you want to sign out as:\n";
	msg+=list->getItemText(list->getCurrentItem()).text();
	msg+="?";
	puts("This gem (I mean SIGNIN function) is a placeholder.");
	choice->ask(this,0,"SIGN OUT",msg,NULL,"Yes\nNo");
	return 1;
}

// Open file dialog
long mywin::dialog(FXObject*,FXSelector,void*)
{
	puts("open dialog");
	dialogBox->show(PLACEMENT_SCREEN);
	//delete m;
	return 1;
}

// Query dialog
long mywin::query(FXObject*,FXSelector,void*)
{
	puts("query");
	static char fn[512];
	tmpnam(fn);
	FILE*f=fopen(fn,"w+");
	printf("opening '%s' to store query...\n",fn);
	if(!f)
	{
		puts("error:couldn't open temporary file");
		return 1;
	}

	puts("created a file, dude");
	fclose(f);
	return 1;
}

// ListSort function
FXint listSort(FXListItem*a,FXListItem*b)
{
	return a->getText()[0]-b->getText()[0];
}
