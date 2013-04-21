//============================================================================
// Name        : myshell.cpp
// Author      :
// Description : A Command Line Interpreter for in C++
//============================================================================

/*
 * jobs to be done ....
 *  cp – copy a file (cp source target )
 *  del - deletes a file del (del file )
 *  host – prints the hostname of the machine
 *  cd – changes the directory (cd dir1)
 *  clr - clears the screen
 *  dir – lists the contents within a directory
 *  environ - lists the environment settings
 *  echo <comment> - displays the comment followed by a new line
 *  help – displays the help associated with your shell
 *  quit – exits the shell
 */

/*
 * Additional task : We should add a new command "list-command" to list
 * all the commands we allow in our shell . It will be needed when this
 * shell will be accessed through a network .
 * And a "touch" and "pwd" is implemented for testing purpose :-)
 *
 * touch <filename> - creates new file "filename" if doesn't exist or do just nothing
 * pwd - pRint wOrking dIrectory
 */

#include "Shell.cpp"

int main( int argc , char* argv[] )
{
    if( argc>2 )
    {
        cout<<"More than 1 batchfile is not permitted"<<endl;
        return 1;
    }
    else if(argc==2)
    {
        string content="",argument="",function="",result="";
        ifstream input( (char*)argv[1] );
        if( !input )
        {
            cout<<"Could not read "<<argv[1]<<" ."<<endl;
            return 1;
        }
        while( !input.eof() )
        {
            char ch=(char)input.get();
            if( ch==EOF )
                break;
            content+=ch;
        }
        char *array=(char*)content.c_str() , *token, ch;
        int i,index;
        bool functionTaken=false;
        token=strtok(array,"\n");
        MyShell *myshell=new MyShell();
        while( token!=NULL )
        {
            index=0;
            functionTaken=false;
            ch=' ';
            argument="";function="";result="";
            while( ch==' ' )
            {
                ch=*(token+index);
                index++;
            }
            if( ch=='\0' )
            {
                token=strtok(NULL,"\n");
                continue;
            }
            while( ch!='\0' )
            {
                if( ch!=' ' && functionTaken==false )
                {
                    function+=ch;
                }
                else
                {
                    functionTaken=true;
                }
                if( functionTaken==true )
                {
                    argument+=ch;
                }
                ch=*(token+index);
                index++;
            }
            for( i=0;i<argument.size() ; i++ )
            {
                if( argument[i]!=' ' )
                {
                    break;
                }
            }
            argument.erase(0,i);
            for( i=argument.size()-1;i>=0;i-- )
            {
                if( argument[i]!=' ' )
                {
                    break;
                }
            }
            argument.erase( i+1 , argument.size()-i-1 );
            // end of processing . Now , just execute it .
            result+=myshell->Execute(function,argument);
            cout<<result;
            if( myshell->isQuit() )
            {
                exit(0);
            }
            token=strtok(NULL,"\n");
        }// end of while(token!=NULL)
        return 0;
    }
    // interactive mode
    MyShell *myshell=new MyShell();
    int i;
    bool functionTaken;
    char *current,*buf,ch;
    string argument, function,result;
    
    while(1)
    {
        char defaultString[MAX]="user@MyShell : ";
        functionTaken=false;
        ch=' ';
        argument="";
        function="";
        result="";
        current=getcwd(buf,MAX);
        strcat(defaultString,current);
        cout<<defaultString<<" : ";
        while( ch==' ' )
        {
            ch=getchar();
        }
        if( ch=='\n' )
        {
            continue;
        }
        while( ch!='\n' )
        {
            if( ch!=' ' && functionTaken==false )
            {
                function+=ch;
            }
            else
            {
                functionTaken=true;
            }
            if( functionTaken==true )
            {
                argument+=ch;
            }
            ch=getchar();
        }
        for( i=0;i<argument.size() ; i++ )
        {
            if( argument[i]!=' ' )
            {
                break;
            }
        }
        argument.erase(0,i);
        for( i=argument.size()-1;i>=0;i-- )
        {
            if( argument[i]!=' ' )
            {
                break;
            }
        }
        argument.erase( i+1 , argument.size()-i-1 );
        // end of processing . Now , just execute it .
        result+=myshell->Execute(function,argument);
        cout<<result;
        if( myshell->isQuit() )
        {
            exit(0);
        }
    }
    return 0;
}