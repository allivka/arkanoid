#include "server.hpp"

int main(int argc, char *argv[]) {
    
    int cameraId = 0;
    if(argc == 2) cameraId = atoi(argv[1]);
    
    ProcessingContext context;
    context.loadFromFile("data/server/config.txt");
    
    // connectEsp();
    
    cv::VideoCapture cap;
    cap.open(cameraId);
    CapContext c = setupCap(cap, "data/configApp, config.txt");
    
    int key = -1;
    
    cv::Mat frame, pf;
    
    cv::namedWindow("display", cv::WINDOW_NORMAL);
    
    while(key != 27) {
        key = cv::waitKey(1000 / c.fps);
        cap >> frame;
        
        pf = frame.clone();
        
        std::array<cv::Point, 3> points = processFrame(pf, context);
        
        cv::circle(frame, points.at(0), 50, cv::Scalar(0, 0, 150), 5);
        
        cv::imshow("display", frame);
        
    }
    
    close(Esp32UDP::socketFd);
    
    exit(EXIT_SUCCESS);
    
}
