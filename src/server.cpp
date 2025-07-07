#include <opencv4/opencv2/opencv.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>

class Command {
public:
    bool isKick = false;
    bool direction = false;
    uint8_t intensity = 0; //values from 0 to 63
    
    Command(const bool& b1, const bool& b2, const uint8_t& b38) : isKick(b1), direction(b2), intensity(b38) {}
    
    uint8_t convertToByte() const {
        uint8_t result = intensity;
        
        result |= (1 * (isKick)) << 7;
        result |= (1 * (isKick)) << 7;
        
        return result;
    }
};

class Esp32UDP {
    public:
    
    Esp32UDP() = delete;
    
    static int socketFd;
    
    static int establishConnection(sockaddr_in thisAddr, sockaddr_in esp32Addr) {
        
        int socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if(socketFd == -1) {
            close(socketFd);
            socketFd = -1;
            return -1;
        }
        
        if(bind(socketFd, (sockaddr*)&thisAddr, sizeof(thisAddr)) == -1) {
            close(socketFd);
            socketFd = -1;
            return - 1;
        }
        
        if(connect(socketFd, (sockaddr*)&esp32Addr, sizeof(esp32Addr)) == 1) {
            close(socketFd);
            socketFd = -1;
            return - 1;
        }
        
        return 0;
    }
    
    static int sendCommand(const Command& com) {
        uint8_t buffer = com.convertToByte();
        
        return send(socketFd, &buffer, sizeof(buffer), 0);
    }
    
};

int Esp32UDP::socketFd = -1;

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
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
    
}
