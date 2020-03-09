#ifndef IMGSOURCE_H
#define IMGSOURCE_H

#include <memory>
#include <opencv2/core.hpp>
#include "httplib.h"

class ImgSource
{
public:
    ImgSource();
    virtual ~ImgSource();

    bool isAlive();
    cv::Mat get();


private:
    std::shared_ptr<httplib::Client> client;
};

#endif // IMGSOURCE_H
