#include "connection.hpp"
#include "camera.hpp"
#include <cstring>
#include <cstdio>
#include <fstream>

void connectEsp() {
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
    int bytesSent = Esp32UDP::sendCommand(com);
    
    if(bytesSent == -1) {
        perror("Failed sending command to esp32");
        close(Esp32UDP::socketFd);
        exit(EXIT_FAILURE);
    }
    
    printf("Successfully sent %d bytes of data", bytesSent);
}

class objDetContext {
public:

    unsigned int lowHue = 0;
    unsigned int highHue = 179;
    unsigned int lowSat = 0;
    unsigned int highSat = 255;
    unsigned int lowVal = 0;
    unsigned int highVal = 255;
    
    cv::Rect zone;
    
    void print() const {
        std::cout << "\n " << lowHue << ' ' << highHue << ' ' << lowSat << ' ' << highSat << ' ' << lowVal << ' ' << highVal << '\n';
        std::cout << ' ' << zone.x << ' ' << zone.y << ' ' << zone.width << ' ' << zone.height << '\n';
    }
    
    void readFromIfstream(std::ifstream& in) {
        in >> lowHue >> highHue >> lowSat >> highSat >> lowVal >> highVal;
        in >> zone.x >> zone.y >> zone.width >> zone.height;
    }
};

class processingContext {
public:
    
    objDetContext ball;
    objDetContext robot;
    objDetContext enemy;
    
    
    processingContext& loadFromFile(const std::string& path) {
        std::ifstream in(path);
        
        ball.readFromIfstream(in);
        robot.readFromIfstream(in);
        enemy.readFromIfstream(in);
        
        return *this;
    }
    
    void print() {
        ball.print();
        robot.print();
        enemy.print();
    }
    
};