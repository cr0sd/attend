#pragma once
#include<fx.h>
#include<sqlite3.h>

#include"sqly.cc"

// define mywin class
class mywin : public FXMainWindow
{
	FXDECLARE(mywin)
	FXButton	*buttonQuit;
	FXButton	*buttonSignin;
	FXButton	*buttonSignout;
	FXMenuBar	*menubar;
	FXMenuPane	*mpFile;
	FXFileDialog *dialogBox;

	Db db; // sqly

	mywin(){}

public:
	enum
	{
		ID_QUIT=FXMainWindow::ID_LAST,
		ID_SIGNIN,
		ID_SIGNOUT,
		ID_DIALOG
	};

	// FOX-1.6 callbacks
	long quit(FXObject*,FXSelector,void*);
	long signin(FXObject*,FXSelector,void*);
	long signout(FXObject*,FXSelector,void*);
	long dialog(FXObject*,FXSelector,void*);

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
	FXMAPFUNC(SEL_COMMAND,mywin::ID_DIALOG,mywin::dialog)
};

FXIMPLEMENT(mywin,FXMainWindow,mywinMap,ARRAYNUMBER(mywinMap))

// mywin  CLASS CONSTRUCTOR!!
mywin::mywin(FXApp*a) : FXMainWindow(a,"Attend")
{
	// Set up GUI stuff -----

	// Menu bar
	dialogBox=new FXFileDialog(this,"Open");
	dialogBox->resize(320,190);
	menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
	mpFile=new FXMenuPane(this);
	new FXMenuTitle(menubar,"&File",NULL,mpFile);
	new FXMenuCommand(mpFile,"&Quit",NULL,this,mywin::ID_QUIT);
	new FXMenuCommand(mpFile,"&Open",NULL,this,mywin::ID_DIALOG);

	// Buttons
	buttonQuit=new FXButton (this,"Quit",NULL,this,mywin::ID_QUIT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignin=new FXButton (this,"Sign in",NULL,this,mywin::ID_SIGNIN,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignout=new FXButton (this,"Sign out",NULL,this,mywin::ID_SIGNOUT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);

	// Table
	FXList*t=new FXList(this,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y|LIST_SINGLESELECT);
	t->appendItem("Name of person");
	t->appendItem("Name of other person");

	resize(640,480);
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
	return 1;
}

// Sign out button callback
long mywin::signout(FXObject*,FXSelector,void*)
{
	puts("This gem (I mean SIGNOUT function) is a placeholder.");
	return 1;
}

long mywin::dialog(FXObject*,FXSelector,void*)
{
	puts("open dialog");
	dialogBox->show(PLACEMENT_SCREEN);
	//delete m;
	return 1;
}
