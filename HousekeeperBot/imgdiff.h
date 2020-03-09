#ifndef IMGDIFF_H
#define IMGDIFF_H

#include "imgsource.h"
#include "timercpp.h"
#include <functional>
#include <queue>

class ImgDiffFinder
{
public:
    ImgDiffFinder();
    virtual ~ImgDiffFinder();
    void run(int ms, std::function<void (double, std::string)>);

private:
    Timer timer;
    ImgSource isrc;
    std::queue<cv::Mat> images_queue;
};

#endif // IMGDIFF_H
