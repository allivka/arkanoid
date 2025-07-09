#include "connection.hpp"
#include <opencv4/opencv2/opencv.hpp>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <array>
#include <optional>

#define DEFAULT_PK 1
#define DEFAULT_DK 0.3
#define DEFAULT_SPEED 0
#define DEFAULT_RANGE 252

class Targets{
public:
    std::optional<cv::Point> ballPos;
    std::optional<cv::Point> robotPos;
    std::optional<cv::Point> enemyPos;
};

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

class ObjDetContext {
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

class ProcessingContext {
public:
    
    ObjDetContext ball;
    ObjDetContext robot;
    ObjDetContext enemy;
    
    
    ProcessingContext& loadFromFile(const std::string& path) {
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

class CapContext {
public:
    int fps = 30;
    int width = 960;
    int height = 720;
};

CapContext setupCap(cv::VideoCapture& cap, const::std::string& path) {
    std::ifstream in(path);
    
    CapContext c;
    
    in >> c.fps;
    std::cout << c.fps << '\t';
    
    in >> c.width;
    std::cout << c.width << '\t';
        
    in >> c.height;
    std::cout << c.height << '\t';
    
    cap.set(cv::CAP_PROP_FPS, c.fps);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, c.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, c.height);
    
    return c;
}

cv::Point getObjPos(const cv::Mat& frame, const ObjDetContext& context) {
    static cv::Mat binFrame;
    
    cv::inRange(frame(context.zone), cv::Scalar(context.lowHue, context.lowSat, context.lowVal),
    cv::Scalar(context.highHue, context.highSat, context.highVal), binFrame);
    
    cv::Moments m = cv::moments(binFrame);
    
    double k = m.m00;
    double xC = m.m10 / (k + 0.001);
    double yC = m.m01 / (k + 0.001);
    
    return cv::Point(xC + context.zone.x, yC + context.zone.y);
}

Targets processFrame(cv::Mat frame, const ProcessingContext& context) {
    
    cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0, 0);
    
    cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
    
    std::array<cv::Point, 3> positions;
    
    cv::Point buff;
    Targets result;
    
    buff = getObjPos(frame, context.ball);
    
    if(buff.x == 0 && buff.y == 0) {
        result.ballPos = std::optional<cv::Point>(NULL);
    } else {
        result.ballPos = buff;
    }
    
    buff = getObjPos(frame, context.robot);
    
    if(buff.x == 0 && buff.y == 0) {
        result.robotPos = std::optional<cv::Point>(NULL);
    } else {
        result.robotPos = buff;
    }
    
    buff = getObjPos(frame, context.enemy);
    
    if(buff.x == 0 && buff.y == 0) {
        result.enemyPos = std::optional<cv::Point>(NULL);
    } else {
        result.enemyPos = buff;
    }
    
    return result;
}

template<typename T> T restrict(const T& value, const T& low ,const T& high) {
    if(value < low) return low;
    if(value > high) return high;
    return value;
}

class Robot {
public:
    
    int boardWidth = 960;
    int errold = 0;
    
    Robot(const int& p_w) : boardWidth(p_w) {}
    
    int pdRegulatorX(const cv::Point& currentPos, const cv::Point& targetPos, const double& pK = DEFAULT_PK, const double& dK = DEFAULT_DK, double speed = DEFAULT_SPEED, const double& range = DEFAULT_RANGE) {
        int err = targetPos.x - currentPos.x;
        int u = err * pK + (err - errold) * dK;
        speed += u;
        speed = restrict(speed / (double)boardWidth * range, 0.0, range);
        
        return speed;
    }
};
