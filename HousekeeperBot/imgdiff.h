#ifndef IMGDIFF_H
#define IMGDIFF_H

#include "imgsource.h"
#include "timercpp.h"
#include <functional>
#include <queue>

class ImgDiffFinder
{
    typedef std::function<void (double, std::string)> ImgDiffHandler;
public:
    ImgDiffFinder();
    virtual ~ImgDiffFinder();
    void onImgDiffFinded(int ms, ImgDiffHandler);

private:
    Timer timer;
    ImgSource isrc;
    std::queue<cv::Mat> images_queue;
};

#endif // IMGDIFF_H
