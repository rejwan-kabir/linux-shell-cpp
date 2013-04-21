#include "Shell.h"

// constructor
MyShell::MyShell()
{
    this->argument="";
    this->function="";
    this->result="";
    this->documentation_directory="";// must be changed for your application . Remember the trailing /
    this->list_file="list_command.txt";
    this->help_file="help.txt";
    this->quit=0;// indicates not to quit .
}

// Implementing private methods
bool MyShell::copyfile( const char src[] , const char dest[] )
{
    std::ifstream input;
    std::ofstream output;
    input.open( src , std::ios::binary );
    output.open( dest , std::ios::binary );
    if(  !input.is_open()  )
    {
        this->result+="Could not copy source file '";
        this->result+=src;
        this->result+="' . The file might not exist or permission denied";
        this->result+="\n";
        return false;
    }
    if( !output.is_open() )
    {
        this->result+="Could not create destination file '";
        this->result+=dest;
        this->result+="' . Permission denied";
        this->result+="\n";
        return false;
    }
    output<<input.rdbuf();
    output.close();
    input.close();
    return true;
}

bool MyShell::cd()
{
    char *newDirectory,*homeDirectory;
    if( this->argument=="" ) // only "cd" command is wanted . cd to HOME .
    {
        homeDirectory=getenv("HOME");
        chdir(homeDirectory);
        return true;
    }
    newDirectory=(char*)this->argument.c_str();
    int result=chdir(newDirectory); // looking for the new directory
    if( result==-1 )                // checking if the new directory exists
    {
        this->result+="Could not find directory : ";
        this->result+=this->argument;
        this->result+="\n";
        return false;
    }
    return true;
}

void MyShell::clr()
{
    system("clear");
}

bool MyShell::cp()
{
    string source="" , destination="" ;
    int i,j,size=argument.size();
    for( i=0;i<size;i++ )
    {
        if( this->argument[i]==' ' )
        {
            break;
        }
        source+=this->argument[i];
    }
    while( this->argument[i]==' ' )
    {
        i++;
    }
    for(j=i;j<size;j++)
    {
        destination+=this->argument[j];
    }
    if( !this->copyfile( (const char*)source.c_str() ,
                    (const char*)destination.c_str() ) )
    {
        return false;
    }
    return true;
}

bool MyShell::del()
{
    if( this->argument=="" )
    {
        this->result+="del: missing operand";
        this->result+="\n";
        this->result+="Try 'help' for more information.";
        this->result+="\n";
        return false;
    }
    if( remove( this->argument.c_str() )==-1 ) // permission denied
    {                                                                                  // or file not found
        this->result+="Could not delete '";
        this->result+=this->argument;
        this->result+="' . File not found or Permission Denied .";
        this->result+="\n";
        return false;
    }
    return true;
}

bool MyShell::dir()
{
    DIR *directoryPointer;
    struct dirent *directoryEntry;
    char *directoryName;
    if( this->argument=="" )
    {
        this->argument+=".";
    }
    directoryName=(char*)this->argument.c_str();
    if( ( directoryPointer=opendir(directoryName) )==NULL )
    {
        this->result+="MyShell: dir:";
        this->result+=directoryName;
        this->result+=": No such file or directory";
        this->result+="\n";
        return false;
    }
    while( (directoryEntry=readdir(directoryPointer) )!=NULL )
    {
        if( !strcmp(directoryEntry->d_name,".") ||
            !strcmp(directoryEntry->d_name,"..") )
        {
            continue;
        }
        this->result+=directoryEntry->d_name;
        this->result+="\n";
    }
    return true;
}

void MyShell::echo()
{
    this->result+=this->argument;
    this->result+="\n";
}

void MyShell::environment()
{
    char **env=environ;
    while( *env )
    {
        this->result+=(*env);
        this->result+="\n";
        env++;
    }
    return ;
}

void MyShell::help()
{
    string help_file_path=this->documentation_directory+this->help_file;
    ifstream input( (char*)help_file_path.c_str() );
    if( !input )
    {
        this->result+="Documentation not found";
        this->result+="\n";
        return ;
    }
    string content="";
    char ch;
    while( !input.eof() )
    {
        ch=(char)input.get();
        if( ch==EOF )
                break;
        content+=ch;
    }
    this->result+=content;
    this->result+="\n";
    return ;
}

void MyShell::host()
{
    char *computer ;
    struct utsname uts ;
    computer = new char[256];
    if( gethostname(computer,256)!=0 || uname(&uts)<0 )
    {
        this->result+="Could not find host information.";
        this->result+="\n";
        return ;
    }
    this->result+="Computer Name : ";
    this->result+=computer;
    this->result+="\n";
    this->result+="System Name : ";
    this->result+=uts.sysname;
    this->result+="\n";
    this->result+="Node Name : ";
    this->result+=uts.nodename;
    this->result+="\n";
    this->result+="Release : ";
    this->result+=uts.release;
    this->result+="\n";
    this->result+="Version : ";
    this->result+=uts.version;
    this->result+="\n";
    this->result+="Machine : ";
    this->result+=uts.machine;
    this->result+="\n";
    return ;
}

void MyShell::list_command()
{
    string list_file_path=this->documentation_directory+this->list_file;
    ifstream input( (char*)list_file_path.c_str() );
    if( !input )
    {
        this->result+="List not found";
        this->result+="\n";
        return ;
    }
    string content="";
    char ch;
    while( !input.eof() )
    {
        ch=(char)input.get();
        if( ch==EOF )
            break;
        content+=ch;
    }
    this->result+=content;
    this->result+="\n";
    return ;
}

void MyShell::touch()
{
    char command[MAX]="touch ";
    strcat( command , (char*)this->argument.c_str() );
    system( command );
}
// End of Implementing private methods

// Implementing public methods
string MyShell::Execute( string function , string argument )
{
    this->argument=argument;
    this->function=function;
    this->result="";
    if( this->function=="cp" ){
        this->cp();
    }else if( this->function=="cd" ){
        this->cd();
    }else if( this->function=="clr" ){
        this->clr();
    }else if( this->function=="del" ){
        this->del();
    }else if( this->function=="dir" ){
        this->dir();
    }else if( this->function=="echo" ){
        this->echo();
    }else if( this->function=="environ" ){
        this->environment();
    }else if( this->function=="help" ){
        this->help();
    }else if( this->function=="host" ){
        this->host();
    }else if( this->function=="list-command" ){
        this->list_command();
    }else if( this->function=="quit" ){
            this->quit=1; // indicate to quit .
    }else if( this->function=="touch" ){
        this->touch();
    }else{
        this->result+="Command not found . Type 'help' for help .\n";
    }
    return this->result;
}

int MyShell::isQuit()
{
    return this->quit;
}
// End of Implementing public methods