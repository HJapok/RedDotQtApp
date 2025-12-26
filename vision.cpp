#include "vision.h"
#include <chrono>
#include <iomanip>
#include <sstream>

// Detect red dots and return their info
std::vector<Dot> Vision::detect(const cv::Mat& image) {
    std::vector<Dot> dots;
    std::string resultLabel;

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

    // Red color ranges in HSV
    cv::Mat mask1, mask2, mask;
    cv::inRange(hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsv, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), mask2);

    cv::bitwise_or(mask1, mask2, mask);

    // Reduce noise
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    int id = 1;
    for (const auto& c : contours) {
        cv::Moments m = cv::moments(c);
        if (m.m00 == 0) continue;

        Dot d;
        d.id = id++;
        d.x = static_cast<int>(m.m10 / m.m00);
        d.y = static_cast<int>(m.m01 / m.m00);
        d.contour = c;
        if (dots.empty()) 
        {
            resultLabel = "FAIL";
        } 
        else 
        {
            resultLabel = "PASS";
        }

        dots.push_back(d);
    }

    return dots;
}

// Draw result and save image with timestamp
void Vision::drawAndSave(cv::Mat& image,
                         const std::vector<Dot>& dots,
                         const std::string& baseFilename) {
    std::string resultLabel;

    if (dots.empty()) {
        resultLabel = "FAIL";
    } else {
        resultLabel = "PASS";
    }

    // Draw each dot contour and label
    for (const auto& d : dots) {
        // Draw contour (green)
        cv::drawContours(image, std::vector<std::vector<cv::Point>>{d.contour}, -1, cv::Scalar(0, 255, 0), 2);

        // Draw ID and coordinates (blue)
        std::string label = "ID:" + std::to_string(d.id) +
                            " (" + std::to_string(d.x) + "," + std::to_string(d.y) + ")";
        cv::putText(image, label, cv::Point(d.x + 5, d.y - 5),
                    cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255, 0, 0), 3);
    }

    // Draw PASS/FAIL label at top-left
    cv::putText(image, resultLabel, cv::Point(10, 30),
                cv::FONT_HERSHEY_SIMPLEX, 1.5,
                (resultLabel == "PASS") ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255),
                2);

    // Generate timestamp for filename
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << baseFilename << "_"
        << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".png";

    std::string filename = oss.str();

    cv::imwrite(filename, image);
}
