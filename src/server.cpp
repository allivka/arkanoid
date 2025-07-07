#include <opencv4/opencv2/opencv.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctime>
#include <time.h>
#include <unistd.h>

int main() {
    int socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(socketFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    sockaddr_in clientAddr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("192.168.4.1");
    
    if(connect(socketFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    int bytesSent = 0;
    
    uchar buffer = 1;
    
    while(bytesSent != sizeof(buffer)) {
        bytesSent = send(socketFd, &buffer, sizeof(buffer), MSG_DONTROUTE);
        std::cout << "Attempted to send one byte of data\n";
    }
    
    std::cout << "Successfully sent one byte of data\n";
    
    close(socketFd);
    
    exit(EXIT_SUCCESS);
    
}