/*** server5.c 
-- a stream socket server demo**
--Modificado pelo Hae para atender um unico cliente. **
-- compila server5*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <cekeikon.h>

#define PORT "3490"  // the port users will be connecting to#define BACKLOG 1   // how many pending connections queue will hold#define MAXDATASIZE 256 // max number of bytes we can get at once 
#define MAXDATASIZE 256
#define BACKLOG 1

bool testaBytes(BYTE* buf, BYTE b,int n){
    //Testa se n bytes da memoria buf possuem valor b
    bool igual=true;
    for(unsigned i=0; i<n; i++)
        if(buf[i]!=b){ igual=false;break;}
    return igual;
}


class SERVER {
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    char buf[MAXDATASIZE];
    struct addrinfo hints,*servinfo,*p;
    struct sockaddr_storage their_addr; // connector's address information 
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    // get sockaddr, IPv4 or IPv6:
    static void *get_in_addr(struct sockaddr *sa) {
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }

    public:
    // construtor
    SERVER(){
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // use my IP

        if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }

        // loop through all the results and bind to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
            }
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
            }
            if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
            }
            break;
        }
        freeaddrinfo(servinfo); // all done with this structure
        if (p == NULL)  {
            fprintf(stderr, "server: failed to bind\n");
            exit(1);
        }

        if (listen(sockfd, BACKLOG) == -1) {
            perror("listen");
            exit(1);
        }
    }

    // destrutor
    ~SERVER(){
        close(new_fd);
        printf("server: Closed connection.");
    }

    void waitConnection(){
        printf("server: Waiting for connections...\n");
        
        while(1){
            sin_size = sizeof their_addr;
            new_fd = accept(sockfd,(struct sockaddr *)&their_addr,&sin_size);
            
            if(new_fd ==-1){
                perror("accept");
                continue;
            } 
            else break;
        }
        
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s,sizeof s);
                  
        printf("server: got connection from %s\n", s);
        // close(sockfd);// doesn't need the listener anymore
    }

    void sendBytes(int nBytesToSend, BYTE *buf){
        int totalSentBytes = 0;
        while (totalSentBytes < nBytesToSend) {
            int sentBytes = send(new_fd, buf, nBytesToSend, 0);
            if (sentBytes==-1) {
                perror("sendBytes error"); 
                exit(1);
            }
            totalSentBytes += sentBytes;
            buf += sentBytes;
        }
    }

    void receiveBytes(int nBytesToReceive, BYTE *buf) {
        int totalReceivedBytes = 0;
        while (totalReceivedBytes < nBytesToReceive) {
            int receivedBytes = recv(new_fd, buf, nBytesToReceive, 0);
            if (receivedBytes==-1){
                perror("receiveBytes error");
                exit(1);
            }
            totalReceivedBytes += receivedBytes;
            buf += receivedBytes;
        }
    }

    void sendUint(uint32_t m){
        static uint32_t n;
        n=htonl(m);
        sendBytes(4, (BYTE*)&n);
    }

    void receiveUint(uint32_t& m) {
        receiveBytes(4,(BYTE*)&m);
        m=ntohl(m);
    }
};


class CLIENT {
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    // get sockaddr, IPv4 or IPv6:
    void *get_in_addr(struct sockaddr *sa) {
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
        }
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }

    public:
    // construtor
    CLIENT(string endereco){
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if ((rv = getaddrinfo(endereco.c_str(), PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }
        
        // loop through all the results and connect to the first we can
        for(p = servinfo; p !=NULL; p = p->ai_next){
            if((sockfd = socket(p->ai_family, p->ai_socktype,
                                p->ai_protocol))==-1){
                perror("client: socket");
                continue;
            }
            if(connect(sockfd, p->ai_addr, p->ai_addrlen)==-1){
                perror("client: connect");
                close(sockfd);
                continue;
            }
        break;
        
        }
        if(p ==NULL){
            fprintf(stderr,"client: failed to connect\n");
            exit(2);
        }
        
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                  s,sizeof s);
        printf("client: connecting to %s\n", s);
        freeaddrinfo(servinfo);// all done with this structure
    }

    // destrutor
    ~CLIENT(){
        close(sockfd);
        printf("client: Closed connection.");
    }

    void sendBytes(int nBytesToSend, BYTE *buf){
        int totalSentBytes = 0;
        while (totalSentBytes < nBytesToSend) {
            int sentBytes = send(sockfd, buf, nBytesToSend, 0);
            if (sentBytes==-1) {
                perror("sendBytes error"); 
                exit(1);
            }
            totalSentBytes += sentBytes;
            buf += sentBytes;
        }
    }

    void receiveBytes(int nBytesToReceive, BYTE *buf) {
        int totalReceivedBytes = 0;
        while (totalReceivedBytes < nBytesToReceive) {
            int receivedBytes = recv(sockfd, buf, nBytesToReceive, 0);
            if (receivedBytes==-1){
                perror("receiveBytes error");
                exit(1);
            }
            totalReceivedBytes += receivedBytes;
            buf += receivedBytes;
        }
    }
    
    void sendUint(uint32_t m){
        static uint32_t n;
        n=htonl(m);
        sendBytes(4, (BYTE*)&n);
    }

    void receiveUint(uint32_t& m) {
        receiveBytes(4,(BYTE*)&m);
        m=ntohl(m);
    }
};