#pragma once
#include<fox-1.6/fx.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

class St
{
	FXString u; // Username
	FXString n; // Name
	FXString p; // Password
public:
	const char*uname(){return u.text();}
	const char*name(){return n.text();}
	const char*pw(){return p.text();}
};
