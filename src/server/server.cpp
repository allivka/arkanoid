#include "server.hpp"

int main(int argc, char *argv[]) {
    
    int cameraId = 0;
    if(argc == 2) cameraId = atoi(argv[1]);
    
    ProcessingContext context;
    context.loadFromFile("data/server/config.txt");
    
    connectEsp();
    
    cv::VideoCapture cap;
    cap.open(cameraId);
    CapContext c = setupCap(cap, "data/configApp, config.txt");
    
    int key = -1;
    
    cv::Mat frame, pf;
    
    cv::namedWindow("display", cv::WINDOW_NORMAL);
    
    Robot robot(c.width);
    
    while(key != 27) {
        key = cv::waitKey(1000 / c.fps);
        cap >> frame;
        
        pf = frame.clone();
        
        try {
        
        Targets points = processFrame(pf, context);
        
        if(!points.ballPos) {
            // int speed = robot.pdRegulatorX(points.robotPos.value(), cv::Point(context.robot.zone.width / 2 + context.robot.zone.x, 0)) / 4;
            int speed = 0;
            Command com;
            com.isKick = 0;
            com.direction = (bool)(speed < 0);
            com.setIntensity((uint8_t)std::abs(speed));
            
            sendCom(com);
            
            cv::circle(frame, points.robotPos.value(), 50, cv::Scalar(150, 0, 0), 5);
            cv::imshow("display", frame);
            std::cout << "No ball found!\n";
            continue;
        }
        
        cv::circle(frame, points.ballPos.value(), 50, cv::Scalar(0, 150, 0), 5);
        cv::circle(frame, points.robotPos.value(), 50, cv::Scalar(150, 0, 0), 5);
        
        int speed = robot.pdRegulatorX(points.robotPos.value(), points.ballPos.value()) / 4;
        
        std::cout << points.robotPos.value() << '\t' << points.ballPos.value() << '\t' << speed << '\n';

        Command com;
        com.isKick = 0;
        com.direction = (bool)(speed > 0);
        com.setIntensity((uint8_t)std::abs(speed));
        
        if(std::abs(points.ballPos.value().y - points.robotPos.value().y) <= 100 && std::abs(points.ballPos.value().x - points.robotPos.value().x) <= 100) {
            com.isKick = 1;
        }
        
        if((points.robotPos.value().x > context.robot.zone.x + context.robot.zone.width - 40 && !com.direction) || (points.robotPos.value().x < context.robot.zone.x + 40 && com.direction)) {
            com.setIntensity(0);
        }
        
        sendCom(com);
        
        } catch(const std::bad_optional_access& err) {
            std::cout << "Error: Failed receiveing object position(maybe no object of requested color found):\t" << err.what() << '\n';
        }
        
        cv::imshow("display", frame);
        
    }
    
    close(Esp32UDP::socketFd);
    
    exit(EXIT_SUCCESS);
    
}
