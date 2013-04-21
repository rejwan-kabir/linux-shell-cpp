#ifndef SHELL_H
#define SHELL_H

/* Defining our own Library */

// universal library
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<iostream>
#include<string>

// for Linux Specifically
#include<sys/utsname.h>
#include<unistd.h>
#include<dirent.h>

#define MAX 2500
// MAX must have to be greater than 2350 . Because our environ command
// returns an output of 2350 characters . When running the shell across the
// network , we must have to allocate enough space to our buffer that will be
// passed to the client . For this , we have to use MAX>2350 .

// As MAx is 2500 character long , we are permitted to enter a command of
// most 2499 character long , and a 2499 character long output as well .
// This value should me more than enough for our Application .

using namespace std;

extern char **environ;

class MyShell
{
    private :
        string function ;
        string argument ;
        string documentation_directory;
        string list_file;
        string help_file;
        string result;
        int quit;
        bool copyfile( const char src[] , const char dest[] );
        bool cd(); // method names are self-explanatory
        void clr();
        bool cp();
        bool del();
        bool dir();
        void echo();
        void environment();
        void help();
        void host();
        void list_command();
        void touch();
    public :
        MyShell();
        string Execute( string function , string argument );
        int isQuit(); // returns 1 if quit is called . Need to cut the infinite loop
};

#endif