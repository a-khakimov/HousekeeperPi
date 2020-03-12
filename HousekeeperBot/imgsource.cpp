/** 
 *  @file   imgsource.cpp 
 *  @brief  Get image from http-server 
 *  @author a-khakimov 
 ***********************************************/

#include "imgsource.h"

#include "plog/Log.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>


ImgSource::ImgSource()
{
    client = std::make_shared<httplib::Client>("localhost", 1234);
}

ImgSource::~ImgSource()
{

}

/*!
  \fn bool ImgSource::isAlive()
    Blah blah
*/
bool ImgSource::isAlive()
{
    auto res = client->Get("/test");
    if (res && res->status == 200) {
        std::cout << res->body << std::endl;
    }
    return (res && res->status == 200 && res->body == "I am ok!");
}

/*!
  \fn std::tuple<cv::Mat, bool> ImgSource::get()
    Blah blah
*/
std::tuple<cv::Mat, bool> ImgSource::get()
{
    cv::Mat img;
    bool isOk = false;
    std::vector<char> inputImgArray;

    auto res = client->Get("/webcamera.png", [&](const char *data, uint64_t data_length) {
        inputImgArray.insert(inputImgArray.end(), data, data + data_length);
        return true;
    });

    if (res && res->status == 200) {
        img = cv::imdecode(cv::Mat(inputImgArray), 1);
        isOk = true;
    }

    return std::tuple { img, isOk };
}
