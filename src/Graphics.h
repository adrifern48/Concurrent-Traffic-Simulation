#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>

#include "TrafficObject.h"

class Graphics
{
public:
    // getters / setters
    void setBgFilename(std::string filename) { _bgFilename = filename; }
    void setTrafficObjects(std::vector<std::shared_ptr<TrafficObject>> &trafficObjects) { _trafficObjects = trafficObjects; };

    // typical behavior methods
    void simulate();

private:
    // typical behavior methods
    void loadBackgroundImg();
    void drawTrafficObjects();

    // member variables
    std::vector<std::shared_ptr<TrafficObject>> _trafficObjects;
    std::string _bgFilename;
    std::string _windowName;
    std::vector<cv::Mat> _images;
};

#endif