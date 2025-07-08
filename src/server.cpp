#include <opencv4/opencv2/opencv.hpp>
#include "connection.hpp"

int main() {
    sockaddr_in thisAddr;
    memset(&thisAddr, 0, sizeof(thisAddr));
    thisAddr.sin_family = AF_INET,
    thisAddr.sin_port = htons(80),
    thisAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sockaddr_in espAddr;
    memset(&espAddr, 0, sizeof(espAddr));
    espAddr.sin_family = AF_INET,
    espAddr.sin_port = htons(80),
    espAddr.sin_addr.s_addr = inet_addr("192.168.4.1");
    
    if(Esp32UDP::establishConnection(thisAddr, espAddr)) {
        perror("Failed establishing UDP connection with esp32");
        close(Esp32UDP::socketFd);
        exit(EXIT_FAILURE);
    }
    
    int bytesSent = Esp32UDP::sendCommand(Command(1, 0, 22));
    
    if(bytesSent == -1) {
        perror("Failed sending command to esp32");
        close(Esp32UDP::socketFd);
        exit(EXIT_FAILURE);
    }
    
    printf("Successfully sent %d bytes of data", bytesSent);
    
    close(Esp32UDP::socketFd);
    
    exit(EXIT_SUCCESS);
    
}
