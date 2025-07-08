#include "server.hpp"

int main(int argc, char *argv[]) {
    
    int cameraId = 0;
    if(argc == 2) cameraId = atoi(argv[1]);
    
    processingContext context;
    context.loadFromFile("data/server/config.txt");
    context.print();
    
    close(Esp32UDP::socketFd);
    
    exit(EXIT_SUCCESS);
    
}
