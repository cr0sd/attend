#pragma once
#include<fox-1.6/fx.h>
#include"sqly.hh"
#include"dir.hh"
#include"st.hh"
#include<time.h>

extern char LICENSE_STRING;

#if defined(AT_WINDOWS)
#define COMPILESYSTEM "Windows"
#else
#define COMPILESYSTEM "Linux/Mac"
#endif

#define ABOUT_STRING \
"Title: " PROGRAMTITLE "\n" "Time of build: " COMPILETIME \
"\n" "System: " COMPILESYSTEM

#define STFILENAME "st.dat"

FXint listSort(FXListItem*a,FXListItem*b);

// define mywin class
class mywin : public FXMainWindow
{
	FXDECLARE(mywin)

	FXHorizontalFrame	*horizFrame;

	FXButton			*buttonQuit;
	FXButton			*buttonSignin;
	FXButton			*buttonSignout;
	FXMenuBar			*menubar;
	FXMenuPane			*mpFile;
	FXMenuPane			*mpEdit;
	FXMenuPane			*mpSearch;
	FXMenuPane			*mpHelp;
	FXFileDialog		*dialogBox;
	//FXProgressDialog	*progress;
	FXChoiceBox			*choice;
	FXList				*list;
	FXDialogBox			*tabWin;
	FXTable				*tabWinTable;

	FXInputDialog		*pwBox;

	FXMenuTitle			*mbSearch;

	FXMenuCommand		*fiOpen;

	FXMenuCommand		*edStudents;
	FXMenuCommand		*edAdLogin;
	FXMenuCommand		*edAdLogout;

	FXMenuCommand		*quAll;
	FXMenuCommand		*quName;
	FXMenuCommand		*quTardies;
	FXMenuCommand		*quLeave;

	FXLabel				*adminLabel;

	Db *db;		// sqly.hh
	Dir *dir;	// dir.hh
	St *st;		// st.hh

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
		ID_QUERYLEAVES,
		ID_EDITSTUDENTS,
		ID_SAVESTUDENTS,
		ID_ADMINLOGIN,
		ID_ADMINLOGOUT,
		ID_ABOUT
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
	long editStudents(FXObject*,FXSelector,void*);
	void loadStudents();
	long saveStudents(FXObject*,FXSelector,void*);
	long about(FXObject*,FXSelector,void*);


	long adminLogin(FXObject*,FXSelector,void*);
	long adminLogout(FXObject*,FXSelector,void*);
	void syncStData();
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
	FXMAPFUNC(SEL_COMMAND,mywin::ID_QUERYLEAVES,mywin::queryAllLeaveEarlies),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_EDITSTUDENTS,mywin::editStudents),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_SAVESTUDENTS,mywin::saveStudents),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_ADMINLOGIN,mywin::adminLogin),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_ADMINLOGOUT,mywin::adminLogout),
	FXMAPFUNC(SEL_COMMAND,mywin::ID_ABOUT,mywin::mywin::about)
};

FXIMPLEMENT(mywin,FXMainWindow,mywinMap,ARRAYNUMBER(mywinMap))

// mywin  CLASS CONSTRUCTOR!!
mywin::mywin(FXApp*a) : FXMainWindow(a,PROGRAMTITLE)
{

	dir=new Dir;
	db=new Db(dir);
	st=new St[512];
	printf("Working directory:\t%s\n",dir->getWd());
	printf("Data files directory:\t%s\n",dir->getDataDir());

	printf("Config directory:\t%s\n",dir->getWd("config.ini"));
	printf("Database directory:\t%s\n",dir->getDataDir("data.db"));

	printf("CSV file directory:\t%s\n",dir->getDataDir("file.csv"));

	// Set up GUI stuff -----

	// Menu bar
	FXColor pix[2]={0xffffff,0xffffff};
	FXIcon *ico=new FXIcon(getApp(),pix);
	setIcon(ico);

	// Set FXTopWindow (dialog boxes)
	tabWin=new FXDialogBox(this,"Students",DECOR_ALL,0,0,480,480);
	tabWin->setIcon(ico);
	new FXButton(tabWin,"OK",0,tabWin,FXDialogBox::ID_ACCEPT,BUTTON_NORMAL|LAYOUT_FIX_WIDTH,0,0,75,20);
	new FXButton(tabWin,"Cancel",0,tabWin,FXDialogBox::ID_CANCEL,BUTTON_NORMAL|LAYOUT_FIX_WIDTH,0,0,150,20);
	tabWinTable=new FXTable(tabWin,NULL,0,
		LAYOUT_FILL_X|LAYOUT_FILL_Y|TABLE_ROW_RENUMBER|
		TABLE_NO_ROWSELECT|TABLE_NO_COLSELECT);
	tabWinTable->insertColumns(0,3);
	tabWinTable->insertRows(0,50);
	//tabWinTable->setItemText(1,2,"<text>");
	tabWinTable->setColumnText(0,"Name");
	tabWinTable->setColumnText(1,"Username");
	tabWinTable->setColumnText(2,"Password");


	//progress=new FXProgressDialog(this,"Processing","",PROGRESSDIALOG_NORMAL|PROGRESSDIALOG_CANCEL);
	//progress->setBarStyle(PROGRESSBAR_NORMAL|PROGRESSBAR_HORIZONTAL|PROGRESSBAR_PERCENTAGE|LAYOUT_FILL_Y);
	//progress->setTotal(100);
	//progress->setProgress(0);

	dialogBox=new FXFileDialog(this,"Open");
	dialogBox->resize(320,190);
	dialogBox->allowPatternEntry(false);
	dialogBox->setPatternList("SQLite3 Database (*.db)\nAll Files (*)");
	choice=new FXChoiceBox(this,"Prompt","Are you sure?",NULL,"Yes\nNo",LIST_BROWSESELECT);
	choice->resize(320,190);

	pwBox=new FXInputDialog(this,"Admin Dialog","Password",NULL,INPUTDIALOG_PASSWORD);

	// MenuBar
	menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
	mpFile=new FXMenuPane(this);
	mpEdit=new FXMenuPane(this);
	mpSearch=new FXMenuPane(this);
	mpHelp=new FXMenuPane(this);
	new FXMenuTitle(menubar,"&File",NULL,mpFile);
	new FXMenuTitle(menubar,"&Edit",NULL,mpEdit);
	mbSearch=new FXMenuTitle(menubar,"&Search",NULL,mpSearch);
	new FXMenuTitle(menubar,"&Help",NULL,mpHelp,LAYOUT_RIGHT);

	fiOpen=new FXMenuCommand(mpFile,"&Open",NULL,this,mywin::ID_OPEN);
	new FXMenuCommand(mpFile,"E&xit",NULL,this,mywin::ID_QUIT);
	new FXMenuCommand(mpHelp,"&About",NULL,this,mywin::ID_ABOUT);

	edStudents=new FXMenuCommand(mpEdit,"&Students",NULL,this,mywin::ID_EDITSTUDENTS);
	new FXSeparator(mpEdit);
	edAdLogin=new FXMenuCommand(mpEdit,"Admin &Login",NULL,this,mywin::ID_ADMINLOGIN);
	edAdLogout=new FXMenuCommand(mpEdit,"Admin Log&out",NULL,this,mywin::ID_ADMINLOGOUT);

	quAll=new FXMenuCommand(mpSearch,"&All",NULL,this,mywin::ID_QUERY);
	quName=new FXMenuCommand(mpSearch,"By &Name",NULL,this,mywin::ID_QUERYWHERENAME);
	quTardies=new FXMenuCommand(mpSearch,"&Tardies",NULL,this,mywin::ID_QUERYTARDIES);
	quLeave=new FXMenuCommand(mpSearch,"&Leave Earlies",NULL,this,mywin::ID_QUERYLEAVES);

	// Buttons
	horizFrame=new FXHorizontalFrame(this);
	//buttonQuit=new FXButton (horizFrame,"Quit",NULL,this,mywin::ID_QUIT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignin=new FXButton (horizFrame,"Sign in",NULL,this,mywin::ID_SIGNIN,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignin->setBackColor(0x88dd88);
	buttonSignout=new FXButton (horizFrame,"Sign out",NULL,this,mywin::ID_SIGNOUT,BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,0,0,150);
	buttonSignout->setBackColor(0x88dddd);

	adminLabel=new FXLabel(horizFrame,"-- ADMIN --");
	adminLabel->setTextColor(0xff0000ff);

	// List of students
	list=new FXList(this,NULL,0,
		LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_THICK|LIST_BROWSESELECT);

	loadStudents();
	for(int i=0;i<50;++i)
		if(!st[i].n.empty())
			list->appendItem(st[i].n),
			tabWinTable->setItemText(i,0,st[i].n);

	//Sort table, set selection to first
	list->setSortFunc((FXListSortFunc)listSort);
	list->sortItems();
	if(list->getNumItems()>0)
	{
		list->setCurrentItem(0);
		list->selectItem(0); // So that the user knows who is SIGNed IN or OUT
	}

	resize(640,480);

	printf("opening '%s'...\n",dir->getDataDir("data.db"));
	db->open(dir->getDataDir("data.db"));

	// Disable admin commands without login
	adminLogout(NULL,0,NULL);
}

mywin::~mywin()
{
	delete db;
	delete dir;
	delete[] st;

	// GUI
	//delete buttonQuit;
	delete buttonSignin;
	delete buttonSignout;
	delete menubar;
	delete mpFile;
	delete mpEdit;
	delete mpSearch;
	delete mpHelp;
	delete dialogBox;
	delete pwBox;
	//delete progress;
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
}

void mywin::syncStData()
{
	// Update tabWinTable and list from St array
	list->clearItems();
	for(int i=0;i<50;++i)
	{
		if(!st[i].n.empty())
		{
			list->appendItem(st[i].n);
			tabWinTable->setItemText(i,0,st[i].n);
		}
		else
			tabWinTable->setItemText(i,0,"");

		if(!st[i].u.empty())
			tabWinTable->setItemText(i,1,st[i].u);
		else
			tabWinTable->setItemText(i,1,"");

		if(!st[i].p.empty())
			tabWinTable->setItemText(i,2,st[i].p);
		else
			tabWinTable->setItemText(i,2,"");
	}
	list->sortItems();
}

long mywin::editStudents(FXObject*,FXSelector,void*)
{
	puts("Editing students...");

	puts("loading st file...");
	LoadStArray(st,50,dir->getDataDir(STFILENAME)); // Update St
	syncStData(); // Update GUI

	if(tabWin->execute(PLACEMENT_SCREEN)==1)
	{
		puts("Saving students file...");
		for(int i=0;i<50;++i)
		{
			st[i].n=tabWinTable->getItemText(i,0);
			st[i].u=tabWinTable->getItemText(i,1);
			st[i].p=tabWinTable->getItemText(i,2);
		}
		SaveStArray(st,50,dir->getDataDir(STFILENAME));
	}
	else
	{
		puts("did not save students file");
		LoadStArray(st,50,dir->getDataDir(STFILENAME));
	}

	syncStData();
	return 1;
}

// Login for admin functions (edit, search)
long mywin::adminLogin(FXObject*,FXSelector,void*)
{
	puts("admin login");

	// Verify password is correct
	pwBox->setText("");
	if(pwBox->execute(PLACEMENT_SCREEN)==1 && (pwBox->getText()=="password"))
	{
			FXMessageBox mb(this,"Login success!","You have logged in as admin",
				NULL,MBOX_OK);
			mb.execute(PLACEMENT_SCREEN);
			puts("login success!");
	}
	else
	{
		FXMessageBox mb(this,"Login failed!","You have NOT logged in as admin",
			NULL,MBOX_OK);
		mb.execute(PLACEMENT_SCREEN);
		puts("login failure");
		return 1;;
	}

	quAll->enable();
	quName->enable();
	quTardies->enable();
	quLeave->enable();
	edStudents->enable();
	fiOpen->enable();

	mbSearch->enable();
	buttonSignin->disable();
	buttonSignout->disable();

	// Disable login, enable logout
	edAdLogout->enable();
	edAdLogin->disable();

	//adminLabel->show();
	adminLabel->setText("-- ADMIN --");

	return 1;
}

long mywin::adminLogout(FXObject*,FXSelector,void*)
{
	puts("admin logout");
	quAll->disable();
	quName->disable();
	quTardies->disable();
	quLeave->disable();
	edStudents->disable();
	fiOpen->disable();

	mbSearch->disable();
	buttonSignin->enable();
	buttonSignout->enable();

	// Enable login, disable logout
	edAdLogout->disable();
	edAdLogin->enable();

	//adminLabel->hide();
	adminLabel->setText("");

	return 1;
}

// Manage students login information
void mywin::loadStudents()
{
	puts("Loading students...");
	LoadStArray(st,50,dir->getDataDir(STFILENAME));
}

long mywin::saveStudents(FXObject*,FXSelector,void*)
{
	puts("Saving students...");
	SaveStArray(st,50,dir->getDataDir(STFILENAME));
	return 1;
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

	if(list->getNumItems()==0)return 1;

	strcpy(name,list->getItemText(list->getCurrentItem()).text());

	// Create prompt message
	msg="";
	msg+="Are you sure you want to sign in as:\n";
	msg+=name;
	msg+="?";

	int res=choice->ask(this,0,"SIGN IN",msg,NULL,"Yes\nNo");
	if(res==0) /*Yes => 0, No => 1*/
		db->insertSignIn((const char*)name);

	return 1;
}

// Sign out button callback
long mywin::signout(FXObject*,FXSelector,void*)
{
	static FXString msg;
	static char name[512];

	if(list->getNumItems()==0)return 1;

	strcpy(name,list->getItemText(list->getCurrentItem()).text());

	// Create prompt message
	msg="";
	msg+="Are you sure you want to sign out as:\n";
	msg+=name;
	msg+="?";

	int res=choice->ask(this,0,"SIGN OUT",msg,NULL,"Yes\nNo");
	if(res==0) /*Yes => 0, No => 1*/
		db->insertSignOut((const char*)name);

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

	//progress->setProgress(0);
	//progress->show(PLACEMENT_SCREEN);
	db->selectAll();

	//progress->setProgress(100);
	//progress->hide();
	return 1;
}

// queryAllByName dialog
long mywin::queryAllByName(FXObject*,FXSelector,void*)
{
	static char name[512];
	strcpy(name,list->getItemText(list->getCurrentItem()).text());

	printf("queryAllByName('%s')\n",name);

	//progress->setProgress(0);
	//progress->show(PLACEMENT_SCREEN);
	db->selectAllWhereName(name);

	//progress->setProgress(100);
	//progress->hide();
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

	//progress->setProgress(0);
	//progress->show(PLACEMENT_SCREEN);
	db->selectAllTardies(sqlTime);

	//progress->setProgress(100);
	//progress->hide();
	return 1;
}

// queryAllLeaveEarlies dialog
long mywin::queryAllLeaveEarlies(FXObject*,FXSelector,void*)
{
	static char sqlTime[512];
	strcpy(sqlTime,"14:25:00");
	//sprintf(sqlTime,"%s",sqlTime);

	printf("queryAllLeaveEarlies('%s')\n",sqlTime);

	//progress->setProgress(0);
	//progress->show(PLACEMENT_SCREEN);
	db->selectAllLeaveEarlies(sqlTime);

	//progress->hide();
	return 1;
}

long mywin::about(FXObject*,FXSelector,void*)
{
	FXDialogBox*x=new FXDialogBox(this,"About",
		DECOR_TITLE|DECOR_BORDER,0,0,280);

	new FXLabel(x,ABOUT_STRING,0,LABEL_NORMAL);

	FXText*t=new FXText(x,0,0,
		TEXT_READONLY|LAYOUT_FILL_X|
		LAYOUT_FIX_HEIGHT,
		0,0,0,100);

	t->setText(&LICENSE_STRING);

	new FXHorizontalSeparator(x);

	new FXButton(x,"OK",0,x,FXDialogBox::ID_ACCEPT,
		BUTTON_NORMAL|LAYOUT_CENTER_X|LAYOUT_FIX_WIDTH,
		0,0,150,20);

	x->execute(PLACEMENT_SCREEN);
	x->hide();
	return 1;
}

// Global listSort function
FXint listSort(FXListItem*a,FXListItem*b)
{
	return a->getText()[0]-b->getText()[0];
}
