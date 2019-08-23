#pragma once
#include<fox-1.6/fx.h>
#include"sqly.cc"
#include<time.h>

FXint listSort(FXListItem*a,FXListItem*b);

// define mywin class
class mywin : public FXMainWindow
{
	FXDECLARE(mywin)
	FXButton			*buttonQuit;
	FXButton			*buttonSignin;
	FXButton			*buttonSignout;
	FXMenuBar			*menubar;
	FXMenuPane			*mpFile;
	FXMenuPane			*mpSearch;
	FXFileDialog		*dialogBox;
	FXProgressDialog	*progress;
	FXChoiceBox			*choice;
	FXList				*list;
	FXDialogBox			*tabWin;
	FXTable				*tabWinTable;

	Db db; // sqly

	mywin(){}

public:
	enum
	{
		ID_QUIT=FXMainWindow::ID_LAST,
		ID_SIGNIN,
		ID_SIGNOUT,
		ID_OPEN,
		ID_QUERY,
		ID_QUERYWHERENAME,
		ID_QUERYTARDIES,
		ID_QUERYLEAVES
	};

	// FOX-1.6 callbacks
	// (Which may or may not interface
	// with SQL ;) )
	long quit(FXObject*,FXSelector,void*);
	long signin(FXObject*,FXSelector,void*);
	long signout(FXObject*,FXSelector,void*);
	long dialog(FXObject*,FXSelector,void*);
	long queryAll(FXObject*,FXSelector,void*);
	long queryAllByName(FXObject*,FXSelector,void*);
	long queryAllTardies(FXObject*,FXSelector,void*);
	long queryAllLeaveEarlies(FXObject*,FXSelector,void*);

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
	FXMAPFUNC(SEL_COMMAND,mywin::ID_QUERY,mywin::queryAll),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_QUERYWHERENAME,mywin::queryAllByName),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_QUERYTARDIES,mywin::queryAllTardies),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_QUERYLEAVES,mywin::queryAllLeaveEarlies)
};

FXIMPLEMENT(mywin,FXMainWindow,mywinMap,ARRAYNUMBER(mywinMap))

// mywin  CLASS CONSTRUCTOR!!
mywin::mywin(FXApp*a) : FXMainWindow(a,"Attend")
{
	// Set up GUI stuff -----

	// Menu bar
	FXColor pix[2]={0xffffff,0xffffff};
	FXIcon *ico=new FXIcon(getApp(),pix);
	setIcon(ico);

	// Set FXTopWindow (dialog boxes)
	tabWin=new FXDialogBox(this,"Table Viewer",DECOR_ALL,0,0,480,480);
	tabWin->setIcon(ico);
	tabWinTable=new FXTable(tabWin,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y);
	tabWinTable->insertColumns(0,6);
	tabWinTable->insertRows(0,30);
	tabWinTable->setItemText(1,2,"<text>");

	progress=new FXProgressDialog(this,"Processing","",PROGRESSDIALOG_NORMAL|PROGRESSDIALOG_CANCEL);
	progress->setBarStyle(PROGRESSBAR_NORMAL|PROGRESSBAR_HORIZONTAL|PROGRESSBAR_PERCENTAGE|LAYOUT_FILL_Y);
	progress->setTotal(100);
	progress->setProgress(0);

	dialogBox=new FXFileDialog(this,"Open");
	dialogBox->resize(320,190);
	dialogBox->allowPatternEntry(false);
	dialogBox->setPatternList("SQLite3 Database (*.db)\nAll Files (*)");
	choice=new FXChoiceBox(this,"Prompt","Are you sure?",NULL,"Yes\nNo",LIST_BROWSESELECT);
	choice->resize(320,190);

	// MenuBar
	menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
	mpFile=new FXMenuPane(this);
	mpSearch=new FXMenuPane(this);
	new FXMenuTitle(menubar,"&File",NULL,mpFile);
	new FXMenuTitle(menubar,"&Search",NULL,mpSearch);

	new FXMenuCommand(mpSearch,"Q&uery",NULL,this,mywin::ID_QUERY);
	new FXMenuCommand(mpSearch,"QueryBy&Name",NULL,this,mywin::ID_QUERYWHERENAME);
	new FXMenuCommand(mpSearch,"Query&Tardies",NULL,this,mywin::ID_QUERYTARDIES);
	new FXMenuCommand(mpSearch,"Query&LeaveEarlies",NULL,this,mywin::ID_QUERYLEAVES);

	new FXMenuCommand(mpFile,"&Open",NULL,this,mywin::ID_OPEN);
	new FXMenuCommand(mpFile,"E&xit",NULL,this,mywin::ID_QUIT);

	// Buttons
	buttonQuit=new FXButton (this,"Quit",NULL,this,mywin::ID_QUIT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignin=new FXButton (this,"Sign in",NULL,this,mywin::ID_SIGNIN,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignout=new FXButton (this,"Sign out",NULL,this,mywin::ID_SIGNOUT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);

	// List of students
	list=new FXList(this,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_THICK|LIST_BROWSESELECT);
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

	//Sort table, set selection to first
	list->setSortFunc((FXListSortFunc)listSort);
	list->sortItems();
	list->setCurrentItem(0);
	list->selectItem(0); // So that the user knows who is SIGNed IN or OUT

	resize(640,480);

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
	delete progress;
	delete choice;
	delete list;
	delete tabWin;
	//delete tabWinTable;
}

// Create window
void mywin::create()
{
	FXMainWindow::create();
	show(PLACEMENT_SCREEN);
	//tabWin->execute();
}

// Quit GUI
long mywin::quit(FXObject*,FXSelector,void*)
{
	puts("Exiting...");
	getApp()->exit(1);
	return 1;
}

// Sign in button callback
long mywin::signin(FXObject*,FXSelector,void*)
{
	static FXString msg;
	static char name[512];
	strcpy(name,list->getItemText(list->getCurrentItem()).text());

	// Create prompt message
	msg="";
	msg+="Are you sure you want to sign in as:\n";
	msg+=name;
	msg+="?";

	int res=choice->ask(this,0,"SIGN IN",msg,NULL,"Yes\nNo");
	if(res==0) /*Yes => 0, No => 1*/
		db.insertSignIn((const char*)name);

	return 1;
}

// Sign out button callback
long mywin::signout(FXObject*,FXSelector,void*)
{
	static FXString msg;
	static char name[512];
	strcpy(name,list->getItemText(list->getCurrentItem()).text());

	// Create prompt message
	msg="";
	msg+="Are you sure you want to sign out as:\n";
	msg+=name;
	msg+="?";

	int res=choice->ask(this,0,"SIGN OUT",msg,NULL,"Yes\nNo");
	if(res==0) /*Yes => 0, No => 1*/
		db.insertSignOut((const char*)name);

	return 1;
}

// Open file dialog
long mywin::dialog(FXObject*,FXSelector,void*)
{
	puts("open dialog");
	FXuint res=dialogBox->execute(PLACEMENT_SCREEN);
	
	printf("close dialog: '%s'\n",res?(dialogBox->getFilename().text()):"<none>");
	return 1;
}

// queryAll dialog
long mywin::queryAll(FXObject*,FXSelector,void*)
{
	puts("queryAll");

	progress->setProgress(0);
	progress->show(PLACEMENT_SCREEN);
	db.selectAll();

	progress->setProgress(100);
	progress->hide();
	return 1;
}

// queryAllByName dialog
long mywin::queryAllByName(FXObject*,FXSelector,void*)
{
	static char name[512];
	strcpy(name,list->getItemText(list->getCurrentItem()).text());

	printf("queryAllByName('%s')\n",name);

	progress->setProgress(0);
	progress->show(PLACEMENT_SCREEN);
	db.selectAllWhereName(name);

	progress->setProgress(100);
	progress->hide();
	return 1;
}

// queryAllTardies dialog
long mywin::queryAllTardies(FXObject*,FXSelector,void*)
{
	static char sqlTime[512];
	//time_t t=time(0);
	//strftime(sqlTime,512,"%F %T",localtime(&t));
	strcpy(sqlTime,"08:45:00");
	//sprintf(sqlTime,"%s",sqlTime);

	printf("queryAllTardies('%s')\n",sqlTime);

	progress->setProgress(0);
	progress->show(PLACEMENT_SCREEN);
	db.selectAllTardies(sqlTime);

	progress->setProgress(100);
	progress->hide();
	return 1;
}

// queryAllLeaveEarlies dialog
long mywin::queryAllLeaveEarlies(FXObject*,FXSelector,void*)
{
	static char sqlTime[512];
	strcpy(sqlTime,"14:25:00");
	//sprintf(sqlTime,"%s",sqlTime);

	printf("queryAllLeaveEarlies('%s')\n",sqlTime);

	progress->setProgress(0);
	progress->show(PLACEMENT_SCREEN);
	db.selectAllLeaveEarlies(sqlTime);

	progress->hide();
	return 1;
}

// Global listSort function
FXint listSort(FXListItem*a,FXListItem*b)
{
	return a->getText()[0]-b->getText()[0];
}
