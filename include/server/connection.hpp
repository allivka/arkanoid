#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>

class Command {
private:
    uint8_t intensity = 0; //values from 0 to 63
    
public:
    
    bool isKick = false;
    bool direction = false;
    
    Command() = default;
    
    Command(const bool& p_isKick, const bool& p_direction, const uint8_t& p_intensity);
    
    Command(const uint8_t& data);
    
    void setIntensity(const uint8_t& p);
    
    uint8_t getIntensity() const;
    
    uint8_t convertToByte() const;
    
};

class Esp32UDP {
    public:
    
    Esp32UDP() = delete;
    
    static int socketFd;
    
    static int establishConnection(sockaddr_in thisAddr, sockaddr_in esp32Addr);
    
    static int sendCommand(const Command& com);
    
};
