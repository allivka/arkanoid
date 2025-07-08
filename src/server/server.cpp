#include "connection.hpp"
#include "camera.hpp"
#include <cstring>
#include <cstdio>
#include <fstream>

void initEsp() {
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
}

void sendCom(const Command& com) {
    int bytesSent = Esp32UDP::sendCommand(Command(1, 0, 22));
    
    if(bytesSent == -1) {
        perror("Failed sending command to esp32");
        close(Esp32UDP::socketFd);
        exit(EXIT_FAILURE);
    }
    
    printf("Successfully sent %d bytes of data", bytesSent);
}

class processingContext {
public:
    uchar lowHue = 0;
    uchar highHue = 179;
    uchar lowSat = 0;
    uchar highSat = 255;
    uchar lowVal = 0;
    uchar highVal = 255;
    
    cv::Rect ballZone;
    cv::Rect robotZone;
    cv::Rect enemyZone;
    
    processingContext& loadFromFile(const std::string& path) {
        std::ifstream in(path);
        
        in >> lowHue >> highHue >> lowSat >> highSat >> lowVal >> highVal;
        in >> ballZone.x >> ballZone.y >> ballZone.width >> ballZone.height;
        in >> robotZone.x >> robotZone.y >> robotZone.width >> robotZone.height;
        in >> enemyZone.x >> enemyZone.y >> enemyZone.width >> enemyZone.height;
        
        return *this;
    }
    
};

int main(int argc, char *argv[]) {
    
    int cameraId = 0;
    if(argc == 2) cameraId = atoi(argv[1]);
    
    
    close(Esp32UDP::socketFd);
    
    exit(EXIT_SUCCESS);
    
}
