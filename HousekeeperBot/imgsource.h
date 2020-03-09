#ifndef IMGSOURCE_H
#define IMGSOURCE_H

#include <memory>
#include <tuple>
#include <opencv2/core.hpp>
#include "httplib.h"

class ImgSource
{
public:
    ImgSource();
    virtual ~ImgSource();

    bool isAlive();
    std::tuple<cv::Mat, bool> get();

private:
    std::shared_ptr<httplib::Client> client;
};

#endif // IMGSOURCE_H
