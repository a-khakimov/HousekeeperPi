/** 
 *  @file   ImgDiffFinder.cpp
 *  @brief  Get diff for images 
 *  @author a-khakimov 
 ***********************************************/

#include "ImgDiffFinder.h"
#include "plog/Log.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/quality.hpp>

ImgDiffFinder::ImgDiffFinder()
{
    p_camera = nullptr;
    timer = Timer();
}

ImgDiffFinder::ImgDiffFinder(HttpCamera& camera)
{
    timer = Timer();
    p_camera = &camera;
}

ImgDiffFinder::~ImgDiffFinder()
{

}

/*!
  \fn void ImgDiffFinder::onImgDiffFinded(int ms, ImgDiffFinder::ImgDiffHandler handler)
    Blah blah
*/
void ImgDiffFinder::onImgDiffFinded(int ms, ImgDiffFinder::ImgDiffHandler handler)
{
    timer.setInterval([this, &handler]() {
        PLOG_DEBUG << "Timeout";

        auto [ img, isOk ] = p_camera->get();
        if (not isOk) {
            PLOG_ERROR << "PiCameraServer is not avaliable";
            handler(0, "", false);
            return;
        }

        images_queue.push(img);

        if (images_queue.size() == 2) {
            cv::Mat imgA = images_queue.front();
            images_queue.pop();
            cv::Mat imgB = images_queue.front();

            cv::Mat gray_imgA, gray_imgB;
            cv::cvtColor(imgA, gray_imgA, cv::COLOR_BGR2GRAY);
            cv::cvtColor(imgB, gray_imgB, cv::COLOR_BGR2GRAY);

            cv::Scalar mse = cv::quality::QualityMSE::compute(gray_imgA, gray_imgB, cv::noArray());
            PLOG_VERBOSE << "MSE:" << mse;

            if (mse.val[0] >= 50) {
                cv::Mat diffImg;
                cv::absdiff(imgA, imgB, diffImg);
                cv::Mat concatImgAImgB;
                cv::vconcat(imgA, imgB, concatImgAImgB);
                cv::Mat concatImgAImgBDiffImg;
                cv::vconcat(concatImgAImgB, diffImg, concatImgAImgBDiffImg);
                std::string imgPath = "/tmp/diffImg.png";
                cv::imwrite(imgPath, concatImgAImgBDiffImg);
                PLOG_INFO << "Found diffs with mse " << mse << ". Image diff saved (" << imgPath << ")";
                handler(mse.val[0], imgPath, true);
            }
        }
    }, ms);
}

void ImgDiffFinder::setCamera(HttpCamera& camera)
{
    p_camera = &camera;
}
