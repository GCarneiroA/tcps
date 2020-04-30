

// http://www.linuxhowtos.org/C_C++/socket.htm

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <string>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "header.h"
#include "endianness.h"
 
#define MYPORT 3490    // the port users will be connecting to
#define BACKLOG 10    // how many pending connections queue will hold

void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}
<<<<<<< HEAD
=======
 
// Structure to be sent over TCP Socket
/*
struct RTUDATA
{
    unsigned int tagid;
    unsigned char flag;

    char name[20];

    float value;
    time_t time_stamp;
};
*/

struct RTUDATA
{
    uint8_t x;
    uint32_t y;
} __attribute__((packed));

>>>>>>> 1c903e2... server: update structure

int main(void)
{
    int sockfd, new_fd;        // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;    // Sender's address information
    struct sockaddr_in their_addr;    // connector’s address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    int n=0;

    struct RTUDATA rtu;
<<<<<<< HEAD
    rtu.x = 17;
    rtu.y = hton32(2924);
=======

    rtu.x = (uint8_t)17;
    rtu.y = htonl(2924);
>>>>>>> 1c903e2... server: update structure

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    my_addr.sin_family = AF_INET;        //host byte order
    my_addr.sin_port = htons(MYPORT);    //short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY;    //automatically fill with my IP
    memset(&(my_addr.sin_zero),'\0', 8);    //zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1) {
        perror("bind");
        exit(1);
    }
 
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
 
    sa.sa_handler = sigchld_handler; // clear all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(1) {    // main accept() loop
        sin_size = sizeof(struct sockaddr_in);
 
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }
 
        printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));
 
        //printf("Data to be transferred is: %d\n", (int*)sizeof(rtu));
 
        if (!fork()) {        // this is the child process
            close(sockfd);    // child doesn’t need the listener
            /*
            if (send(new_fd, str, 15, 0) == -1)
                perror("send");
            */
<<<<<<< HEAD
=======
           //rtu.time_stamp = time(NULL);
>>>>>>> 1c903e2... server: update structure
           n = sendto(new_fd, &rtu, sizeof(struct RTUDATA), 0, (struct sockaddr *)&their_addr, sizeof(their_addr));
                   
            // n=send(new_fd,(void *) &rtu ,sizeof(rtu), 0);

            
            if ( n== -1)
                perror("send");
            else 
                printf("Total Data sent to %s is: %d \n",inet_ntoa(their_addr.sin_addr), n);
            
            close(new_fd);
            exit(0);
        }    // child process if condition close 
        close(new_fd);    // parent doesn’t need this
    }    // while loop close
 
    return 0;
}



/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>


volatile sig_atomic_t gGracefulShutdown = 0;
volatile sig_atomic_t gCaughtHupSignal = 0;

int gLockFileDesc = -1;
int gMasterSocket = -1;

const int gHelloServerPort = 30153;

const char *const gLockFilePath = "/var/run/helloserver.pid";

int BecomeDaemonProcess(const char *const lockFilename,
        const char *const logPrefix,
        const int logLevel,
        int *const lockFileDesc,
        int *const thisPID);
int ConfigureSignalHandlers(void);
int BindPassiveSocket(const unsigned long interfaceAddress,
        const int portNum,
        int *const boundSocket);

int AcceptConnections(const int master);
int HandleConnection(const int slave);
int writeToSocket(const int sock, const char *const buffer, const size_t buflen);
int ReadLine(const int sock, char *const buffer, const size_t buflen, size_t *const bytesRead);

void FatalSigHandler(int sig);
void TermHandler(int sig);
void HupHandler(int sig);
void Usr1Handler(int sig);
void TidyUp(void);

#define OPEN_MAX_GUESS 256

int main(void )
{
    int result;
    pid_t daemonPID;

    if ((result = BecomeDaemonProcess(gLockFilePath, "helloserver", LOG_DEBUG, &gLockFileDesc, &daemonPID)) < 0) {
        perror("Failed to become daemon process");
        exit(result);
    }

    if ((result = ConfigureSignalHandlers()) < 0) {
        syslog(LOG_LOCAL0 | LOG_INFO, "ConfigureSignalHandlers failed, errno=%d", errno);
        unlink("/var/log/helloserver.pid");
        exit(result);
    }

    if ((result = BindPassiveSocket(INADDR_ANY, gHelloServerPort, &gMasterSocket)) < 0) {
        syslog(LOG_LOCAL0 | LOG_INFO, "BindPassiveSocket failed, errono=%d", errno);
        unlink("/var/log/helloserver.pid");
        exit(result);
    }

    do {
        if (AcceptConnections(gMasterSocket) < 0) {
            syslog(LOG_LOCAL0 | LOG_INFO, "AcceptConnections failed, errno=%d", errno);
            unlink("/var/log/helloserver.pid");
            exit(result);
        }

        if ((gGracefulShutdown == 1) && (gCaughtHupSignal == 0)) {
            break;
        }

        gGracefulShutdown = gCaughtHupSignal = 0;
    } while(1);

    TidyUp();

    return 0;
}


int BecomeDaemonProcess(const char *const lockFilename,
        const char *const logPrefix,
        const int logLevel,
        int *const lockFileDesc,
        int *const thisPID)
{
    int currPID;
    int stdioFD;
    int lockResult;
    int killResult;
    int lockFD;
    int i;
    int numFiles;

    char pidBuf[17];
    char *lfs;
    char pidStr[7];
    
    FILE *lfp;
    
    unsigned long lockPID;
    struct flock exclusiveLock;

    chdir("/");

    lockFD = open(lockFilename, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (lockFD == -1) {
        lfp = fopen(lockFilename, "r");
        if (lfp == 0) {
            perror("Can't get lockfile");
            return -1;
        }
        lfs = fgets(pidBuf, 16, lfp);
        if (lfs != 0) {
            if (pidBuf[strlen(pidBuf) -1] == '\n') {
                pidBuf[strlen(pidBuf) -1] = 0;
            }
            lockPID = strtoul(pidBuf, (char**)0, 10);
            
            killResult = kill(lockPID, 0);
            if (killResult == 0) {
                printf("\n\nERROR\n\nA lock file %s has been detected. It appears it is owned\nby the (active) process with PID %ld.\n\n", lockFilename, lockPID);
            }
            else {
                if (errno == ESRCH) {
                    printf("\n\nERROR\n\nA lock file %s has been detected. It appears it is owned\nby the process with PID %ld, which is now defunct. Delete the lock file\nand try again.\n\n", lockFilename, lockPID);
                }
                else {
                    perror("Could not acquire exclusive lock on lock file");
                }
            }
        }
        else {
            perror("Could not read lock file");
        }
        fclose(lfp);

        return -1;
    }

    exclusiveLock.l_type = F_WRLCK;
    exclusiveLock.l_whence = SEEK_SET;
    exclusiveLock.l_len = exclusiveLock.l_start = 0;
    exclusiveLock.l_pid = 0;
    lockResult = fcntl(lockFD, F_SETLK, &exclusiveLock);

    if (lockResult < 0) {
        close(lockFD);
        perror("Can't get lockfile");
        return -1;
    }

    currPID = fork();
    switch (currPID) {
        case 0:
            break;
        case -1:
            fprintf(stderr, "Error: initial fork failed: %s\n",
                strerror(errno));
            return -1;
            break;
        default:
            exit(0);
            break;
    }

    if (setsid() < 0) {
        return -1;
    }

    signal(SIGHUP, SIG_IGN);

    currPID = fork();
    switch (currPID) {
        case 0:
            break;
        case -1:
            return -1;
            break;
        default:
            exit(0);
            break;
    }

    if (ftruncate(lockFD, 0) < 0) {
        return -1;
    }

    sprintf(pidStr, "%d\n", (int)getpid());

    write(lockFD, pidStr, strlen(pidStr));

    *lockFileDesc = lockFD;

    numFiles = sysconf(_SC_OPEN_MAX);

    if (numFiles < 0) {
        numFiles = OPEN_MAX_GUESS;
    }

    for (i = numFiles -1; i >= 0; --i) {
        if (i != lockFD) {
            close(i);
        }
    }
    
    umask(0);

    stdioFD = open("/dev/null", O_RDWR);
    dup(stdioFD);
    dup(stdioFD);

    openlog(logPrefix, LOG_PID | LOG_CONS | LOG_NDELAY | LOG_NOWAIT, LOG_LOCAL0);

    (void)setlogmask(LOG_UPTO(logLevel));

    setpgrp();

    return 0;
}


int ConfigureSignalHandlers(void)
{
    struct sigaction sighupSA;
    struct sigaction sigusr1SA;
    struct sigaction sigtermSA;

    signal(SIGUSR2,     SIG_IGN);	
	signal(SIGPIPE,     SIG_IGN);
	signal(SIGALRM,     SIG_IGN);
	signal(SIGTSTP,     SIG_IGN);
	signal(SIGTTIN,     SIG_IGN);
	signal(SIGTTOU,     SIG_IGN);
	signal(SIGURG,      SIG_IGN);
	signal(SIGXCPU,     SIG_IGN);
	signal(SIGXFSZ,     SIG_IGN);
	signal(SIGVTALRM,   SIG_IGN);
	signal(SIGPROF,     SIG_IGN);
	signal(SIGIO,       SIG_IGN);
	signal(SIGCHLD,     SIG_IGN);

    signal(SIGQUIT,     FatalSigHandler);
	signal(SIGILL,      FatalSigHandler);
	signal(SIGTRAP,     FatalSigHandler);
	signal(SIGABRT,     FatalSigHandler);
	signal(SIGIOT,      FatalSigHandler);
	signal(SIGBUS,      FatalSigHandler);
#ifdef SIGEMT
	signal(SIGEMT,      FatalSigHandler);
#endif
	signal(SIGFPE,      FatalSigHandler);
	signal(SIGSEGV,     FatalSigHandler);
	signal(SIGSTKFLT,   FatalSigHandler);
	signal(SIGCONT,     FatalSigHandler);
	signal(SIGPWR,      FatalSigHandler);
	signal(SIGSYS,      FatalSigHandler);

    sigtermSA.sa_handler = TermHandler;
    sigemptyset(&sigtermSA.sa_mask);
    sigtermSA.sa_flags = 0;
    sigaction(SIGTERM, &sigtermSA, NULL);

    sigusr1SA.sa_handler = Usr1Handler;
    sigemptyset(&sigusr1SA.sa_mask);
    sigusr1SA.sa_flags = 0;
    sigaction(SIGUSR1, &sigusr1SA, NULL);

    sighupSA.sa_handler = HupHandler;
    sigemptyset(&sighupSA.sa_mask);
    sighupSA.sa_flags = 0;
    sigaction(SIGHUP, &sigusr1SA, NULL);

    return 0;
}

int BindPassiveSocket(const unsigned long interfaceAddress,
        const int portNum,
        int *const boundSocket)
{
    struct sockaddr_in sin;
    struct protoent *proto;
    int newsock;
    int optval;
    size_t optlen;

    if ((proto = getprotobyname("tcp")) == NULL) {
        return -1;
    }

    memset(&sin.sin_zero, 0, 8);

    sin.sin_port = htons(portNum);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(interfaceAddress);

    if ((newsock = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0) {
        return -1;
    }

    optval = 1;
    optlen = sizeof(int);
    setsockopt(newsock, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);

    if (bind(newsock, (struct sockaddr*)&sin, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }

    if (listen(newsock, SOMAXCONN) < 0) {
        return -1;
    }

    *boundSocket = newsock;

    return 0;
}

int AcceptConnections(const int master)
{
    int proceed = 1;
    int slave;
    int retval = 0;
    struct sockaddr_in client;
    socklen_t clilen;

    while ((proceed == 1) && (gGracefulShutdown == 0)) {
        clilen = sizeof(client);
        slave = accept(master, (struct sockaddr *)&client, &clilen);

        if (slave < 0) {
            if (errno == EINTR) continue;

            syslog(LOG_LOCAL0 | LOG_INFO, "accept() failed: %m\n");
            proceed = 0;
            retval = -1;
        }
        else {
            retval = HandleConnection(slave);
            if (retval) {
                proceed = 0;
            }
        }
        close(slave);
    }
    return retval;
}

int HandleConnection(const int slave)
{
    char readbuf[1025];
    size_t bytesRead;
    const size_t buflen = 1024;
    
    int retval = ReadLine(slave, readbuf, buflen, &bytesRead);

    return retval;
}

int writeToSocket(const int sock, const char *const buffer, const size_t buflen)
{
    size_t bytesWritten = 0;
    ssize_t writeResult;
    int retval = 0;
    int done = 0;

    do {
        writeResult = send(sock, buffer + bytesWritten, buflen - bytesWritten, 0);
        if (writeResult == -1) {
            if (errno == EINTR) {
                writeResult = 0;
            }
            else {
                retval = 1;
                done = 1;
            }
        }
        else {
            bytesWritten += writeResult;
            if (writeResult == 0) done = 1;
        }
    } while(done == 0);
    return retval;
}

int ReadLine(const int sock, char *const buffer, const size_t buflen, size_t *const bytesRead)
{
    int done = 0;
    int retval = 0;
    char c;
    char lastC = 0;
    size_t bytesSoFar = 0;
    ssize_t readResult;

    do {
        readResult = recv(sock, &c, 1, 0);
        switch (readResult) {
            case -1:
                if (errno != EINTR) {
                    retval = -1;
                    done = 1;
                }
                break;
            
            case 0:
                retval = 0;
                done = 1;
                break;

            case 1:
                buffer[bytesSoFar] = c;
                bytesSoFar += readResult;
                if (bytesSoFar >= buflen) {
                    done = 1;
                    retval = -1;
                }
                if ((c == '\n') && (lastC == '\r')) {
                    done = 1;
                }
                lastC = c;
                break;
        }
    } while(!done);
    buffer[bytesSoFar] = 0;
    *bytesRead = bytesSoFar;

    return retval;
}

void FatalSigHandler(int sig)
{
#ifdef _GNU_SOURCE
    syslog(LOG_LOCAL0 | LOG_INFO, "caught signal: %s - exiting", strsignal(sig));
#else
    syslog(LOG_LOCAL0 | LOG_INFO, "caught signal: %s - exiting", sig);
#endif
    closelog();
    TidyUp();
    _exit(0);
}

void TermHandler(int sig)
{
    TidyUp();
    _exit(0);
}

void HupHandler(int sig)
{
    syslog(LOG_LOCAL0 | LOG_INFO, "caught SIGHUP");
    gGracefulShutdown = 1;
    gCaughtHupSignal = 1;

    return;
}

void Usr1Handler(int sig)
{
    syslog(LOG_LOCAL0 | LOG_INFO, "caught SIGUSR1 - soft shutdown");
    gGracefulShutdown = 1;

    return;
}

void TidyUp(void)
{
    if (gLockFileDesc != -1) {
        close(gLockFileDesc);
        unlink(gLockFilePath);
        gLockFileDesc = -1;
    }

    if (gMasterSocket != -1) {
        close(gMasterSocket);
        gMasterSocket = -1;
    }
}
*/





/*
int main()
{
    // Create socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening <= 0) {
        std::cerr << "Can't create a socket!";
        return -1;
    }

    // Bind Socket to a IP / Port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }

    close(listening);

    char buf[4096];

    while (true) {
        memset(buf, 0, 4096);

        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }

        if (bytesReceived == 0) {
            std::cout << "Client disconnected " << std::endl;
            break;
        }

        std::cout << std::string(buf, 0, bytesReceived) << std::endl;

        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    close(clientSocket);

    return 0;
}
*/