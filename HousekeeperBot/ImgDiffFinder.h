/** 
 *  @file   ImgDiffFinder.h
 *  @brief  Get diff for images 
 *  @author a-khakimov 
 ***********************************************/

#ifndef IMGDIFF_H
#define IMGDIFF_H

#include "HttpCamera.h"
#include "timercpp.h"
#include <functional>
#include <queue>
#include <memory>

class ImgDiffFinder
{
    typedef std::function <void (double, std::string, bool)> ImgDiffHandler;
public:
    ImgDiffFinder();
    ImgDiffFinder(HttpCamera&);
    virtual ~ImgDiffFinder();

    void onImgDiffFinded(int ms, ImgDiffHandler);
    void setCamera(HttpCamera& camera);

private:
    Timer timer;
    HttpCamera* p_camera;
    std::queue<cv::Mat> images_queue;
};

#endif // IMGDIFF_H
