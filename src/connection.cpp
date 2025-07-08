#include "connection.hpp"

    
Command::Command(const bool& p_isKick, const bool& p_direction, const uint8_t& p_intensity) 
: intensity(p_intensity < 63 ? p_intensity : 63), isKick(p_isKick), direction(p_direction) {}

Command::Command(const uint8_t& data) {
    isKick = data & (1 << 7);
    direction = data & (1 << 6);
    intensity = data & (~((1 << 7) | (1 << 6)));
}

void Command::setIntensity(const uint8_t& p) {
    intensity = p < 63 ? p : 63;
}

uint8_t Command::getIntensity() const {
    return intensity;
}

uint8_t Command::convertToByte() const {
    uint8_t result = intensity;
    
    result |= (1 * (isKick)) << 7;
    result |= (1 * (direction)) << 6;
    
    return result;
}

    
int Esp32UDP::establishConnection(sockaddr_in thisAddr, sockaddr_in esp32Addr) {
    
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(sock == -1) {
        return -1;
    }
    
    if(bind(sock, (sockaddr*)&thisAddr, sizeof(thisAddr)) == -1) {
        close(sock);
        return - 1;
    }
    
    if(connect(sock, (sockaddr*)&esp32Addr, sizeof(esp32Addr)) == -1) {
        close(sock);
        return - 1;
    }
    
    socketFd = sock;
    
    return 0;
}

int Esp32UDP::sendCommand(const Command& com) {
    uint8_t buffer = com.convertToByte();
    
    return send(socketFd, &buffer, sizeof(buffer), 0);
}

int Esp32UDP::socketFd = -1;
