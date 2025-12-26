#ifndef VISION_H
#define VISION_H

#include <opencv2/opencv.hpp>
#include <vector>

// One detected red dot
struct Dot {
    int id;
    int x;
    int y;
    std::string pass;
    std::vector<cv::Point> contour;
};

class Vision {
public:
    std::vector<Dot> detect(const cv::Mat& image);
    void drawAndSave(cv::Mat& image,
                     const std::vector<Dot>& dots,
                     const std::string& filename);
};

#endif
