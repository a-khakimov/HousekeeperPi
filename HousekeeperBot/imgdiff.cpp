#include "imgdiff.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/quality.hpp>

ImgDiff::ImgDiff()
{
    timer = Timer();
}

ImgDiff::~ImgDiff()
{

}

void ImgDiff::run(int ms, std::function<void (double, std::string)> f)
{
    timer.stop();
    timer.setInterval([&]() {
        ImgSource isrc;
        cv::Mat img = isrc.get();
        images_queue.push(img);

        if (images_queue.size() == 2) {
            cv::Mat imgA = images_queue.front();
            images_queue.pop();
            cv::Mat imgB = images_queue.front();

            cv::Mat gray_imgA, gray_imgB;
            cv::cvtColor(imgA, gray_imgA, cv::COLOR_BGR2GRAY);
            cv::cvtColor(imgB, gray_imgB, cv::COLOR_BGR2GRAY);

            cv::Scalar mse = cv::quality::QualityMSE::compute(gray_imgA, gray_imgB, cv::noArray());

            if (mse.val[0] >= 50) {
                cv::Mat diffImg;
                cv::absdiff(imgA, imgB, diffImg);
                cv::Mat concatImgAImgB;
                cv::vconcat(imgA, imgA, concatImgAImgB);
                cv::Mat concatImgAImgBDiffImg;
                cv::vconcat(concatImgAImgB, diffImg, concatImgAImgBDiffImg);
                const std::string imgPath = "/tmp/diffImg.png";
                cv::imwrite(imgPath, concatImgAImgBDiffImg);
                f(mse.val[0], imgPath);
            }
        }
    }, ms);
}
