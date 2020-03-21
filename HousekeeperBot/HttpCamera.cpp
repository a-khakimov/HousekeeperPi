/** 
 *  @file   HttpCamera.cpp
 *  @brief  Get image from http-server 
 *  @author a-khakimov 
 ***********************************************/

#include "HttpCamera.h"

#include "plog/Log.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>


HttpCamera::HttpCamera(const std::string& host, const uint64_t port, const std::string& path) : _path(path)
{
    _info += host + ":" + std::to_string(port) + path;
    _client = std::make_shared<httplib::Client>(host, port);
}

HttpCamera::~HttpCamera()
{

}

/*!
  \fn std::tuple<cv::Mat, bool> HttpCamera::get()
    Blah blah
*/
std::tuple<cv::Mat, bool> HttpCamera::get()
{
    cv::Mat img;
    bool isOk = false;
    std::vector<char> inputImgArray;

    try {
        auto res = _client->Get(_path.c_str(), [&](const char *data, uint64_t data_length) {
            inputImgArray.insert(inputImgArray.end(), data, data + data_length);
            return true;
        });
        if (res && res->status == 200) {
            img = cv::imdecode(cv::Mat(inputImgArray), 1);
            isOk = true;
        }
    }
    catch (std::runtime_error& e) {
        PLOG_ERROR << "EXCEPTION: runtime_error - " << e.what();
        isOk = false;
    }

    return std::tuple { img, isOk };
}

std::string HttpCamera::info() const
{
    return _info;
}
