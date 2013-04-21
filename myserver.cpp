#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include "Shell.cpp"

#define BACKLOG 10
// BACKLOG is important for concurrency .
// Indicates highest number of client we permit for our server .
// you can define any number you wish 

void error(char* message)
{
    perror(message);
    exit(1);
}

void dostuff( int sockfd,MyShell* myshell,char* buffer )
{
    int n;
    char *buf;
    buf = new char[MAX];
    bzero( buf , MAX );
    n=write( sockfd , buffer , strlen(buffer) );
    if( n<0 )
    {
        error("Error writing to socket");
    }
    bzero( buffer , MAX );
    n=read( sockfd , buf , MAX-1 );
    if( n<0 )
    {
        error("Error reading from client");
    }
    char ch=' ';
    int i,index=0;
    bool functionTaken=false;
    string argument="" , function="" , result="" ;
    while( ch==' ' )
    {
        ch=*(buf+index);
        index++;
    }
    if( ch=='\n' )
    {
        strcpy(buf,"\n");
        n=write( sockfd , buf , strlen(buf) );
        if( n<0 )
        {
            error("Error writing to socket");
        }
        strcpy( buffer,buf );
        return ;
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
        ch=*(buf+index);
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
    cout<<"Function : "<<function<<"\n"<<"Argument : "<<argument<<endl;
    result+=myshell->Execute(function,argument);
    if( result=="" )
    {
        result+="\n";
    }
    cout<<"Result : "<<result;    
    sleep(5);
    if( myshell->isQuit() )
    {
        strcpy(buf,"quit");
    }
    else
    {
        buf=(char*)result.c_str();
    }
    n=write( sockfd , buf , strlen(buf) );
    if( n<0 )
    {
        error("Error writing to socket");
    }
    strcpy( buffer , buf );
    return ;
}

int main(int argc , char* argv[])
{
    int sockfd , portno , client_len , newsockfd , pid ;
    struct sockaddr_in server_addr , client_addr ;
    string quit="";
    MyShell *myshell=new MyShell();
    if(argc<2)
    {
        fprintf(stderr,"Usage %s <port-no>\n",argv[0]);
        exit(0);
    }
    portno=atoi(argv[1]);
    sockfd=socket(AF_INET , SOCK_STREAM , 0);
    if( sockfd<0 )
    {
        error("Error opening socket");
    }
    bzero( (char*)&server_addr , 0 );
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(portno);
    if( bind(sockfd , (struct sockaddr *)&server_addr , sizeof(server_addr) )<0 )
    {
        error("Error on binding");
    }
    listen(sockfd, BACKLOG);
    signal(SIGCHLD,SIG_IGN);
    client_len=sizeof( client_addr );
    while(1)
    {
        printf("Server is listening ... \n");
        newsockfd=accept(sockfd , (struct sockaddr *)&client_addr , ( socklen_t *)&client_len);
        if( newsockfd<0 )
        {
            error("Error on accept");
        }
        pid=fork();
        // fork is important here .
        // we are handling multiple client here ...
        // child will process the client ...
        // parent will just ignore current client and will wait for new client to arrive ...
        if(pid==0)// child
        {
            char *buffer=new char[MAX] , *current=new char[MAX];
            char defaultString[MAX]="\n\n#####\n\nType \"list-command\" to acquire list of command you are allowed to run .\n\n#####\n\nremote-user@MyServer : ";
            close(sockfd);
            while(1)
            {
                current=getcwd(buffer,MAX);
                strcat(defaultString,current);
                strcat(defaultString," : ");
                strcpy(buffer,defaultString);
                dostuff(newsockfd,myshell,buffer);
                if( !strcmp(buffer,"quit") )
                {
                    break;
                }
                strcpy(defaultString,"remote-user@MyServer : ");
            }
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    }
    return 0;
}