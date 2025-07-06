
#include <opencv4/opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Point> createTriangle(int x, int y, int angle, int l) {
    std::vector<cv::Point> points;
    
    points.push_back(cv::Point(x, y));
    points.push_back(cv::Point(x + std::cos(angle / 180.0 * M_PI) * l, 
    y + std::sin(angle / 180.0 * M_PI) * l));
    points.push_back(cv::Point(x + l * cos((60 + angle) / 180.0 * M_PI), 
    y + l * sin((60 + angle) / 180.0 * M_PI)));
    
    return points;
    
}

int main() {
    int id = 2;
    
    cv::VideoCapture cap;
    
    cap.open(id);
    
    cap.set(cv::CAP_PROP_FPS, 30);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 720);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 960);
    
    cv::Mat frame1;
    cv::Mat frame2;
    cv::Mat gray;
    cv::Mat hsv;
    
    cv::namedWindow("1", cv::WINDOW_NORMAL);
    cv::namedWindow("2" , cv::WINDOW_NORMAL);
    
    int key = -1;
    
    while(key != 27) {
        key = cv::waitKey(1000 / 30);
        cap >> frame1;
        
        // cv::GaussianBlur(frame1, frame2, cv::Size(7, 7), 1, 1);
        // cv::medianBlur(frame1, frame2, 5);
        
        // cv::cvtColor(frame2, gray, cv::COLOR_BGR2GRAY);
        
        cv::cvtColor(frame1, hsv, cv::COLOR_BGR2HSV);
        
        cv::inRange(hsv, cv::Scalar(35, 10, 10), cv::Scalar(90, 250, 250), frame2);
        
        // cv::threshold(gray, frame2, 150, 255, cv::THRESH_BINARY);
        
        cv::Moments m = cv::moments(frame2);
        double k = m.m00;
        
        if(k == 0) continue;
        
        double xC = m.m01 / k;
        double yC = m.m10 / k;
        
        cv::circle(frame1, cv::Point(yC, xC), 50, cv::Scalar(0, 0, 255), 10);
        
        
        cv::imshow("1", frame1);
        
        cv::imshow("2", frame2);
    }
    
    cv::destroyAllWindows();
    
    cap.release();
    
    return 0;
}