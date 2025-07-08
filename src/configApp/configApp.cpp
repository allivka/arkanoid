
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <fstream>

int lowHue = 0, lowSat = 0, lowVal = 0;
int highHue = 179, highSat = 255, highVal = 255;

cv::Point mousePos;
cv::Point LMBPressPos;
cv::Rect activeRect(cv::Point(0, 0), cv::Point(960, 720));

bool isLMBPressed = false;

cv::Vec3b getMatMedianHSV(cv::Mat mat) {
    
    cv::cvtColor(mat, mat, cv::COLOR_BGR2HSV);
    
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
    
    if(h.size() % 2 == 0) {
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
    cv::namedWindow("settings", cv::WINDOW_NORMAL);
    
    cv::createTrackbar("lowHue", "settings", &lowHue, 179);
    cv::createTrackbar("highHue", "settings", &highHue, 179);
    cv::createTrackbar("lowSat", "settings", &lowSat, 255);
    cv::createTrackbar("highSat", "settings", &highSat, 255);
    cv::createTrackbar("lowVal", "settings", &lowVal, 255);
    cv::createTrackbar("highVal", "settings", &highVal, 255);
}

static void mouseCallback(int event, int x, int y, int flags, void *data) {
    
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

uint fps = 30;
uint width = 960;
uint height = 720;

cv::VideoCapture cap;

void initCaptureFromFile(const std::string& path) {
    std::ifstream in(path);
    
    in >> fps;
    std::cout << fps << '\t';
    
    in >> width;
    std::cout << width << '\t';
        
    in >> height;
    std::cout << height << '\t';
    
    cap.set(cv::CAP_PROP_FPS, fps);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    
}

void highlightProcess(const cv::Mat& instrFrame) {
    int key = -1;
    
    cv::Mat frame, drawFrame, hsvFrame, binFrame;
    
    cv::setMouseCallback("1", mouseCallback, &frame);
    
    while(key != 27) {
        key = cv::waitKey(1000 / fps);
        cv::imshow("instruction", instrFrame);
        
        cap >> frame;
        
        cv::GaussianBlur(frame, frame, cv::Size(3, 3), 0, 0);
        drawFrame = frame;
        
        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
        cv::inRange(hsvFrame, cv::Scalar(lowHue, lowSat, lowVal), cv::Scalar(highHue, highSat, highVal), binFrame);
        
        cv::Moments m = cv::moments(binFrame);
        double k = m.m00;
        
        double xC = m.m10 / (k + 0.01);
        double yC = m.m01 / (k + 0.01);
        
        if(isLMBPressed) cv::rectangle(drawFrame, cv::Rect(LMBPressPos, mousePos), cv::Scalar(100, 0, 0), 10);
        cv::circle(drawFrame, cv::Point(xC, yC), 30, cv::Scalar(0, 0, 150), 10);
        
        cv::imshow("1", drawFrame);
        cv::imshow("2", binFrame);
        
    }
    
    
}

void highlightBall() {
    
    cv::Mat instrFrame = cv::Mat::zeros(300, 500, CV_8UC3);
    
    cv::putText(instrFrame, "highlight the ball", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 100), 2);
    
    highlightProcess(instrFrame);
    
    std::ofstream out("data/server/config.txt", std::ios::app);
    
    out << lowHue << ' ' << highHue << ' ' <<lowSat << ' ' << highSat << ' ' << lowVal << ' ' << highVal << '\n';
    
    std::cout << "Saved data for server!!\n";
    
}

void highlightRobot() {
    cv::Mat instrFrame = cv::Mat::zeros(300, 500, CV_8UC3);
    
    cv::putText(instrFrame, "highlight the robot", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 100), 2);
    
    highlightProcess(instrFrame);
    
    std::ofstream out("data/server/config.txt", std::ios::app);
    
    out << lowHue << ' ' << highHue << ' ' <<lowSat << ' ' << highSat << ' ' << lowVal << ' ' << highVal << '\n';
    
    std::cout << "Saved data for server!!\n";
}

void highlightEnemy() {
    cv::Mat instrFrame = cv::Mat::zeros(300, 500, CV_8UC3);
    
    cv::putText(instrFrame, "highlight the enemy", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 100), 2);
    
    highlightProcess(instrFrame);
    
    std::ofstream out("data/server/config.txt", std::ios::app);
    
    out << lowHue << ' ' << highHue << ' ' <<lowSat << ' ' << highSat << ' ' << lowVal << ' ' << highVal << '\n';
    
    std::cout << "Saved data for server!!\n";
}

void highlightBallZone() {
    cv::Mat instrFrame = cv::Mat::zeros(300, 500, CV_8UC3);
    
    cv::putText(instrFrame, "highlight the ball zone", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 100), 2);
    
    highlightProcess(instrFrame);
    
    std::ofstream out("data/server/config.txt", std::ios::app);
    
    out << activeRect.x << ' ' << activeRect.y << ' ' << activeRect.x + activeRect.width << ' ' << activeRect.y + activeRect.height << '\n';
    
    std::cout << "Saved data for server!!\n";
}

void highlightRobotZone() {
    cv::Mat instrFrame = cv::Mat::zeros(300, 500, CV_8UC3);
    
    cv::putText(instrFrame, "highlight the robot zone", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 100), 2);
    
    highlightProcess(instrFrame);
    
    std::ofstream out("data/server/config.txt", std::ios::app);
    
    out << activeRect.x << ' ' << activeRect.y << ' ' << activeRect.x + activeRect.width << ' ' << activeRect.y + activeRect.height << '\n';
    
    std::cout << "Saved data for server!!\n";
}

void highlightEnemyZone() {
    cv::Mat instrFrame = cv::Mat::zeros(300, 500, CV_8UC3);
    
    cv::putText(instrFrame, "highlight the enemy zone", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 100), 2);
    
    highlightProcess(instrFrame);
    
    std::ofstream out("data/server/config.txt", std::ios::app);
    
    out << activeRect.x << ' ' << activeRect.y << ' ' << activeRect.x + activeRect.width << ' ' << activeRect.y + activeRect.height << '\n';
    
    std::cout << "Saved data for server!!\n";
}

int main(int argc, const char *argv[]) {
    int id = 0;
    
    if(argc == 2) id = atoi(argv[1]);
    
    std::ofstream out("data/server/config.txt", std::ios::trunc);
    out.close();
    
    cap.open(id);
    
    initCaptureFromFile("data/configApp/config.txt");
    
    createSettingsWindow();
    cv::namedWindow("instruction", cv::WINDOW_NORMAL);
    
    cv::namedWindow("1", cv::WINDOW_NORMAL);
    cv::namedWindow("2" , cv::WINDOW_NORMAL);
    
    
    highlightBall();
    highlightRobot();
    highlightEnemy();
    
    cv::destroyAllWindows();
    
    cap.release();
    
    std::cout << "Released all resources! Successfully finishing!\n";
    
    return 0;
}