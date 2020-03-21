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
#include <string>

ImgDiffFinder::ImgDiffFinder(HttpCamera& camera) : _camera(camera)
{
    _timer = Timer();
}

ImgDiffFinder::~ImgDiffFinder()
{

}


cv::Scalar ImgDiffFinder::makeMse(const cv::Mat& imgA, const cv::Mat& imgB)
{
    cv::Mat gray_imgA, gray_imgB;
    cv::cvtColor(imgA, gray_imgA, cv::COLOR_RGB2GRAY);
    cv::cvtColor(imgB, gray_imgB, cv::COLOR_RGB2GRAY);

    const cv::Scalar mse = cv::quality::QualityMSE::compute(gray_imgA, gray_imgB, cv::noArray());
    return  mse;
}

cv::Mat ImgDiffFinder::makeDiffImg(const cv::Mat& imgA, const cv::Mat& imgB)
{
    cv::Mat diff;
    cv::absdiff(imgA, imgB, diff);
    cv::Mat gray_diff;
    cv::cvtColor(diff, gray_diff, cv::COLOR_RGB2GRAY);
    cv::cvtColor(gray_diff, diff, cv::COLOR_GRAY2RGB);
    return diff;
}

cv::Mat ImgDiffFinder::makeConcatImg(const cv::Mat& A, const cv::Mat& B, const cv::Mat& C)
{
    cv::Mat concatAB;
    cv::hconcat(A, B, concatAB);
    cv::Mat concatABC;
    cv::hconcat(concatAB, C, concatABC);
    return concatABC;
}

std::string ImgDiffFinder::makeDiffInfo(const std::string& cameraInfo, const cv::Scalar& mse)
{
    std::string info = cameraInfo + "_" + std::to_string(mse.val[0]);
    std::replace(info.begin(), info.end(), '/', '.');
    std::replace(info.begin(), info.end(), ' ', '_');
    return info;
}

bool ImgDiffFinder::differenceIsLarge(const cv::Scalar& mse)
{
    return (mse.val[0] >= 50);
}

/*!
  \fn void ImgDiffFinder::onImgDiffFinded(int ms, ImgDiffFinder::ImgDiffHandler handler)
    Blah blah
*/
void ImgDiffFinder::onImgDiffFinded(int ms, ImgDiffFinder::ImgDiffHandler handler)
{
    _timer.setInterval([this, handler]() {
        PLOG_DEBUG << "Timeout";
        auto [ img, isOk ] = _camera.get();
        if (not isOk) {
            PLOG_WARNING << _camera.info() << " is not avaliable";
            //handler(0, "", false);
            return;
        }
        _images_queue.push(img);

        if (_images_queue.size() == 2) {
            cv::Mat imgA = _images_queue.front();
            _images_queue.pop();
            cv::Mat imgB = _images_queue.front();

            const cv::Scalar mse = makeMse(imgA, imgB);
            PLOG_VERBOSE << "MSE:" << mse << ", camera: " << _camera.info();

            if (differenceIsLarge(mse)) {
                cv::Mat diffImg = makeDiffImg(imgA, imgB);
                std::string info = makeDiffInfo(_camera.info(), mse);
                cv::putText(diffImg, _camera.info(), cv::Point(0, 25), cv::FONT_HERSHEY_TRIPLEX, 0.65, cv::Scalar(100, 220, 50, 255));
                cv::putText(diffImg, std::to_string(mse[0]), cv::Point(0, 50), cv::FONT_HERSHEY_TRIPLEX, 0.85, cv::Scalar(100, 220, 50, 255));
                cv::Mat concatImg = makeConcatImg(diffImg, imgA, imgB);
                const std::string imgPath = "/tmp/" + info + ".png";
                cv::imwrite(imgPath, concatImg);
                PLOG_INFO << "DIFF:" << _camera.info() << " MSE:" << mse;
                handler(imgPath, true);
            }
        }
    }, ms);
}

void ImgDiffFinder::setCamera(HttpCamera& camera)
{
    _camera = camera;
}
