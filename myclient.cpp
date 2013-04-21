#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

#include "Shell.cpp"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr; // server address
    struct hostent *server; // server host entries .
    char *buffer;
    buffer=new char[MAX];
    if (argc < 3)
    {
       fprintf(stderr,"usage %s <HostName> <Port>\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // set socket properties
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); // all bits serv_addr would be 0
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }
    while(1)
    {
        //printf("Enter Command : ");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX-1);
        if (n < 0)
        {
            error("ERROR reading from socket");
        }
        printf("%s",buffer);
        bzero(buffer,MAX);
        fgets(buffer,MAX-1,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
        {
            error("ERROR writing to socket");
        }
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX-1);
        if (n < 0)
        {
            error("ERROR reading from socket");
        }
        printf("%s",buffer);
        if( !strcmp(buffer,"quit") )
        {
            printf("\n");
            close(sockfd);
            break;
        }
        // infinite loop
        // only quits when "quit" is called .
    }
    return 0;
}