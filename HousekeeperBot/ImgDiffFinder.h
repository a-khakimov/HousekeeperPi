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
private:
    Timer               _timer;
    HttpCamera          _camera;
    std::queue<cv::Mat> _images_queue;

    typedef std::function <void ( const std::string&, const bool )> ImgDiffHandler;

    inline cv::Scalar makeMse       ( const cv::Mat&, const cv::Mat&                 );
    inline cv::Mat makeDiffImg      ( const cv::Mat&, const cv::Mat&                 );
    cv::Mat makeConcatImg    ( cv::Mat, cv::Mat, cv::Mat );
    inline std::string makeDiffInfo ( const std::string&, const cv::Scalar&          );
    inline bool differenceIsLarge   ( const cv::Scalar&                              );

public:
    explicit ImgDiffFinder ( HttpCamera&            );
    virtual ~ImgDiffFinder (                        );
    void onImgDiffFinded   ( int ms, ImgDiffHandler );
    void setCamera         ( HttpCamera& camera     );
};

#endif // IMGDIFF_H
