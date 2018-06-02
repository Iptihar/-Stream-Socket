#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    char* s1;

    char buffer[256];

    s1 = (char*) malloc(sizeof(char)*64);
    cout << "Enter server host name: ";
    fgets(s1, 64, stdin);
    s1[strlen(s1)-1] = '\0';
    server = gethostbyname(s1);

    cout << "Enter server port number: ";
    cin >> portno;

    cin.ignore();

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    while(true) {

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) {
            cerr << "ERROR opening socket" << endl;
            return 0;
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;

        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

        serv_addr.sin_port = htons(portno);

        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
            cerr << "ERROR connecting" << endl;
            return 0;
        }

        printf("Enter a college major: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));

        if (n < 0) {
            cerr << "ERROR writing to socket" << endl;
            return 0;
        }
         
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0) {
            cerr << "ERROR reading from socket" << endl;
            return 0;
        }
         
        printf("%s\n",buffer);

        string msg = buffer;

        if (msg == "You are disconnected from server.") {
            return 0;
        }
    }
    return 0;
}