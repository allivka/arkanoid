
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

int lowHue = 0, lowSat = 0, lowVal = 0;
int highHue = 179, highSat = 255, highVal = 255;

cv::Point mousePos;
cv::Point LMBPressPos;
cv::Rect activeRect(cv::Point(0, 0), cv::Point(960, 720));

bool isLMBPressed = false;

cv::Vec3b getMatMedianHSV(cv::Mat mat) {
    
    std::vector<uchar> h;
    std::vector<uchar> s;
    std::vector<uchar> v;
    
    cv::Vec3b buff;
    
    for(int x = 0; x < mat.size[0]; x++) {
        for(int y = 0; y < mat.size[1]; y++) {
            buff = mat.at<cv::Vec3b>(x, y);
            h.push_back(buff[0]);
            s.push_back(buff[1]);
            v.push_back(buff[2]);
        }
    }
    
    std::sort(h.begin(), h.end());
    std::sort(s.begin(), s.end());
    std::sort(v.begin(), v.end());
    
    cv::Vec3b result;
    
    if(h.size() % 2) {
        result[0] = h[((h.size() / 2) + (h.size() / 2 - 1)) / 2];
        result[1] = s[((s.size() / 2) + (s.size() / 2 - 1)) / 2];
        result[2] = v[((v.size() / 2) + (v.size() / 2 - 1)) / 2];
    } else {
        result[0] = h[h.size() / 2];
        result[1] = s[s.size() / 2];
        result[2] = v[v.size() / 2];
    }
    
    return result;
    
}

void createSettingsWindow() {
    cv::namedWindow("settings", cv::WINDOW_AUTOSIZE);
    
    cv::createTrackbar("lowHue", "settings", &lowHue, 179);
    cv::createTrackbar("highHue", "settings", &highHue, 179);
    cv::createTrackbar("lowSat", "settings", &lowSat, 255);
    cv::createTrackbar("highSat", "settings", &highSat, 255);
    cv::createTrackbar("lowVal", "settings", &lowVal, 255);
    cv::createTrackbar("highVal", "settings", &highVal, 255);
}

void mouseCallback(int event, int x, int y, int flags, void *data) {
    
    mousePos = cv::Point(x, y);
    
    switch(event) {
        case cv::EVENT_LBUTTONDOWN:
        isLMBPressed = true;
        LMBPressPos = mousePos;
        break;
        
        case cv::EVENT_LBUTTONUP:
        isLMBPressed = false;
        activeRect = cv::Rect(LMBPressPos, mousePos);
        
        cv::Vec3b hsvVec = getMatMedianHSV((*((cv::Mat*)data))(activeRect));
        std::cout << (int)hsvVec[0] << '\t' << (int)hsvVec[1] << '\t' << (int)hsvVec[2] << '\n';
        
        break;
        
    }
}

int main() {
    int id = 2;
    
    cv::VideoCapture cap;
    
    cap.open(id);
    
    cap.set(cv::CAP_PROP_FPS, 60);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 960);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    
    cv::Mat frame1;
    cv::Mat frame2;
    cv::Mat frame3;
    cv::Mat gray;
    cv::Mat hsv;
    
    cv::namedWindow("1", cv::WINDOW_NORMAL);
    cv::namedWindow("2" , cv::WINDOW_NORMAL);
    cv::namedWindow("3", cv::WINDOW_NORMAL);
    
    cv::setMouseCallback("1", mouseCallback, &frame1);
    
    cv::moveWindow("1", 100, 100);
    cv::moveWindow("2", 1000, 100);
    cv::moveWindow("3", 100, 500);
    
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
        
        cv::cvtColor(frame2, hsv, cv::COLOR_BGR2HSV);
        
        cv::inRange(hsv, cv::Scalar(lowHue, lowSat, lowVal), cv::Scalar(highHue, highSat, highVal), frame2);
        
        cv::Moments m = cv::moments(frame2);
        double k = m.m00;
        
        if(k == 0) continue;
        
        double xC = m.m10 / k;
        double yC = m.m01 / k;
        
        cv::cvtColor(frame2, frame2, cv::COLOR_GRAY2BGR);
        
        frame3 = frame1 & frame2;
        
        cv::circle(frame1, cv::Point(xC, yC), 50, cv::Scalar(0, 0, 255), 10);
        
        if(isLMBPressed) cv::rectangle(frame1, cv::Rect(LMBPressPos, mousePos), cv::Scalar(100, 0, 0), 10);
        
        
        
        cv::imshow("1", frame1);
        
        cv::imshow("2", frame2);
        
        cv::imshow("3", frame3);
    }
    
    cv::destroyAllWindows();
    
    cap.release();
    
    return 0;
}