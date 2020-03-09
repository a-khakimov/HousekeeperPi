#ifndef IMGDIFF_H
#define IMGDIFF_H

#include "imgsource.h"
#include "timercpp.h"
#include <functional>
#include <queue>

class ImgDiff
{
public:
    ImgDiff();
    virtual ~ImgDiff();
    void run(int ms, std::function<void (double, std::string)> f);

private:
    ImgSource isrc;
    Timer timer;
    std::queue<cv::Mat> images_queue;
};

#endif // IMGDIFF_H
