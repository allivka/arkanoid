
#include <opencv4/opencv2/opencv.hpp>
#include <vector>

int lowHue = 0, lowSat = 0, lowVal = 0;
int highHue = 179, highSat = 255, highVal = 255;

void createSettingsWindow() {
    cv::namedWindow("settings", cv::WINDOW_NORMAL);
    
    cv::createTrackbar("lowHue", "settings", &lowHue, 179);
    cv::createTrackbar("highHue", "settings", &highHue, 179);
    cv::createTrackbar("lowSat", "settings", &lowSat, 255);
    cv::createTrackbar("highSat", "settings", &highSat, 255);
    cv::createTrackbar("lowVal", "settings", &lowVal, 255);
    cv::createTrackbar("highVal", "settings", &highVal, 255);
}

int main() {
    int id = 2;
    
    cv::VideoCapture cap;
    
    cap.open(id);
    
    cap.set(cv::CAP_PROP_FPS, 60);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 720);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 960);
    
    cv::Mat frame1;
    cv::Mat frame2;
    cv::Mat frame3;
    cv::Mat gray;
    cv::Mat hsv;
    
    cv::namedWindow("1", cv::WINDOW_NORMAL);
    cv::namedWindow("2" , cv::WINDOW_NORMAL);
    cv::namedWindow("3", cv::WINDOW_NORMAL);

    
    int key = -1;
    
    while(key != 27) {
        key = cv::waitKey(1000 / 60);
        
        if(key == 115) {
            createSettingsWindow();
        } else if(key == 99) {
            cv::destroyWindow("settings");
        }
        
        cap >> frame1;
        
        cv::GaussianBlur(frame1, frame2, cv::Size(7, 7), 1, 1);
        // cv::medianBlur(frame1, frame2, 5);
        
        // cv::cvtColor(frame2, gray, cv::COLOR_BGR2GRAY);
        
        cv::cvtColor(frame2, hsv, cv::COLOR_BGR2HSV);
        
        cv::inRange(hsv, cv::Scalar(lowHue, lowSat, lowVal), cv::Scalar(highHue, highSat, highVal), frame2);
        
        // cv::threshold(gray, frame2, 150, 255, cv::THRESH_BINARY);
        
        cv::Moments m = cv::moments(frame2);
        double k = m.m00;
        
        if(k == 0) continue;
        
        double xC = m.m10 / k;
        double yC = m.m01 / k;
        
        cv::cvtColor(frame2, frame2, cv::COLOR_GRAY2BGR);
        
        frame3 = frame1 & frame2;
        
        cv::circle(frame1, cv::Point(xC, yC), 50, cv::Scalar(0, 0, 255), 10);
        
        cv::imshow("1", frame1);
        
        cv::imshow("2", frame2);
        
        cv::imshow("3", frame3);
    }
    
    cv::destroyAllWindows();
    
    cap.release();
    
    return 0;
}